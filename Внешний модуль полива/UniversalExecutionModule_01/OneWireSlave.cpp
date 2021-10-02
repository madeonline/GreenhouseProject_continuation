#include "OneWireSlave.h"

// uncomment this line to enable sending messages along with errors (but takes more program memory)
//#define ERROR_MESSAGES

#ifdef ERROR_MESSAGES
#define ERROR(msg) error_(msg)
#else
#define ERROR(msg) error_(0)
#endif

namespace
{
	const unsigned long ResetMinDuration = 480;
	const unsigned long ResetMaxDuration = 900;

	const unsigned long PresenceWaitDuration = 30;
	const unsigned long PresenceDuration = 300;

	const unsigned long ReadBitSamplingTime = 25;

	const unsigned long SendBitDuration = 35;

	const byte ReceiveCommand = (byte)-1;

	void(*timerEvent)() = 0;
}

OneWireSlave OWSlave;

byte OneWireSlave::rom_[8];
byte OneWireSlave::scratchpad_[8];
Pin OneWireSlave::pin_;
byte OneWireSlave::tccr1bEnable_;

unsigned long OneWireSlave::resetStart_;
unsigned long OneWireSlave::lastReset_;

void(*OneWireSlave::receiveBitCallback_)(bool bit, bool error);
void(*OneWireSlave::bitSentCallback_)(bool error);
void(*OneWireSlave::clientReceiveCallback_)(ReceiveEvent evt, byte data);
void(*OneWireSlave::clientReceiveBitCallback_)(bool bit);

byte OneWireSlave::receivingByte_;

byte OneWireSlave::searchRomBytePos_;
byte OneWireSlave::searchRomBitPos_;
bool OneWireSlave::searchRomInverse_;
bool OneWireSlave::resumeCommandFlag_;
bool OneWireSlave::alarmedFlag_;

const byte* OneWireSlave::sendBuffer_;
byte* OneWireSlave::recvBuffer_;
short OneWireSlave::bufferLength_;
byte OneWireSlave::bufferBitPos_;
short OneWireSlave::bufferPos_;
void(*OneWireSlave::receiveBytesCallback_)(bool error);
void(*OneWireSlave::sendBytesCallback_)(bool error);

volatile bool OneWireSlave::waitingSynchronousWriteToComplete_;
volatile bool OneWireSlave::synchronousWriteError_;

bool OneWireSlave::sendingClientBytes_;

bool OneWireSlave::singleBit_;
bool OneWireSlave::singleBitRepeat_;
void(*OneWireSlave::singleBitSentCallback_)(bool error);

void(*OneWireSlave::logCallback_)(const char* message);


ISR(TIMER1_COMPA_vect) // timer1 interrupt
{
	TCCR1B = 0; // disable clock
	void(*event)() = timerEvent;
	timerEvent = 0;
	if(event)
	  event();
}

void OneWireSlave::begin(const byte* rom, byte pinNumber)
{
  //Serial.println("1");
	pin_ = Pin(pinNumber);
	resetStart_ = (unsigned long)-1;
	lastReset_ = 0;

	memcpy(rom_, rom, 7);
	rom_[7] = crc8(rom_, 7);

	resumeCommandFlag_ = false;
	alarmedFlag_       = false;

	clientReceiveBitCallback_ = 0;
	sendingClientBytes_ = false;

  //Serial.println("2");
 // log("Enabling 1-wire library")
  
	cli(); // disable interrupts
	pin_.inputMode();
	pin_.writeLow(); // make sure the internal pull-up resistor is disabled

	// prepare hardware timer
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
	tccr1bEnable_ = (1 << WGM12) | (1 << CS11) | (1 << CS10); // turn on CTC mode with 64 prescaler

	// start 1-wire activity
	beginWaitReset_();
	sei(); // enable interrupts

 // Serial.println("3");

}

void OneWireSlave::end()
{
	// log("Disabling 1-wire library");

	cli();
	disableTimer_();
	pin_.detachInterrupt();
	releaseBus_();
	sei();
}

bool OneWireSlave::write(const byte* bytes, short numBytes)
{
	// TODO: put the arduino to sleep between interrupts to save power?
	waitingSynchronousWriteToComplete_ = true;
	beginWrite(bytes, numBytes, &OneWireSlave::onSynchronousWriteComplete_);
	while (waitingSynchronousWriteToComplete_)
		delay(1);
	return !synchronousWriteError_;
}

void OneWireSlave::onSynchronousWriteComplete_(bool error)
{
	synchronousWriteError_ = error;
	waitingSynchronousWriteToComplete_ = false;
}

void OneWireSlave::beginWrite(const byte* bytes, short numBytes, void(*complete)(bool error))
{
	cli();
	endWrite_(true);
	sendingClientBytes_ = true;
	beginWriteBytes_(bytes, numBytes, complete == 0 ? noOpCallback_ : complete);
	sei();
}

void OneWireSlave::endWrite_(bool error, bool resetInterrupts)
{
	if(resetInterrupts)
		beginWaitReset_();

	if (sendingClientBytes_)
	{
		sendingClientBytes_ = false;
		if (sendBytesCallback_ != 0)
		{
			void(*callback)(bool error) = sendBytesCallback_;
			sendBytesCallback_ = noOpCallback_;
			callback(error);
		}
	}
	else if (singleBitSentCallback_ != 0)
	{
		void(*callback)(bool) = singleBitSentCallback_;
		singleBitSentCallback_ = 0;
		callback(error);
	}
}

bool OneWireSlave::writeBit(bool value)
{
	// TODO: put the arduino to sleep between interrupts to save power?
	waitingSynchronousWriteToComplete_ = true;
	beginWriteBit(value, false, &OneWireSlave::onSynchronousWriteComplete_);
	while (waitingSynchronousWriteToComplete_)
		delay(1);
	return !synchronousWriteError_;
}

void OneWireSlave::beginWriteBit(bool value, bool repeat, void(*bitSent)(bool))
{
	cli();
	endWrite_(true);

	singleBit_ = value;
	singleBitRepeat_ = repeat;
	singleBitSentCallback_ = bitSent;
	beginSendBit_(value, &OneWireSlave::onSingleBitSent_);
	sei();
}

void OneWireSlave::onSingleBitSent_(bool error)
{
	if (!error && singleBitRepeat_)
	{
		beginSendBit_(singleBit_, &OneWireSlave::onSingleBitSent_);
	}
	else
	{
		beginReceiveBytes_(scratchpad_, 1, &OneWireSlave::notifyClientByteReceived_);
	}

	if (singleBitSentCallback_ != 0)
	{
		void(*callback)(bool) = singleBitSentCallback_;
		singleBitSentCallback_ = 0;
		callback(error);
	}
}

void OneWireSlave::stopWrite()
{
	beginWrite(0, 0, 0);
}

void OneWireSlave::alarmed(bool value)
{
	alarmedFlag_ = value;
}

byte OneWireSlave::crc8(const byte* data, short numBytes)
{
	byte crc = 0;

	while (numBytes--) {
		byte inbyte = *data++;
		for (byte i = 8; i; i--) {
			byte mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

void OneWireSlave::setTimerEvent_(short delayMicroSeconds, void(*handler)())
{
	delayMicroSeconds -= 10; // remove overhead (tuned on Arduino Uno)

	short skipTicks = (delayMicroSeconds - 3) / 4; // round the micro seconds delay to a number of ticks to skip (4us per tick, so 4us must skip 0 tick, 8us must skip 1 tick, etc.)
	if (skipTicks < 1) skipTicks = 1;
	TCNT1 = 0;
	OCR1A = skipTicks;
	timerEvent = handler;
	TCCR1B = tccr1bEnable_;
}

void OneWireSlave::disableTimer_()
{
	TCCR1B = 0;
}

void OneWireSlave::onEnterInterrupt_()
{
}

void OneWireSlave::onLeaveInterrupt_()
{
}

void OneWireSlave::error_(const char* message)
{
	if (logCallback_ != 0)
		logCallback_(message);
	endWrite_(true);
	if (clientReceiveCallback_ != 0)
		clientReceiveCallback_(RE_Error, 0);
}

void OneWireSlave::pullLow_()
{
	pin_.outputMode();
	pin_.writeLow();
}

void OneWireSlave::releaseBus_()
{
	pin_.inputMode();
}

void OneWireSlave::beginResetDetection_()
{
 // log("begin reset detection");
	setTimerEvent_(ResetMinDuration - 50, &OneWireSlave::resetCheck_);
	resetStart_ = micros() - 50;
}

void OneWireSlave::beginResetDetectionSendZero_()
{
	setTimerEvent_(ResetMinDuration - SendBitDuration - 50, &OneWireSlave::resetCheck_);
	resetStart_ = micros() - SendBitDuration - 50;
}

void OneWireSlave::cancelResetDetection_()
{
	disableTimer_();
	resetStart_ = (unsigned long)-1;
 // log("cancel reset detection");
}

void OneWireSlave::resetCheck_()
{
	onEnterInterrupt_();
	if (!pin_.read())
	{
		pin_.attachInterrupt(&OneWireSlave::waitReset_, CHANGE);
	//	 log("Reset detected during another operation");
	}
	onLeaveInterrupt_();
}

void OneWireSlave::beginReceiveBit_(void(*completeCallback)(bool bit, bool error))
{
	receiveBitCallback_ = completeCallback;
	pin_.attachInterrupt(&OneWireSlave::receive_, FALLING);
}

void OneWireSlave::receive_()
{
	onEnterInterrupt_();

	pin_.detachInterrupt();
	setTimerEvent_(ReadBitSamplingTime, &OneWireSlave::readBit_);

	onLeaveInterrupt_();
}

void OneWireSlave::readBit_()
{
	onEnterInterrupt_();

	bool bit = pin_.read();
	if (bit)
		cancelResetDetection_();
	else
		beginResetDetection_();
	receiveBitCallback_(bit, false);
	//dbgOutput.writeLow();
	//dbgOutput.writeHigh();

	onLeaveInterrupt_();
}

void OneWireSlave::beginSendBit_(bool bit, void(*completeCallback)(bool error))
{
	bitSentCallback_ = completeCallback;
	if (bit)
	{
		pin_.attachInterrupt(&OneWireSlave::sendBitOne_, FALLING);
	}
	else
	{
		pin_.attachInterrupt(&OneWireSlave::sendBitZero_, FALLING);
	}
}

void OneWireSlave::sendBitOne_()
{
	onEnterInterrupt_();

	beginResetDetection_();
	bitSentCallback_(false);

	onLeaveInterrupt_();
}

void OneWireSlave::sendBitZero_()
{
	pullLow_(); // this must be executed first because the timing is very tight with some master devices

	onEnterInterrupt_();

	pin_.detachInterrupt();
	setTimerEvent_(SendBitDuration, &OneWireSlave::endSendBitZero_);

	onLeaveInterrupt_();
}

void OneWireSlave::endSendBitZero_()
{
	onEnterInterrupt_();

	releaseBus_();
	beginResetDetectionSendZero_();
	bitSentCallback_(false);

	onLeaveInterrupt_();
}

void OneWireSlave::beginWaitReset_()
{
	disableTimer_();
	pin_.attachInterrupt(&OneWireSlave::waitReset_, CHANGE);
	resetStart_ = (unsigned int)-1;
}

//bool WAIT_RESET_FLAG = false;

void OneWireSlave::waitReset_()
{
 //  log("wait reset");

// WAIT_RESET_FLAG = true;
   
	onEnterInterrupt_();
	bool state = pin_.read();
	unsigned long now = micros();
	if (state)
	{
		if (resetStart_ == (unsigned int)-1)
		{
			onLeaveInterrupt_();
			return;
		}

		unsigned long resetDuration = now - resetStart_;
		resetStart_ = (unsigned int)-1;
		if (resetDuration >= ResetMinDuration)
		{
			if (resetDuration > ResetMaxDuration)
			{
				ERROR("Reset too long");
				onLeaveInterrupt_();
				return;
			}

			lastReset_ = now;
			pin_.detachInterrupt();
			setTimerEvent_(PresenceWaitDuration - (micros() - now), &OneWireSlave::beginPresence_);
			endWrite_(true, false);
			if (clientReceiveCallback_ != 0)
				clientReceiveCallback_(RE_Reset, 0);
		}
	}
	else
	{
		resetStart_ = now;
	}
	onLeaveInterrupt_();
}

void OneWireSlave::beginPresence_()
{
	pullLow_();
	setTimerEvent_(PresenceDuration, &OneWireSlave::endPresence_);
}

void OneWireSlave::endPresence_()
{
	releaseBus_();

	beginWaitCommand_();
}

void OneWireSlave::beginWaitCommand_()
{
	bufferPos_ = ReceiveCommand;
	beginReceive_();
}

void OneWireSlave::beginReceive_()
{
	receivingByte_ = 0;
	bufferBitPos_ = 0;
	beginReceiveBit_(&OneWireSlave::onBitReceived_);
}

void OneWireSlave::onBitReceived_(bool bit, bool error)
{
	if (error)
	{
		ERROR("Invalid bit");
		if (bufferPos_ >= 0)
			receiveBytesCallback_(true);
		return;
	}

	receivingByte_ |= ((bit ? 1 : 0) << bufferBitPos_);
	++bufferBitPos_;

	if (clientReceiveBitCallback_ != 0 && bufferPos_ != ReceiveCommand)
		clientReceiveBitCallback_(bit);

	if (bufferBitPos_ == 8)
	{
		// log("received byte");
	//	 log((long)receivingByte_);

		if (bufferPos_ == ReceiveCommand)
		{
			bufferPos_ = 0;
			switch (receivingByte_)
			{
			case 0xF0: // SEARCH ROM
				resumeCommandFlag_ = false;
				beginSearchRom_();
				return;
			case 0xEC: // CONDITIONAL SEARCH ROM
				resumeCommandFlag_ = false;
				if (alarmedFlag_)
				{
					beginSearchRom_();
				}
				else
				{
					beginWaitReset_();
				}
				return;
			case 0x33: // READ ROM
				resumeCommandFlag_ = false;
				beginWriteBytes_(rom_, 8, &OneWireSlave::noOpCallback_);
				return;
			case 0x55: // MATCH ROM
				resumeCommandFlag_ = false;
				beginReceiveBytes_(scratchpad_, 8, &OneWireSlave::matchRomBytesReceived_);
				return;
			case 0xCC: // SKIP ROM
				resumeCommandFlag_ = false;
				beginReceiveBytes_(scratchpad_, 1, &OneWireSlave::notifyClientByteReceived_);
				return;
			case 0xA5: // RESUME
				if (resumeCommandFlag_)
				{
					beginReceiveBytes_(scratchpad_, 1, &OneWireSlave::notifyClientByteReceived_);
				}
				else
				{
					beginWaitReset_();
				}
				return;
			default:
				ERROR("Unknown command");
				return;
			}
		}
		else
		{
			recvBuffer_[bufferPos_++] = receivingByte_;
			receivingByte_ = 0;
			bufferBitPos_ = 0;
			if (bufferPos_ == bufferLength_)
			{
				beginWaitReset_();
				receiveBytesCallback_(false);
				return;
			}
		}
	}
	
	beginReceiveBit_(&OneWireSlave::onBitReceived_);
}

void OneWireSlave::beginSearchRom_()
{
	searchRomBytePos_ = 0;
	searchRomBitPos_ = 0;
	searchRomInverse_ = false;

	beginSearchRomSendBit_();
}

void OneWireSlave::beginSearchRomSendBit_()
{
	byte currentByte = rom_[searchRomBytePos_];
	bool currentBit = bitRead(currentByte, searchRomBitPos_);
	bool bitToSend = searchRomInverse_ ? !currentBit : currentBit;

	beginSendBit_(bitToSend, &OneWireSlave::continueSearchRom_);
}

void OneWireSlave::continueSearchRom_(bool error)
{
	if (error)
	{
		ERROR("Failed to send bit");
		return;
	}

	searchRomInverse_ = !searchRomInverse_;
	if (searchRomInverse_)
	{
		beginSearchRomSendBit_();
	}
	else
	{
		beginReceiveBit_(&OneWireSlave::searchRomOnBitReceived_);
	}
}

void OneWireSlave::searchRomOnBitReceived_(bool bit, bool error)
{
	if (error)
	{
		ERROR("Bit read error during ROM search");
		return;
	}

	byte currentByte = rom_[searchRomBytePos_];
	bool currentBit = bitRead(currentByte, searchRomBitPos_);

	if (bit == currentBit)
	{
		++searchRomBitPos_;
		if (searchRomBitPos_ == 8)
		{
			searchRomBitPos_ = 0;
			++searchRomBytePos_;
		}

		if (searchRomBytePos_ == 8)
		{
		//	 log("ROM sent entirely");

			beginWaitReset_();
		}
		else
		{
			beginSearchRomSendBit_();
		}
	}
	else
	{
		// log("Leaving ROM search");
		beginWaitReset_();
	}
}

void OneWireSlave::beginWriteBytes_(const byte* data, short numBytes, void(*complete)(bool error))
{
	sendBuffer_ = data;
	bufferLength_ = numBytes;
	bufferPos_ = 0;
	bufferBitPos_ = 0;
	sendBytesCallback_ = complete;

	if (sendBuffer_ != 0 && bufferLength_ > 0)
	{
		bool bit = bitRead(sendBuffer_[0], 0);
		beginSendBit_(bit, &OneWireSlave::bitSent_);
	}
	else
	{
		endWrite_(true);
		beginReceiveBytes_(scratchpad_, 1, &OneWireSlave::notifyClientByteReceived_);
	}
}

void OneWireSlave::bitSent_(bool error)
{
	if (error)
	{
		ERROR("error sending a bit");
		sendBytesCallback_(true);
		return;
	}

	++bufferBitPos_;
	if (bufferBitPos_ == 8)
	{
		bufferBitPos_ = 0;
		++bufferPos_;
	}

	if (bufferPos_ == bufferLength_)
	{
		endWrite_(false);
		sendBytesCallback_(false);
		return;
	}

	bool bit = bitRead(sendBuffer_[bufferPos_], bufferBitPos_);
	beginSendBit_(bit, &OneWireSlave::bitSent_);
}

void OneWireSlave::beginReceiveBytes_(byte* buffer, short numBytes, void(*complete)(bool error))
{
	recvBuffer_ = buffer;
	bufferLength_ = numBytes;
	bufferPos_ = 0;
	receiveBytesCallback_ = complete;
	beginReceive_();
}

void OneWireSlave::noOpCallback_(bool error)
{
	if (error)
		ERROR("error during an internal 1-wire operation");
}

void OneWireSlave::matchRomBytesReceived_(bool error)
{
	if (error)
	{
		resumeCommandFlag_ = false;
		ERROR("error receiving match rom bytes");
		return;
	}

	if (memcmp(rom_, scratchpad_, 8) == 0)
	{
	//	 log("ROM matched");
		resumeCommandFlag_ = true;
		beginReceiveBytes_(scratchpad_, 1, &OneWireSlave::notifyClientByteReceived_);
	}
	else
	{
	//	 log("ROM not matched");
		resumeCommandFlag_ = false;
		beginWaitReset_();
	}
}

void OneWireSlave::notifyClientByteReceived_(bool error)
{
	if (error)
	{
		if (clientReceiveCallback_ != 0)
			clientReceiveCallback_(RE_Error, 0);
		ERROR("error receiving custom bytes");
		return;
	}

	beginReceiveBytes_(scratchpad_, 1, &OneWireSlave::notifyClientByteReceived_);
	if (clientReceiveCallback_ != 0)
		clientReceiveCallback_(RE_Byte, scratchpad_[0]);
}
