//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screen2.h"
#include "FileUtils.h"
#include "TinyVector.h"
#include "CONFIG.h"
#include "Settings.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ListFilesScreen* listLogFilesScreen = NULL;
ListFilesScreen* listEthalonsFilesScreen = NULL;
EthalonChartScreen* ethalonChartScreen = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Screen2::Screen2() : AbstractTFTScreen("Settings")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::doSetup(TFTMenu* menu)
{

	int menu_height = 30;
	int button_gap = 15;

  // тут настраиваемся, например, можем добавлять кнопки
  filesButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Файлы");
  menu_height += BUTTON_HEIGHT + button_gap;
  sdScreenButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "SD");
  menu_height += BUTTON_HEIGHT + button_gap;
  adjustTimeButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "ЧАСЫ");
  menu_height += BUTTON_HEIGHT + button_gap;
  paramsScreenButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Параметры");
  menu_height += BUTTON_HEIGHT + button_gap;
  backButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");


  // добавляем разные подэкраны
  Screen.addScreen(SDScreen::create());
  Screen.addScreen(SDInfoScreen::create());
  Screen.addScreen(SDFormatScreen::create());

  listLogFilesScreen = ListFilesScreen::create(vtLogsListing);
  listEthalonsFilesScreen = ListFilesScreen::create(vtEthalonsListing);
  ethalonChartScreen = EthalonChartScreen::create();
  
  Screen.addScreen(listLogFilesScreen);
  Screen.addScreen(listEthalonsFilesScreen);
  Screen.addScreen(ethalonChartScreen);
  
  Screen.addScreen(EthalonScreen::create());
  Screen.addScreen(EthalonRecordScreen::create());
  Screen.addScreen(FilesScreen::create());
  Screen.addScreen(FilesListScreen::create());
  Screen.addScreen(ClearDataScreen::create());
  Screen.addScreen(SystemScreen::create());
  Screen.addScreen(CommunicateScreen::create());
  Screen.addScreen(ParamsScreen::create());
  Screen.addScreen(InductiveSensorScreen::create());
  Screen.addScreen(TransformerScreen::create());
  Screen.addScreen(PulsesCountScreen::create());
  Screen.addScreen(PulsesDeltaScreen::create());
  Screen.addScreen(MotoresourceScreen::create());
  Screen.addScreen(MotoresourceMaxScreen::create());
  Screen.addScreen(BorderMaxScreen::create());
  Screen.addScreen(BorderMinScreen::create());
  Screen.addScreen(AcsDelayScreen::create());
  Screen.addScreen(RelayDelayScreen::create());
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::doUpdate(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::doDraw(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Screen2::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Main"); // переключаемся на главный экран
  else 
  if(pressedButton == sdScreenButton)
    menu->switchToScreen("SDScreen"); // переключаемся на экран работы с SD
  else 
  if(pressedButton == adjustTimeButton)
    menu->switchToScreen("RTCSettings"); // переключаемся на экран настройки часов
  else 
  if(pressedButton == filesButton)
     menu->switchToScreen("FilesScreen"); // переключаемся на экран "Файлы"
  else 
  if(pressedButton == paramsScreenButton)
    menu->switchToScreen("ParamsScreen"); // переключаемся на экран "Параметры"

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FilesScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FilesScreen::FilesScreen() : AbstractTFTScreen("FilesScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  

  int menu_height = 30;
  int button_gap = 15;

  // тут настраиваемся, например, можем добавлять кнопки
  filesListButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Список");
  menu_height += BUTTON_HEIGHT + button_gap;
  ethalonScreenButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Эталоны");
  menu_height += BUTTON_HEIGHT + button_gap;
  viewLogButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Логи");
  menu_height += BUTTON_HEIGHT + button_gap;
  clearDataButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Очистка");
  menu_height += BUTTON_HEIGHT + button_gap;
  backButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран настроек
  else if(pressedButton == filesListButton)
    menu->switchToScreen("FilesListScreen");
  else if(pressedButton == ethalonScreenButton)
    menu->switchToScreen("EthalonScreen");
  else if(pressedButton == viewLogButton)
  {
    listLogFilesScreen->rescanFiles();
    menu->switchToScreen(listLogFilesScreen); // переключаемся на экран просмотра логов
  }
  else if(pressedButton == clearDataButton)
    menu->switchToScreen("ClearDataScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ClearDataScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ClearDataScreen::ClearDataScreen() : AbstractTFTScreen("ClearDataScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearDataScreen::doSetup(TFTMenu* menu)
{
  // тут настраиваемся, например, можем добавлять кнопки
  clearDataButton = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Очистить");
//  reserved = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearDataScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearDataScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearDataScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("FilesScreen"); // переключаемся на экран настроек
  else if(pressedButton == clearDataButton)
    menu->switchToScreen("StartClearDataScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDScreen::SDScreen() : AbstractTFTScreen("SDScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);

  int menu_height = 30;
  int button_gap = 15;

  // тут настраиваемся, например, можем добавлять кнопки
  sdInfoButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "SD-инфо");
  menu_height += BUTTON_HEIGHT + button_gap;
  formatSDButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Формат SD");
  menu_height += BUTTON_HEIGHT + button_gap;
//  reserved = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  menu_height += BUTTON_HEIGHT + button_gap;
//  reserved = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  menu_height += BUTTON_HEIGHT + button_gap;
  backButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран настроек
  else if(pressedButton == sdInfoButton)
    menu->switchToScreen("SDInfoScreen");
  else if(pressedButton == formatSDButton)
    menu->switchToScreen("FormatSDScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SystemScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SystemScreen::SystemScreen() : AbstractTFTScreen("SystemScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemScreen::doSetup(TFTMenu* menu)
{
   screenButtons->setButtonColors(TFT_BUTTON_COLORS2);

  // тут настраиваемся, например, можем добавлять кнопки
  communicateButton = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Коммуникатор");
//  reserved = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings"); // переключаемся на экран настроек
  else if(pressedButton == communicateButton)
    menu->switchToScreen("CommunicateScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CommunicateScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CommunicateScreen::CommunicateScreen() : AbstractTFTScreen("CommunicateScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CommunicateScreen::doSetup(TFTMenu* menu)
{
  
  // тут настраиваемся, например, можем добавлять кнопки
  rs485Button = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "RS485");
  wiFiButton = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "WiFi");
//  reserved = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CommunicateScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CommunicateScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CommunicateScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("SystemScreen");
  else if(pressedButton == rs485Button)
    menu->switchToScreen("RS485Screen");
  else if(pressedButton == wiFiButton)
    menu->switchToScreen("WiFiScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ParamsScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ParamsScreen::ParamsScreen() : AbstractTFTScreen("ParamsScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParamsScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  int menu_height = 30;
  int button_gap = 15;

  // тут настраиваемся, например, можем добавлять кнопки
  createEncoderChartButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "График создать");
  menu_height += BUTTON_HEIGHT + button_gap;
  transformerButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Пороги транс.");
  menu_height += BUTTON_HEIGHT + button_gap;
  acsDelayButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Задержка АСУ");
  menu_height += BUTTON_HEIGHT + button_gap;
  relayDelayButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "Задержка имп.");
  menu_height += BUTTON_HEIGHT + button_gap;
  backButton = screenButtons->addButton(100, menu_height, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParamsScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParamsScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParamsScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("Settings");
  else if(pressedButton == createEncoderChartButton)
    menu->switchToScreen("CreateEncoderChartScreen");
  else if(pressedButton == transformerButton)
    menu->switchToScreen("TransformerScreen");
  else if(pressedButton == acsDelayButton)
    menu->switchToScreen("AcsDelayScreen");
  else if(pressedButton == relayDelayButton)
    menu->switchToScreen("RelayDelayScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// InductiveSensorScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
InductiveSensorScreen::InductiveSensorScreen() : AbstractTFTScreen("InductiveSensorScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InductiveSensorScreen::doSetup(TFTMenu* menu)
{
  // тут настраиваемся, например, можем добавлять кнопки
  pulsesCountButton = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Импульсы");
  pulseDeltaButton = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "Дельты");
  motoresourceButton = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "Ресурс тек.");
  motoresourceMaxButton = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "Ресурс макс.");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InductiveSensorScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InductiveSensorScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InductiveSensorScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("ParamsScreen");
  else if(pressedButton == pulsesCountButton)
    menu->switchToScreen("PulsesCountScreen");
  else if(pressedButton == pulseDeltaButton)
    menu->switchToScreen("PulsesDeltaScreen");
  else if(pressedButton == motoresourceButton)
    menu->switchToScreen("MotoresourceScreen");
  else if(pressedButton == motoresourceMaxButton)
    menu->switchToScreen("MotoresourceMaxScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// PulsesCountScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PulsesCountScreen::PulsesCountScreen() : AbstractTFTScreen("PulsesCountScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesCountScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  currentEditedButton = -1;
  channel1PulsesVal = Settings.getChannelPulses(0);
  //DEPRECATED: channel2PulsesVal = Settings.getChannelPulses(1);
  //DEPRECATED: channel3PulsesVal = Settings.getChannelPulses(2);
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, 30, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1PulsesVal.c_str());
  //DEPRECATED: channel2Button = screenButtons->addButton(120, 65, 95, 30, channel2PulsesVal.c_str());
  //DEPRECATED: channel3Button = screenButtons->addButton(120, 100, 95, 30, channel3PulsesVal.c_str());
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel2Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel3Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesCountScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1PulsesVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1PulsesVal.c_str());
      Settings.setChannelPulses(0,channel1PulsesVal.toInt());
    }
	/*
	//DEPRECATED:
    else if(currentEditedButton == channel2Button)
    {
      channel2PulsesVal = enteredValue;
      screenButtons->relabelButton(channel2Button,channel2PulsesVal.c_str());
      Settings.setChannelPulses(1,channel2PulsesVal.toInt());
    }
    else if(currentEditedButton == channel3Button)
    {
      channel3PulsesVal = enteredValue;
      screenButtons->relabelButton(channel3Button,channel3PulsesVal.c_str());
      Settings.setChannelPulses(2,channel3PulsesVal.toInt());
    }
	*/

  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesCountScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesCountScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Импульсы",2,2);
  menu->print("Канал 1:", 2, 37);
  //DEPRECATED: menu->print("Канал 2:", 2, 72);
  //DEPRECATED: menu->print("Канал 3:", 2, 107);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesCountScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("InductiveSensorScreen");
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 5);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// PulsesDeltaScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PulsesDeltaScreen::PulsesDeltaScreen() : AbstractTFTScreen("PulsesDeltaScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesDeltaScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  currentEditedButton = -1;
  channel1DeltaVal = Settings.getChannelDelta(0);
  //DEPRECATED: channel2DeltaVal = Settings.getChannelDelta(1);
  //DEPRECATED: channel3DeltaVal = Settings.getChannelDelta(2);
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, 30, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1DeltaVal.c_str());
  //DEPRECATED: channel2Button = screenButtons->addButton(120, 65, 95, 30, channel2DeltaVal.c_str());
  //DEPRECATED: channel3Button = screenButtons->addButton(120, 100, 95, 30, channel3DeltaVal.c_str());
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel2Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel3Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesDeltaScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1DeltaVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1DeltaVal.c_str());
      Settings.setChannelDelta(0,channel1DeltaVal.toInt());
    }
	/*
	//DEPRECATED:
    else if(currentEditedButton == channel2Button)
    {
      channel2DeltaVal = enteredValue;
      screenButtons->relabelButton(channel2Button,channel2DeltaVal.c_str());
      Settings.setChannelDelta(1,channel2DeltaVal.toInt());
    }
    else if(currentEditedButton == channel3Button)
    {
      channel3DeltaVal = enteredValue;
      screenButtons->relabelButton(channel3Button,channel3DeltaVal.c_str());
      Settings.setChannelDelta(2,channel3DeltaVal.toInt());
    }
	*/

  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesDeltaScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesDeltaScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Дельты",2,2);
  menu->print("Канал 1:", 2, 37);
  //DEPRECATED: menu->print("Канал 2:", 2, 72);
  //DEPRECATED: menu->print("Канал 3:", 2, 107);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PulsesDeltaScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("InductiveSensorScreen");
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 3);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MotoresourceScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotoresourceScreen::MotoresourceScreen() : AbstractTFTScreen("MotoresourceScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceScreen::onActivate()
{
  channel1MotoresourceVal = Settings.getMotoresource(0);
  //DEPRECATED: channel2MotoresourceVal = Settings.getMotoresource(1);
  //DEPRECATED: channel3MotoresourceVal = Settings.getMotoresource(2);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  currentEditedButton = -1;
  onActivate();
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1MotoresourceVal.c_str());
  //DEPRECATED: channel2Button = screenButtons->addButton(120, 65, 95, BUTTON_HEIGHT, channel2MotoresourceVal.c_str());
  //DEPRECATED: channel3Button = screenButtons->addButton(120, 100, 95, BUTTON_HEIGHT, channel3MotoresourceVal.c_str());
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  resetButton = screenButtons->addButton(113, 142, 100, BUTTON_HEIGHT, "СБРОС");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel2Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel3Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1MotoresourceVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1MotoresourceVal.c_str());
      Settings.setMotoresource(0,channel1MotoresourceVal.toInt());
    }
	/*
	//DEPRECATED:
    else if(currentEditedButton == channel2Button)
    {
      channel2MotoresourceVal = enteredValue;
      screenButtons->relabelButton(channel2Button,channel2MotoresourceVal.c_str());
      Settings.setMotoresource(1,channel2MotoresourceVal.toInt());
    }
    else if(currentEditedButton == channel3Button)
    {
      channel3MotoresourceVal = enteredValue;
      screenButtons->relabelButton(channel3Button,channel3MotoresourceVal.c_str());
      Settings.setMotoresource(2,channel3MotoresourceVal.toInt());
    }
	*/

  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Ресурс тек.",2,2);
  menu->print("Канал 1:", 2, 37);
  //DEPRECATED: menu->print("Канал 2:", 2, 72);
  //DEPRECATED: menu->print("Канал 3:", 2, 107);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("InductiveSensorScreen");
  else if(pressedButton == resetButton)
  {
      channel1MotoresourceVal = '0';
	  //DEPRECATED: channel2MotoresourceVal = '0';
	  //DEPRECATED: channel3MotoresourceVal = '0';
      
      screenButtons->relabelButton(channel1Button,channel1MotoresourceVal.c_str(),true);
	  //DEPRECATED: screenButtons->relabelButton(channel2Button,channel2MotoresourceVal.c_str(),true);
	  //DEPRECATED: screenButtons->relabelButton(channel3Button,channel3MotoresourceVal.c_str(),true);
      
      Settings.setMotoresource(0,channel1MotoresourceVal.toInt());    
	  //DEPRECATED: Settings.setMotoresource(1,channel2MotoresourceVal.toInt());    
	  //DEPRECATED: Settings.setMotoresource(2,channel3MotoresourceVal.toInt());    
  }
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 8);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MotoresourceMaxScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotoresourceMaxScreen::MotoresourceMaxScreen() : AbstractTFTScreen("MotoresourceMaxScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceMaxScreen::onActivate()
{
  channel1MotoresourceVal = Settings.getMotoresourceMax(0);
  //DEPRECATED: channel2MotoresourceVal = Settings.getMotoresourceMax(1);
  //DEPRECATED: channel3MotoresourceVal = Settings.getMotoresourceMax(2);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceMaxScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  currentEditedButton = -1;
  onActivate();
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1MotoresourceVal.c_str());
  //DEPRECATED: channel2Button = screenButtons->addButton(120, 65, 95, BUTTON_HEIGHT, channel2MotoresourceVal.c_str());
  //DEPRECATED: channel3Button = screenButtons->addButton(120, 100, 95, BUTTON_HEIGHT, channel3MotoresourceVal.c_str());
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  resetButton = screenButtons->addButton(113, 142, 100, BUTTON_HEIGHT, "СБРОС");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel2Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonBackColor(channel3Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceMaxScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1MotoresourceVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1MotoresourceVal.c_str());
      Settings.setMotoresourceMax(0,channel1MotoresourceVal.toInt());
    }
	/*
	//DEPRECATED:
    else if(currentEditedButton == channel2Button)
    {
      channel2MotoresourceVal = enteredValue;
      screenButtons->relabelButton(channel2Button,channel2MotoresourceVal.c_str());
      Settings.setMotoresourceMax(1,channel2MotoresourceVal.toInt());
    }
    else if(currentEditedButton == channel3Button)
    {
      channel3MotoresourceVal = enteredValue;
      screenButtons->relabelButton(channel3Button,channel3MotoresourceVal.c_str());
      Settings.setMotoresourceMax(2,channel3MotoresourceVal.toInt());
    }
	*/

  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceMaxScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceMaxScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Ресурс макс.",2,2);
  menu->print("Канал 1:", 2, 37);
  //DEPRECATED: menu->print("Канал 2:", 2, 72);
  //DEPRECATED: menu->print("Канал 3:", 2, 107);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotoresourceMaxScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("InductiveSensorScreen");
  else if(pressedButton == resetButton)
  {
      channel1MotoresourceVal = '0';
	  //DEPRECATED: channel2MotoresourceVal = '0';
	  //DEPRECATED: channel3MotoresourceVal = '0';
      
      screenButtons->relabelButton(channel1Button,channel1MotoresourceVal.c_str(),true);
	  //DEPRECATED: screenButtons->relabelButton(channel2Button,channel2MotoresourceVal.c_str(),true);
	  //DEPRECATED: screenButtons->relabelButton(channel3Button,channel3MotoresourceVal.c_str(),true);
      
      Settings.setMotoresourceMax(0,channel1MotoresourceVal.toInt());    
	  //DEPRECATED: Settings.setMotoresourceMax(1,channel2MotoresourceVal.toInt());    
	  //DEPRECATED: Settings.setMotoresourceMax(2,channel3MotoresourceVal.toInt());    
  }  
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 8);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TransformerScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TransformerScreen::TransformerScreen() : AbstractTFTScreen("TransformerScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformerScreen::doSetup(TFTMenu* menu)
{
  // тут настраиваемся, например, можем добавлять кнопки
  borderMaxButton = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Порог макс.");
  borderMinButton = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "Порог мин.");
//  acsDelayButton = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "Задержка АСУ");
//  reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformerScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformerScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformerScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("ParamsScreen");
  else if(pressedButton == borderMaxButton)
    menu->switchToScreen("BorderMaxScreen");
  else if(pressedButton == borderMinButton)
    menu->switchToScreen("BorderMinScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// BorderMaxScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BorderMaxScreen::BorderMaxScreen() : AbstractTFTScreen("BorderMaxScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMaxScreen::onActivate()
{
  channel1BorderVal = Settings.getTransformerHighBorder();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMaxScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  currentEditedButton = -1;
  onActivate();
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1BorderVal.c_str());
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  resetButton = screenButtons->addButton(113, 142, 100, BUTTON_HEIGHT, "СБРОС");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMaxScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1BorderVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1BorderVal.c_str());
      Settings.setTransformerHighBorder(channel1BorderVal.toInt());
    }
   
  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMaxScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMaxScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Порог макс.",2,2);
  menu->print("Порог:", 2, 37);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMaxScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("TransformerScreen");
  else if(pressedButton == resetButton)
  {
      channel1BorderVal = TRANSFORMER_HIGH_DEFAULT_BORDER;
      
      screenButtons->relabelButton(channel1Button,channel1BorderVal.c_str(),true);
      
      Settings.setTransformerHighBorder(channel1BorderVal.toInt());    
  }  
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 8);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// BorderMinScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BorderMinScreen::BorderMinScreen() : AbstractTFTScreen("BorderMinScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMinScreen::onActivate()
{
  channel1BorderVal = Settings.getTransformerLowBorder();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMinScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS2);
  
  currentEditedButton = -1;
  onActivate();
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1BorderVal.c_str());
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  resetButton = screenButtons->addButton(113, 142, 100, BUTTON_HEIGHT, "СБРОС");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMinScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1BorderVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1BorderVal.c_str());
      Settings.setTransformerLowBorder(channel1BorderVal.toInt());
    }
   
  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMinScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMinScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Порог мин.",2,2);
  menu->print("Порог:", 2, 37);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BorderMinScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("TransformerScreen");
  else if(pressedButton == resetButton)
  {
      channel1BorderVal = TRANSFORMER_LOW_DEFAULT_BORDER;
      
      screenButtons->relabelButton(channel1Button,channel1BorderVal.c_str(),true);
      
      Settings.setTransformerLowBorder(channel1BorderVal.toInt());    
  }  
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 8);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// AcsDelayScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AcsDelayScreen::AcsDelayScreen() : AbstractTFTScreen("AcsDelayScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AcsDelayScreen::onActivate()
{
  channel1AcsDelayVal = Settings.getACSDelay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AcsDelayScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS);
  
  currentEditedButton = -1;
  onActivate();
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1AcsDelayVal.c_str());
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  resetButton = screenButtons->addButton(113, 142, 100, BUTTON_HEIGHT, "СБРОС");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AcsDelayScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1AcsDelayVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1AcsDelayVal.c_str());
      Settings.setACSDelay(channel1AcsDelayVal.toInt());
    }
   
  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AcsDelayScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AcsDelayScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Задержка АСУ",2,2);
  menu->print("Сигнал:", 2, 37);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AcsDelayScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("ParamsScreen");
  else if(pressedButton == resetButton)
  {
      channel1AcsDelayVal = ACS_SIGNAL_DELAY;
      
      screenButtons->relabelButton(channel1Button,channel1AcsDelayVal.c_str(),true);
      
      Settings.setACSDelay(channel1AcsDelayVal.toInt());    
  }  
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 4);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// RelayDelayScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RelayDelayScreen::RelayDelayScreen() : AbstractTFTScreen("RelayDelayScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RelayDelayScreen::onActivate()
{
  channel1RelayDelayVal = Settings.getRelayDelay()/1000;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RelayDelayScreen::doSetup(TFTMenu* menu)
{
  screenButtons->setButtonColors(TFT_BUTTON_COLORS);
  
  currentEditedButton = -1;
  onActivate();
  
  // тут настраиваемся, например, можем добавлять кнопки
  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  channel1Button = screenButtons->addButton(120, 30, 95, BUTTON_HEIGHT, channel1RelayDelayVal.c_str());
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  resetButton = screenButtons->addButton(113, 142, 100, BUTTON_HEIGHT, "СБРОС");

  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RelayDelayScreen::onKeyboardInput(bool enterPressed, const String& enteredValue)
{
  if(!enterPressed)
    return;

    if(currentEditedButton == channel1Button)
    {
      channel1RelayDelayVal = enteredValue;
      screenButtons->relabelButton(channel1Button,channel1RelayDelayVal.c_str());
      Settings.setRelayDelay(channel1RelayDelayVal.toInt()*1000);
    }
   
  currentEditedButton = -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RelayDelayScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RelayDelayScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(BigRusFont);
  dc->setColor(VGA_WHITE);

  menu->print("Задержка имп.",2,2);
  menu->print("Время:", 2, 37);
  
  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RelayDelayScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("ParamsScreen");
  else if(pressedButton == resetButton)
  {
      channel1RelayDelayVal = RELAY_WANT_DATA_AFTER/1000;
      
      screenButtons->relabelButton(channel1Button,channel1RelayDelayVal.c_str(),true);
      
      Settings.setRelayDelay(channel1RelayDelayVal.toInt()*1000);    
  }  
  else
  {
    currentEditedButton = pressedButton;
    String strValToEdit = screenButtons->getLabel(currentEditedButton);
    ScreenKeyboard->show(ktDigits,strValToEdit,this,this, 4);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FilesListScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FilesListScreen::FilesListScreen() : AbstractTFTScreen("FilesListScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesListScreen::doSetup(TFTMenu* menu)
{
  // тут настраиваемся, например, можем добавлять кнопки
  viewChartsButton = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Графики");
//  reserved = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesListScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesListScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilesListScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("FilesScreen"); // переключаемся на экран настроек
  else if(pressedButton == viewChartsButton)
    menu->switchToScreen("ViewChartsScreen");
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// EthalonScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EthalonScreen::EthalonScreen() : AbstractTFTScreen("EthalonScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::doSetup(TFTMenu* menu)
{

  viewEthalonButton = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Просмотр");
  createEthalonButton = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "Создание");
  singleButton = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "Однократный");
  ethalonFlagButton = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "Флаг эталон");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("FilesScreen"); // переключаемся на экран работы с файлами
  else if(pressedButton == viewEthalonButton)
  {
    listEthalonsFilesScreen->rescanFiles();
    menu->switchToScreen(listEthalonsFilesScreen); // переключаемся на экран просмотра эталонов
  }
  else if(pressedButton == createEthalonButton)
  {
    Vector<const char*> lines;
    lines.push_back("Начать");
    lines.push_back("запись");
    lines.push_back("эталона?");    
    MessageBox->confirm(lines,"EthalonRecordScreen","EthalonScreen");   
  }
  else if(pressedButton == singleButton)
    menu->switchToScreen("SingleScreen");    
  else if(pressedButton == ethalonFlagButton)
    menu->switchToScreen("EthalonFlagScreen");    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// EthalonChartScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EthalonChartScreen::EthalonChartScreen() : AbstractTFTScreen("EthalonChartScreen")
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonChartScreen::doSetup(TFTMenu* menu)
{

  //reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  //reserved = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  //reserved = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  //reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonChartScreen::show(const String& fName)
{
    fileName = fName;
    serie.clear();
    InterruptTimeList lst;

    SdFile file;
    file.open(fileName.c_str(),FILE_READ);
    if(file.isOpen())
    {
      uint32_t curRec;
      while(1)
      {
        int readResult = file.read(&curRec,sizeof(curRec));
        if(readResult == -1 || size_t(readResult) < sizeof(curRec))
          break;
  
          lst.push_back(curRec);
      }
      file.close();
    }

    Drawing::ComputeChart(lst, serie);
    Screen.switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonChartScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonChartScreen::doDraw(TFTMenu* menu)
{
  Drawing::DrawChart(this,serie);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonChartScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen(listEthalonsFilesScreen); // переключаемся на экран работы со списком эталонов
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// EthalonRecordScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EthalonRecordScreen::EthalonRecordScreen() : AbstractTFTScreen("EthalonRecordScreen")
{
  state = recStarted;
  direction = dirUp;
  channel1Button = /*channel2Button = channel3Button =*/ channel1SaveButton = /*channel2SaveButton = channel3SaveButton =*/ directionButton -1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::doSetup(TFTMenu* menu)
{

  screenButtons->setSymbolFont(Various_Symbols_32x32);
  channel1SelectedChannel = /*channel2SelectedChannel = channel3SelectedChannel =*/ -1;
  channel1SaveChannel = /*channel2SaveChannel = channel3SaveChannel =*/ -1;
  currentDrawState = 0;

//  reserved = screenButtons->addButton(5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton( 5, 72, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
//  reserved = screenButtons->addButton(5, 107, BUTTON_WIDTH, BUTTON_HEIGHT, "reserved");
  backButton = screenButtons->addButton(5, 142, 100, BUTTON_HEIGHT, "ВЫХОД");
  saveButton = screenButtons->addButton(113, 142, 94, BUTTON_HEIGHT, "СОХР");

  uint16_t curX = 162;
  uint16_t curY = 20; 
  uint8_t boxSize = 20;
  uint8_t buttonSpacing = 4;

  channel1Button  = screenButtons->addButton(curX, curY, boxSize, boxSize, "-");
  screenButtons->setButtonBackColor(channel1Button,VGA_BLACK);
  screenButtons->setButtonFontColor(channel1Button,VGA_WHITE);

  channel1SaveButton = screenButtons->addButton(curX + boxSize + buttonSpacing*2, curY, boxSize, boxSize, "-");
  screenButtons->setButtonBackColor(channel1SaveButton,VGA_BLACK);
  screenButtons->setButtonFontColor(channel1SaveButton,VGA_WHITE);
  
  curY += boxSize + buttonSpacing;

  //DEPRECATED: channel2Button  = screenButtons->addButton(curX, curY, boxSize, boxSize, "-");
  //DEPRECATED: screenButtons->setButtonBackColor(channel2Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonFontColor(channel2Button,VGA_WHITE);

  //DEPRECATED: channel2SaveButton = screenButtons->addButton(curX + boxSize + buttonSpacing*2, curY, boxSize, boxSize, "-");
  //DEPRECATED: screenButtons->setButtonBackColor(channel2SaveButton,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonFontColor(channel2SaveButton,VGA_WHITE);
  
  //DEPRECATED: curY += boxSize + buttonSpacing;

  //DEPRECATED: channel3Button  = screenButtons->addButton(curX, curY, boxSize, boxSize, "-");
  //DEPRECATED: screenButtons->setButtonBackColor(channel3Button,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonFontColor(channel3Button,VGA_WHITE);

  //DEPRECATED: channel3SaveButton = screenButtons->addButton(curX + boxSize + buttonSpacing*2, curY, boxSize, boxSize, "-");
  //DEPRECATED: screenButtons->setButtonBackColor(channel3SaveButton,VGA_BLACK);
  //DEPRECATED: screenButtons->setButtonFontColor(channel3SaveButton,VGA_WHITE);

  //DEPRECATED: curY += boxSize + buttonSpacing;

  directionButton = screenButtons->addButton(curX, curY, boxSize*2 + buttonSpacing*2, 34, "c",BUTTON_SYMBOL);

  showButtons(false);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::resetButtons()
{
  int arr[] = {channel1Button, /*channel2Button, channel3Button,*/ channel1SaveButton, /*channel2SaveButton, channel3SaveButton,*/ -100};
  int cntr = 0;
  while(arr[cntr] != -100)
  {
    if(arr[cntr] > -1)
    {
      screenButtons->setButtonBackColor(arr[cntr],VGA_BLACK);
      screenButtons->setButtonFontColor(arr[cntr],VGA_WHITE);
      screenButtons->relabelButton(arr[cntr],"-");     
    }
    cntr++;
  }

  screenButtons->relabelButton(directionButton,"c");
  direction = dirUp;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
    drawState(menu);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::drawState(TFTMenu* menu)
{
  currentDrawState++;

  if(currentDrawState < 2)
    return;
  
  if(currentDrawState > 2)
  {
    currentDrawState = 2;
    return;
  }

  if(state == recStarted)
    drawWelcome(menu);
  else
  if(state == recDone)
  {
    Drawing::ComputeChart(list1, serie1);
    
    Screen.getDC()->fillScr(TFT_BACK_COLOR);
    showButtons(true);    
    
    Drawing::DrawChart(this, serie1, VGA_RED);
  }    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::doDraw(TFTMenu* menu)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::rotateSelectedChannel(int button, int& val)
{
  val++;
  
  if(val > 1)
     val = -1;

  word color = VGA_BLACK;
  word fontColor = VGA_WHITE;
  const char* capt = "-";
  switch(val)
  {
    case -1:
      color = VGA_BLACK;
      capt = "-";
    break;

    case 0:
      color = VGA_RED;
      capt = "1";
    break;
/*    
    case 1:
      color = VGA_BLUE;
      capt = "2";
    break;
    
    case 2:
      color = VGA_YELLOW;
      fontColor = VGA_BLACK;
      capt = "3";
    break;
*/
  }

  screenButtons->setButtonBackColor(button,color);
  screenButtons->setButtonFontColor(button,fontColor);
  screenButtons->relabelButton(button, capt,true);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::saveEthalon(int selChannel, int saveChannel)
{
  if(saveChannel == -1) // не выбрано, для какого канала сохранять
    return;

  InterruptTimeList fakeList;
  InterruptTimeList* selectedList = &fakeList;

  switch(selChannel)
  {
    case 0:
      selectedList = &list1;
    break;
    
	/*
	//DEPRECATED:
    case 1:
      selectedList = &list2;
    break;
    
    case 2:
      selectedList = &list3;
    break;
	*/

    default:
      selectedList = &fakeList;
    break;
  }

  SD.mkdir(ETHALONS_DIRECTORY);

  String fileName = ETHALONS_DIRECTORY;
  fileName += ETHALON_NAME_PREFIX;
  fileName += saveChannel;
  if(direction == dirUp)
    fileName += ETHALON_UP_POSTFIX;
  else
    fileName += ETHALON_DOWN_POSTFIX;

  fileName += ETHALON_FILE_EXT;

  DBG(F("WRITE ETHALON TO FILE "));
  DBGLN(fileName);

  SdFile file;
  file.open(fileName.c_str(),FILE_WRITE | O_CREAT | O_TRUNC);
  
  if(file.isOpen())
  {
    DBG(F("WRITE ETHALON DATA, RECORDS COUNT: "));
    DBGLN(selectedList->size());

    for(size_t i=0;i<selectedList->size();i++)
    {
      uint32_t val = (*selectedList)[i];
      file.write(&val,sizeof(val));
    }

  //  file.flush();
    file.close();
  }
 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::saveEthalons()
{
  DBGLN(F("SAVE ETHALONS!"));
  
  // тут сохранение эталонов. Если не выбран график, но выбран канал - создаём пустой файл. Если не выбран канал, но выбран график - ничего не делаем.
  // если выбрано и то, и то - пишем данные в файл.

  saveEthalon(channel1SelectedChannel,channel1SaveChannel);
  //DEPRECATED: saveEthalon(channel2SelectedChannel,channel2SaveChannel);
  //DEPRECATED: saveEthalon(channel3SelectedChannel,channel3SaveChannel);

  Vector<const char*> lines;
  lines.push_back("Эталоны");
  lines.push_back("сохранены.");    
  MessageBox->show(lines,"EthalonScreen");
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
  {
    menu->switchToScreen("EthalonScreen");
  }
  else if(pressedButton == channel1Button)
  {
    rotateSelectedChannel(pressedButton, channel1SelectedChannel);
  }
  /*
  //DEPRECATED:
  else if(pressedButton == channel2Button)
  {
    rotateSelectedChannel(pressedButton, channel2SelectedChannel);
  }
  else if(pressedButton == channel3Button)
  {
    rotateSelectedChannel(pressedButton, channel3SelectedChannel);
  }
  */
  else if(pressedButton == channel1SaveButton)
  {
    rotateSelectedChannel(pressedButton, channel1SaveChannel);
  }
  /*
  //DEPRECATED:
  else if(pressedButton == channel2SaveButton)
  {
    rotateSelectedChannel(pressedButton, channel2SaveChannel);
  }
  else if(pressedButton == channel3SaveButton)
  {
    rotateSelectedChannel(pressedButton, channel3SaveChannel);
  }
  */
  else if(pressedButton == directionButton)
  {
    if(direction == dirUp)
    {
      direction = dirDown;
      screenButtons->relabelButton(directionButton,"d",true);
    }
    else if(direction == dirDown)
    {
      direction = dirUp;
      screenButtons->relabelButton(directionButton,"c",true);
    }
  }
  else if(pressedButton == saveButton)
  {
    saveEthalons();
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::showButtons(bool bShow)
{
  if(bShow)
  {
    screenButtons->showButton(backButton,true);
    
    screenButtons->showButton(channel1Button,true);
	//DEPRECATED: screenButtons->showButton(channel2Button,true);
	//DEPRECATED: screenButtons->showButton(channel3Button,true);
    
    screenButtons->showButton(channel1SaveButton,true);
	//DEPRECATED: screenButtons->showButton(channel2SaveButton,true);
	//DEPRECATED: screenButtons->showButton(channel3SaveButton,true);
    
    screenButtons->showButton(directionButton,true);
    screenButtons->showButton(saveButton,true); 
  }
  else
  {
    screenButtons->hideButton(backButton);
    
    screenButtons->hideButton(channel1Button);
	//DEPRECATED: screenButtons->hideButton(channel2Button);
	//DEPRECATED: screenButtons->hideButton(channel3Button);

    screenButtons->hideButton(channel1SaveButton);
	//DEPRECATED: screenButtons->hideButton(channel2SaveButton);
	//DEPRECATED: screenButtons->hideButton(channel3SaveButton);

    screenButtons->hideButton(directionButton);
    screenButtons->hideButton(saveButton);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::drawWelcome(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  
  showButtons(false);
  
  dc->fillScr(TFT_BACK_COLOR);
  
  uint8_t* oldFont = dc->getFont();

  dc->setFont(SmallRusFont);

  Vector<const char*> lines;
  lines.push_back("Для записи эталона");
  lines.push_back("необходимо привести");
  lines.push_back("штангу в движение.");
  lines.push_back("");
  lines.push_back("Приведите штангу");
  lines.push_back("в движение и ждите");
  lines.push_back("результата записи.");
  
   int fontHeight = dc->getFontYsize();
   int fontWidth = dc->getFontXsize();
   int displayWidth = dc->getDisplayXSize();
   int lineSpacing = 2;  

   int curX = 0;
   int curY = 10;

    for(size_t i=0;i<lines.size();i++)
    {
      int lineLength = menu->print(lines[i],curX,curY,0,true);
      curX = (displayWidth - lineLength*fontWidth)/2;    
      menu->print(lines[i],curX,curY);
      curY += fontHeight + lineSpacing;
    }
  

  dc->setFont(oldFont);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::onActivate()
{
  state = recStarted;
  channel1SelectedChannel = /*channel2SelectedChannel = channel3SelectedChannel =*/ -1;
  channel1SaveChannel = /*channel2SaveChannel = channel3SaveChannel =*/ -1;
  currentDrawState = 0;
  resetButtons();
  showButtons(false);
  
  list1.clear();
  //DEPRECATED: list2.clear();
  //DEPRECATED: list3.clear();
  InterruptHandler.setSubscriber(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::onDeactivate()
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::OnHaveInterruptData()
{
  // сбрасываем подписчика
  InterruptHandler.setSubscriber(NULL);

  DBGLN(F("EthalonRecordScreen::OnHaveInterruptData"));

  // смотрим, в каком листе есть данные, и устанавливаем кнопки выбора графика на первый список с данными
  if (list1.size())
  {
	  channel1SelectedChannel = -1;
	  rotateSelectedChannel(channel1Button, channel1SelectedChannel);
  }

  /*
  //DEPRECATED: 
  InterruptTimeList* lists[] = {&list1, &list2, &list3};
 

  for(int i=0;i<3;i++)
  {
    if(lists[i]->size())
    {
      channel1SelectedChannel = i-1;
      channel2SelectedChannel = i-1;
      channel3SelectedChannel = i-1;

      rotateSelectedChannel(channel1Button,channel1SelectedChannel);
      rotateSelectedChannel(channel2Button,channel2SelectedChannel);
      rotateSelectedChannel(channel3Button,channel3SelectedChannel);
      
      break;
    }
  } // for
  */


  state = recDone;

  Drawing::ComputeChart(list1, serie1);
  
  Screen.getDC()->fillScr(TFT_BACK_COLOR);
  showButtons(true);  
  Drawing::DrawChart(this, serie1, VGA_RED);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EthalonRecordScreen::OnInterruptRaised(const InterruptTimeList& list, EthalonCompareResult compareResult)
{
  DBGLN(F("EthalonRecordScreen::OnInterruptRaised"));

  // пришли результаты серии прерываний с одного из списков.
  // мы запоминаем результаты в локальный список.
  list1 = list;

  /*
  //DEPRECATED:
  switch(listNum)
  {
    case 0:
    break;      
    case 1:
      list2 = list;
    break;      

    case 2:
      list3 = list;
    break;      
    
  } // switch
  */

  
  // для теста - печатаем в Serial
  #ifdef _DEBUG

    if(list.size() > 1)
    {
      DBGLN("INTERRUPT DATA >>");
      
      for(size_t i=0;i<list.size();i++)
      {
        DBGLN(list[i]);
      }
    }

    DBGLN("<< END OF INTERRUPT DATA");
    
  #endif // _DEBUG  
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FileEntry
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t FileEntry::getTimestamp(const char* fileRootDir)
{
  uint32_t result = 0;

  SdFile root, file;
  root.open(fileRootDir,O_READ);
  if(root.isOpen())
  {
    file.open(&root,dirIndex,O_READ);
    if(file.isOpen())
    {
      dir_t d;
      file.dirEntry(&d);
      result = (d.creationDate << 16) | d.creationTime;      
      file.close();
    }
    root.close();
  }

  return result;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String FileEntry::getName(const char* fileRootDir)
{
  String result;

  SdFile root, file;
  root.open(fileRootDir,O_READ);
  if(root.isOpen())
  {
    file.open(&root,dirIndex,O_READ);
    if(file.isOpen())
    {
      char nameBuff[50] = {0};
      file.getName(nameBuff,50);
      result = nameBuff;
      
      file.close();
    }
    root.close();
  }

  return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ListFilesScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ListFilesScreen* ListFilesScreen::create(ListFilesType vt)
{
  if(vt == vtLogsListing)
    return new ListFilesScreen(vt, "ListLogsScreen");  
  else if(vt == vtEthalonsListing)
    return new ListFilesScreen(vt, "ListEthalonsScreen");

  return NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ListFilesScreen::ListFilesScreen(ListFilesType vt, const char* name) : AbstractTFTScreen(name)
{
  viewType = vt;
  filesButtons = NULL;
  totalFilesCount = 0;
  totalPages = 0;
  currentPageNum = 0;
  isFirstScan = true;
  currentPageButton = -1;
  lastSelectedFileIndex = -1;
  files = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ListFilesScreen::~ListFilesScreen()
{
  delete filesButtons;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::clearFiles()
{
  if(!files)
    return;
    
  for(int i=0;i<totalFilesCount;i++)
    delete files[i];

  delete [] files;
  files = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::sortFiles()
{
  if(!files)
    return;

  //TODO: Тут сортировка файлов!!!  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::showPage(int step)
{
  if(!files || !filesButtons)
    return;

   currentPageNum += step;
   if(currentPageNum < 0)
    currentPageNum = 0;

   if(currentPageNum >= totalPages)
    currentPageNum = totalPages - 1;

    // тут отображение файлов
    int startIndex = currentPageNum*SCREEN_FILES_COUNT;
    int endIndex = startIndex + SCREEN_FILES_COUNT;
    if(endIndex >  totalFilesCount)
      endIndex = totalFilesCount;

    int buttonCounter = 0;
    
    const char* linkedDir = getDirName();
    
    for(int i=startIndex;i<endIndex;i++)
    {
        FileEntry* entry = files[i];
        filesNames[buttonCounter] = entry->getName(linkedDir);
        
        filesButtons->relabelButton(buttonCounter,filesNames[buttonCounter].c_str());
        filesButtons->showButton(buttonCounter,isActive());
        
        buttonCounter++;
    } // buttonCounter

    // форсим перерисовку скрытых кнопок
    for(int i=buttonCounter;i<SCREEN_FILES_COUNT;i++)
    {
      filesButtons->hideButton(i,isActive());
    }

    if(lastSelectedFileIndex != -1)
    {
      filesButtons->selectButton(lastSelectedFileIndex,false,true);
      lastSelectedFileIndex = -1;
    }

    screenButtons->disableButton(viewFileButton,true);
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* ListFilesScreen::getDirName()
{
   if(viewType == vtLogsListing)
     return LOGS_DIRECTORY;
  else if(viewType == vtEthalonsListing)
     return ETHALONS_DIRECTORY;  

  return "";
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::rescanFiles()
{
  if(!hasSD)
    return;

   int lastFilesCount = totalFilesCount;

   String dirName = getDirName();

   totalFilesCount = FileUtils::CountFiles(dirName);

   DBG(F("COUNT OF FILES IN \""));
   DBG(dirName);
   DBG(F("\" DIRECTORY: "));
   DBGLN(totalFilesCount);

   if(lastFilesCount != totalFilesCount)
   {
    clearFiles();

    files = new FileEntry*[totalFilesCount];

    SdFile file, root;
    
    for(int i=0;i<totalFilesCount;i++)
    {
      files[i] = new  FileEntry;
    } // for

    root.open(dirName.c_str(),O_READ);

    int cntr = 0;
    while (file.openNext(&root, O_READ)) 
    {
      if(cntr < totalFilesCount)
      {
        files[cntr]->dirIndex = file.dirIndex();
      }
      
      file.close();
      cntr++;
    }      
    root.close();

    sortFiles();
    
    // кол-во файлов изменилось!!!
    totalPages = totalFilesCount/SCREEN_FILES_COUNT;
    if(totalFilesCount % SCREEN_FILES_COUNT)
      totalPages++;

      if(isFirstScan)
      {
        isFirstScan = false;
        currentPageNum = totalPages-1;
      }
      
      if(filesButtons)
      {
        drawCurrentPageNumber();
      }
     
   } // if(lastFilesCount != totalFilesCount)
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::drawCurrentPageNumber()
{
  if(!isActive() || ! filesButtons || currentPageButton == -1)
    return;

    currentPageCaption = (currentPageNum + 1);
    if(!totalPages)
      currentPageCaption = '0';
      
    currentPageCaption += '/';
    currentPageCaption += totalPages;

    filesButtons->relabelButton(currentPageButton,currentPageCaption.c_str(),true);
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::doSetup(TFTMenu* menu)
{
  // инициализируем SD
  hasSD = SDInit::InitSD();

  screenButtons->setSymbolFont(Various_Symbols_32x32);
  
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");
  
  if(viewType == vtEthalonsListing)
    viewFileButton = screenButtons->addButton(128, 2, 90, 50, "|", BUTTON_SYMBOL);
  else if(viewType == vtLogsListing)
    viewFileButton = screenButtons->addButton(128, 2, 90, BUTTON_HEIGHT, "=>COM");

  UTFT* dc = menu->getDC();
  int screenWidth = dc->getDisplayXSize();

  if(hasSD)
  {
    filesButtons = new UTFT_Buttons_Rus(menu->getDC(),menu->getTouch(),menu->getRusPrinter());
    filesButtons->setButtonColors(VGA_WHITE,VGA_WHITE,VGA_WHITE,VGA_RED,VGA_BLACK);
    filesButtons->setTextFont(SmallRusFont);
  
    int filesButtonHeight = 20;
    int filesButtonWidth = 118;
    int curY = 2;
    int curX = 5;
  
    for(int i=0;i<SCREEN_FILES_COUNT;i++)
    {
      int addedButton = filesButtons->addButton(curX, curY,filesButtonWidth,filesButtonHeight,"");
      filesButtons->hideButton(addedButton);
      
      curY += filesButtonHeight + 2;
    }


    curY += 2;
    int pageButtonWigth = 80;
    int navigationButtonWidth = 50;
    int pagesButtonLeft = (screenWidth - pageButtonWigth)/2;

    currentPageButton = filesButtons->addButton(pagesButtonLeft, curY,pageButtonWigth,filesButtonHeight,"", BUTTON_DISABLED);

    prevPageButton = filesButtons->addButton(5, curY,navigationButtonWidth,filesButtonHeight,"<");
    nextPageButton = filesButtons->addButton(screenWidth - navigationButtonWidth - 5, curY,navigationButtonWidth,filesButtonHeight,">");
    
  } // hasSD
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::doUpdate(TFTMenu* menu)
{
  if(filesButtons)
  {
    int checkedFilesButton = filesButtons->checkButtons();

    if(checkedFilesButton != -1)
    {
      menu->notifyAction(this);
    }
    
    if(checkedFilesButton == prevPageButton)
    {
      showPage(-1);
      drawCurrentPageNumber();
    }
    else
    if(checkedFilesButton == nextPageButton)
    {
      showPage(1);
      drawCurrentPageNumber();
    }
    else if(checkedFilesButton == currentPageButton)
    {
      
    }
    else
    {
      if(checkedFilesButton > -1)
      {
        DBG(F("SELECTED FILE: "));
        DBGLN(filesNames[checkedFilesButton]);

        if(lastSelectedFileIndex != -1)
        {
          filesButtons->selectButton(lastSelectedFileIndex,false,true);
        }

        lastSelectedFileIndex = checkedFilesButton;
        filesButtons->selectButton(lastSelectedFileIndex,true,true);

        screenButtons->enableButton(viewFileButton,true);
      } // if
      
    }
    
  } // if(filesButtons)

  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);
  
  if(!hasSD)
  {
    // не удалось инициализировать!
    menu->print("Нет SD!", 10,10);
    dc->setFont(oldFont);
    return;
  }

  if(filesButtons)
  {
    filesButtons->drawButtons();
    drawCurrentPageNumber();
    
  } // if(filesButtons)

  showPage(0);

  dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListFilesScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
  {
    if(viewType == vtLogsListing)
      menu->switchToScreen("FilesScreen"); // переключаемся на экран "Файлы"
    else if(viewType == vtEthalonsListing)
      menu->switchToScreen("EthalonScreen"); // переключаемся на экран "Эталоны"
    
  }
  else
  if(pressedButton == viewFileButton)
  {
    if(lastSelectedFileIndex != -1)
    {
        DBG(F("VIEW FILE: "));
        DBGLN(filesNames[lastSelectedFileIndex]);
        
      // ПРОСМОТР ВЫБРАННОГО ФАЙЛА
      String fileName = getDirName();
      fileName += "/";
      fileName += filesNames[lastSelectedFileIndex];

        if(viewType == vtEthalonsListing)
        {
          ethalonChartScreen->show(fileName);
        }
        else if(viewType == vtLogsListing)
        {
          // отправляем содержимое файла в Serial
          FileUtils::SendToStream(Serial,fileName);
        }
      
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDInfoScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDInfoScreen::SDInfoScreen() : AbstractTFTScreen("SDInfoScreen")
{
  cardSize = 0;
  fatType = 0;
  freeSpace = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::doSetup(TFTMenu* menu)
{
  // инициализируем SD
  hasSD = SDInit::InitSD();
  
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

  // собираем информацию об SD
  collectSDInfo();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::doUpdate(TFTMenu* menu)
{
    // тут обновляем внутреннее состояние
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);
  
  if(!hasSD)
  {
    // не удалось инициализировать!
    menu->print("Нет SD!", 10,10);
    dc->setFont(oldFont);
    return;
  }

   int fontHeight = dc->getFontYsize();

   int curX = 10;
   int curY = 10;
   int lineSpacing = 2;
   
   String info = "Р-р: ";
   info += formatSize(cardSize);
   
   menu->print(info.c_str(), curX,curY);
   curY += fontHeight + lineSpacing;

   info = "Тип: FAT";
   info += fatType;

   menu->print(info.c_str(), curX,curY);
   curY += fontHeight + lineSpacing;

   info = "Своб.: ";
   info +=  formatSize(freeSpace);
   
   menu->print(info.c_str(), curX,curY);
   curY += fontHeight + lineSpacing;


   dc->setFont(oldFont);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("SDScreen"); // переключаемся на экран работы с SD
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInfoScreen::collectSDInfo()
{
  if(!hasSD)
    return;
    
  cardSize = SD.card()->cardSize();
  cardSize *= 512;
  
  fatType = SD.vol()->fatType();

  uint32_t volFree = SD.vol()->freeClusterCount();
  freeSpace = 512L*volFree*SD.vol()->blocksPerCluster();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String SDInfoScreen::formatSize(uint32_t sz)
{
   String result;

   if(sz < 1024L) // under 1 Kb
   {
      result = sz;
      result += "B";
      
   }
   else 
   if(sz < 1024L*1024L) // under 1 Mb
   {
      float f = (sz*1.0)/1024L;
      result += String(f,2);
      result += "Kb";
   }
   else 
   if(sz < 1024L*1024L*1024L) // under 1 Gb
   {
      float f = (sz*1.0)/(1024L*1024L);
      result += String(f,2);
      result += "Mb";
   }
   else // over 1 Gb
   {
      float f = (sz*1.0)/(1024L*1024L*1024L);
      result += String(f,2);
      result += "Gb";
   }

   return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFormatScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFormatScreen::SDFormatScreen() : AbstractTFTScreen("FormatSDScreen")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::doSetup(TFTMenu* menu)
{
  // инициализируем SD
  hasSD = SDInit::InitSD();

  if(hasSD)
  {
    formatButton = screenButtons->addButton( 5, 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Формат");
    clearFatButton = screenButtons->addButton(5, 37, BUTTON_WIDTH, BUTTON_HEIGHT, "Стереть всё");
  }
  
  backButton = screenButtons->addButton(5, 142, BUTTON_WIDTH, BUTTON_HEIGHT, "ВЫХОД");

  // добавляем экран форматирования
  menu->addScreen(SDFormatterScreen::create());  
  // добавляем экран очистки
  menu->addScreen(SDEraserScreen::create());  

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::doUpdate(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::doDraw(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);
  
  if(!hasSD)
  {
    // не удалось инициализировать!
    menu->print("Нет SD!", 10,10);
    dc->setFont(oldFont);
    return;
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
  if(pressedButton == backButton)
    menu->switchToScreen("SDScreen"); // переключаемся на экран работы с SD
  else
  if(pressedButton == formatButton)
  {
    Vector<const char*> lines;
    lines.push_back("Начать");
    lines.push_back("формат?");    
    MessageBox->confirm(lines,"SDFormatterScreen","FormatSDScreen");
  }
  else
  if(pressedButton == clearFatButton)
  {
    Vector<const char*> lines;
    lines.push_back("Начать");
    lines.push_back("очистку?");    
    MessageBox->confirm(lines,"SDEraserScreen","FormatSDScreen");
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFormatterScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFormatterScreen::SDFormatterScreen() : AbstractTFTScreen("SDFormatterScreen")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::doSetup(TFTMenu* menu)
{


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::doUpdate(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);

   int fontHeight = dc->getFontYsize();
   int fontWidth = dc->getFontXsize();
   int displayWidth = dc->getDisplayXSize();
   int displayHeight = dc->getDisplayYSize();
   int lineSpacing = 2;

   String message = "Идёт формат,";
   
   int lineLength = menu->print(message.c_str(),0,0,0,true);
   int curX = (displayWidth - lineLength*fontWidth)/2;
   int curY = (displayHeight - fontHeight*2 - lineSpacing)/2;

   menu->print(message.c_str(),curX,curY);  
   curY += fontHeight + lineSpacing;

   message = "ждите...";
   lineLength = menu->print(message.c_str(),0,0,0,true);
   curX = (displayWidth - lineLength*fontWidth)/2;
   menu->print(message.c_str(),curX,curY);  
     
  dc->setFont(oldFont);

    // форматируем карточку
    SDFormatter fmt;
    fmt.formatCard();

    Vector<const char*> lines;
    lines.push_back("Формат");
    lines.push_back("завершён.");    
    MessageBox->show(lines,"FormatSDScreen");


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::doDraw(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatterScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDEraserScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDEraserScreen::SDEraserScreen() : AbstractTFTScreen("SDEraserScreen")
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::doSetup(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::doUpdate(TFTMenu* menu)
{
  UTFT* dc = menu->getDC();
  uint8_t* oldFont = dc->getFont();

  dc->setFont(TFT_FONT);

   int fontHeight = dc->getFontYsize();
   int fontWidth = dc->getFontXsize();
   int displayWidth = dc->getDisplayXSize();
   int displayHeight = dc->getDisplayYSize();
   int lineSpacing = 2;

   String message = "Идёт очистка,";
   
   int lineLength = menu->print(message.c_str(),0,0,0,true);
   int curX = (displayWidth - lineLength*fontWidth)/2;
   int curY = (displayHeight - fontHeight*2 - lineSpacing)/2;

   menu->print(message.c_str(),curX,curY);  
   curY += fontHeight + lineSpacing;

   message = "ждите...";
   lineLength = menu->print(message.c_str(),0,0,0,true);
   curX = (displayWidth - lineLength*fontWidth)/2;
   menu->print(message.c_str(),curX,curY);  
     
  dc->setFont(oldFont);

    // форматируем карточку
    SDFormatter fmt;
    fmt.eraseCard();
    fmt.formatCard();

    Vector<const char*> lines;
    lines.push_back("Очистка");
    lines.push_back("завершена.");    
    MessageBox->show(lines,"FormatSDScreen");


}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::doDraw(TFTMenu* menu)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDEraserScreen::onButtonPressed(TFTMenu* menu, int pressedButton)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SDFormatter
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SDFormatter::SDFormatter()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::formatCard()
{
  if(initSizes())
  {  
    if(SD.card()->type() != SD_CARD_TYPE_SDHC)
      makeFat16();
    else
      makeFat32();    
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::sdError(const char* msg)
{
  DBG(F("SD ERROR: "));
  DBGLN(msg);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDFormatter::writeMbr()
{
  clearCache(true);
  part_t* p = cache.mbr.part;
  p->boot = 0;
  uint16_t c = lbnToCylinder(relSector);
  if (c > 1023) 
  {
    sdError("MBR CHS");
    return false;
  }
  p->beginCylinderHigh = c >> 8;
  p->beginCylinderLow = c & 0XFF;
  p->beginHead = lbnToHead(relSector);
  p->beginSector = lbnToSector(relSector);
  p->type = partType;
  uint32_t endLbn = relSector + partSize - 1;
  c = lbnToCylinder(endLbn);
  if (c <= 1023) 
  {
    p->endCylinderHigh = c >> 8;
    p->endCylinderLow = c & 0XFF;
    p->endHead = lbnToHead(endLbn);
    p->endSector = lbnToSector(endLbn);
  } 
  else 
  {
    // Too big flag, c = 1023, h = 254, s = 63
    p->endCylinderHigh = 3;
    p->endCylinderLow = 255;
    p->endHead = 254;
    p->endSector = 63;
  }
  p->firstSector = relSector;
  p->totalSectors = partSize;
  if (!writeCache(0)) 
  {
    sdError("write MBR");
    return false;
  }  

  return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SDFormatter::lbnToSector(uint32_t lbn) 
{
  return (lbn % sectorsPerTrack) + 1;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SDFormatter::lbnToHead(uint32_t lbn) 
{
  return (lbn % (numberOfHeads * sectorsPerTrack)) / sectorsPerTrack;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t SDFormatter::lbnToCylinder(uint32_t lbn) 
{
  return lbn / (numberOfHeads * sectorsPerTrack);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::clearCache(uint8_t addSig) 
{
  memset(&cache, 0, sizeof(cache));
  if (addSig) 
  {
    cache.mbr.mbrSig0 = BOOTSIG0;
    cache.mbr.mbrSig1 = BOOTSIG1;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::makeFat16()
{
  uint32_t nc;
  for (dataStart = 2 * BU16;; dataStart += BU16) 
  {
    nc = (cardSizeBlocks - dataStart)/sectorsPerCluster;
    fatSize = (nc + 2 + 255)/256;
    uint32_t r = BU16 + 1 + 2 * fatSize + 32;
    if (dataStart < r) 
    {
      continue;
    }
    relSector = dataStart - r + BU16;
    break;
  }
  // check valid cluster count for FAT16 volume
  if (nc < 4085 || nc >= 65525) 
  {
    sdError("Bad cluster count");
    return;
  }
  reservedSectors = 1;
  fatStart = relSector + reservedSectors;
  partSize = nc * sectorsPerCluster + 2 * fatSize + reservedSectors + 32;
  if (partSize < 32680) 
  {
    partType = 0X01;
  } 
  else if (partSize < 65536) 
  {
    partType = 0X04;
  } 
  else 
  {
    partType = 0X06;
  }
  
  // write MBR
  if(writeMbr())
  {
      clearCache(true);
      
      fat_boot_t* pb = &cache.fbs;
      pb->jump[0] = 0XEB;
      pb->jump[1] = 0X00;
      pb->jump[2] = 0X90;
      
      for (uint8_t i = 0; i < sizeof(pb->oemId); i++) 
      {
        pb->oemId[i] = ' ';
      }
      
      pb->bytesPerSector = 512;
      pb->sectorsPerCluster = sectorsPerCluster;
      pb->reservedSectorCount = reservedSectors;
      pb->fatCount = 2;
      pb->rootDirEntryCount = 512;
      pb->mediaType = 0XF8;
      pb->sectorsPerFat16 = fatSize;
      pb->sectorsPerTrack = sectorsPerTrack;
      pb->headCount = numberOfHeads;
      pb->hidddenSectors = relSector;
      pb->totalSectors32 = partSize;
      pb->driveNumber = 0X80;
      pb->bootSignature = EXTENDED_BOOT_SIG;
      pb->volumeSerialNumber = volSerialNumber();
      memcpy(pb->volumeLabel, "NO NAME    ", sizeof(pb->volumeLabel));
      memcpy(pb->fileSystemType, "FAT16   ", sizeof(pb->fileSystemType));
      
      // write partition boot sector
      if (!writeCache(relSector)) 
      {
        sdError("FAT16 write PBS failed");
        return;
      }
      // clear FAT and root directory
      if(clearFatDir(fatStart, dataStart - fatStart))
      {
        clearCache(false);
        cache.fat16[0] = 0XFFF8;
        cache.fat16[1] = 0XFFFF;
        // write first block of FAT and backup for reserved clusters
        if (!writeCache(fatStart)
            || !writeCache(fatStart + fatSize)) 
        {
          sdError("FAT16 reserve failed");
        }
      }
  } // if(writeMbr
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDFormatter::clearFatDir(uint32_t bgn, uint32_t count) 
{
  clearCache(false);

  if (!SD.card()->writeStart(bgn, count)) 
  {
    sdError("Clear FAT/DIR writeStart failed");
    return false;
  }
  for (uint32_t i = 0; i < count; i++) 
  {
    if ((i & 0XFF) == 0) 
    {
      //  cout << '.';
    }
    if (!SD.card()->writeData(cache.data)) 
    {
      sdError("Clear FAT/DIR writeData failed");
      return false;
    }
  }
  if (!SD.card()->writeStop()) 
  {
    sdError("Clear FAT/DIR writeStop failed");
    return false;
  }

  return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t SDFormatter::volSerialNumber() 
{
  return (cardSizeBlocks << 8) + micros();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t SDFormatter::writeCache(uint32_t lbn) 
{
  return SD.card()->writeBlock(lbn, cache.data);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::makeFat32()
{
  uint32_t nc;
  relSector = BU32;
  for (dataStart = 2 * BU32;; dataStart += BU32) 
  {
    nc = (cardSizeBlocks - dataStart)/sectorsPerCluster;
    fatSize = (nc + 2 + 127)/128;
    uint32_t r = relSector + 9 + 2 * fatSize;
    if (dataStart >= r) 
    {
      break;
    }
  }
  // error if too few clusters in FAT32 volume
  if (nc < 65525) 
  {
    sdError("Bad cluster count");
    return;
  }
  reservedSectors = dataStart - relSector - 2 * fatSize;
  fatStart = relSector + reservedSectors;
  partSize = nc * sectorsPerCluster + dataStart - relSector;
  // type depends on address of end sector
  // max CHS has lbn = 16450560 = 1024*255*63
  if ((relSector + partSize) <= 16450560) 
  {
    // FAT32
    partType = 0X0B;
  } 
  else 
  {
    // FAT32 with INT 13
    partType = 0X0C;
  }
  
  if(!writeMbr())
    return;
    
  clearCache(true);

  fat32_boot_t* pb = &cache.fbs32;
  pb->jump[0] = 0XEB;
  pb->jump[1] = 0X00;
  pb->jump[2] = 0X90;
  
  for (uint8_t i = 0; i < sizeof(pb->oemId); i++) 
  {
    pb->oemId[i] = ' ';
  }
  pb->bytesPerSector = 512;
  pb->sectorsPerCluster = sectorsPerCluster;
  pb->reservedSectorCount = reservedSectors;
  pb->fatCount = 2;
  pb->mediaType = 0XF8;
  pb->sectorsPerTrack = sectorsPerTrack;
  pb->headCount = numberOfHeads;
  pb->hidddenSectors = relSector;
  pb->totalSectors32 = partSize;
  pb->sectorsPerFat32 = fatSize;
  pb->fat32RootCluster = 2;
  pb->fat32FSInfo = 1;
  pb->fat32BackBootBlock = 6;
  pb->driveNumber = 0X80;
  pb->bootSignature = EXTENDED_BOOT_SIG;
  pb->volumeSerialNumber = volSerialNumber();
  memcpy(pb->volumeLabel, "NO NAME    ", sizeof(pb->volumeLabel));
  memcpy(pb->fileSystemType, "FAT32   ", sizeof(pb->fileSystemType));
  // write partition boot sector and backup
  if (!writeCache(relSector)
      || !writeCache(relSector + 6)) 
  {
    sdError("FAT32 write PBS failed");
    return;
  }
  clearCache(true);
  // write extra boot area and backup
  if (!writeCache(relSector + 2)
      || !writeCache(relSector + 8)) 
  {
    sdError("FAT32 PBS ext failed");
    return;
  }
  fat32_fsinfo_t* pf = &cache.fsinfo;
  pf->leadSignature = FSINFO_LEAD_SIG;
  pf->structSignature = FSINFO_STRUCT_SIG;
  pf->freeCount = 0XFFFFFFFF;
  pf->nextFree = 0XFFFFFFFF;
  // write FSINFO sector and backup
  if (!writeCache(relSector + 1)
      || !writeCache(relSector + 7)) 
  {
    sdError("FAT32 FSINFO failed");
    return;
  }
  
  if(!clearFatDir(fatStart, 2 * fatSize + sectorsPerCluster))
    return;
    
  clearCache(false);
  
  cache.fat32[0] = 0x0FFFFFF8;
  cache.fat32[1] = 0x0FFFFFFF;
  cache.fat32[2] = 0x0FFFFFFF;
  // write first block of FAT and backup for reserved clusters
  if (!writeCache(fatStart)
      || !writeCache(fatStart + fatSize)) 
  {
    sdError("FAT32 reserve failed");
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SDFormatter::initSizes()
{
  cardSizeBlocks = SD.card()->cardSize();
  cardCapacityMB = (cardSizeBlocks + 2047)/2048;  

 if (cardCapacityMB <= 6) 
 {
    sdError("Card is too small.");
    return false;
  } 
  else if (cardCapacityMB <= 16) 
  {
    sectorsPerCluster = 2;
  } 
  else if (cardCapacityMB <= 32) 
  {
    sectorsPerCluster = 4;
  } 
  else if (cardCapacityMB <= 64) 
  {
    sectorsPerCluster = 8;
  } 
  else if (cardCapacityMB <= 128) 
  {
    sectorsPerCluster = 16;
  } 
  else if (cardCapacityMB <= 1024) 
  {
    sectorsPerCluster = 32;
  } 
  else if (cardCapacityMB <= 32768) 
  {
    sectorsPerCluster = 64;
  } 
  else 
  {
    // SDXC cards
    sectorsPerCluster = 128;
  }

  // set fake disk geometry
  sectorsPerTrack = cardCapacityMB <= 256 ? 32 : 63;

  if (cardCapacityMB <= 16) 
  {
    numberOfHeads = 2;
  } 
  else if (cardCapacityMB <= 32) 
  {
    numberOfHeads = 4;
  } 
  else if (cardCapacityMB <= 128) 
  {
    numberOfHeads = 8;
  } 
  else if (cardCapacityMB <= 504) 
  {
    numberOfHeads = 16;
  } 
  else if (cardCapacityMB <= 1008) 
  {
    numberOfHeads = 32;
  } 
  else if (cardCapacityMB <= 2016) 
  {
    numberOfHeads = 64;
  } 
  else if (cardCapacityMB <= 4032) 
  {
    numberOfHeads = 128;
  } 
  else 
  {
    numberOfHeads = 255;
  }
  return true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFormatter::eraseCard()
{
  if(!initSizes())
    return;
    
  uint32_t const ERASE_SIZE = 262144L;
  uint32_t firstBlock = 0;
  uint32_t lastBlock;
 
  do 
  {
    lastBlock = firstBlock + ERASE_SIZE - 1;
    if (lastBlock >= cardSizeBlocks) 
    {
      lastBlock = cardSizeBlocks - 1;
    }
    if (!SD.card()->erase(firstBlock, lastBlock)) 
    {
      sdError("erase failed");
      return;
    }
    firstBlock += ERASE_SIZE;
  } while (firstBlock < cardSizeBlocks);

  if (!SD.card()->readBlock(0, cache.data)) 
  {
    sdError("readBlock");
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


