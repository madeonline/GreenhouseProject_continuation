//--------------------------------------------------------------------------------------------------
#include "Feedback.h"
#include "CONFIG.h"
//--------------------------------------------------------------------------------------------------
FeedbackList Feedback;
//--------------------------------------------------------------------------------------------------
FeedbackList::FeedbackList()
{
  
}
//--------------------------------------------------------------------------------------------------
void FeedbackList::begin()
{
  pinMode(LED_READY, OUTPUT);
  pinMode(LED_FAILURE, OUTPUT);
  pinMode(LED_TEST, OUTPUT);

  pinMode(STATUS_LINE,OUTPUT);
  digitalWrite(STATUS_LINE, LOW);
  alarm(false);

  readyDiode(false);
  failureDiode(false);
  testDiode(false);
}
//--------------------------------------------------------------------------------------------------
void FeedbackList::alarm(bool on)
{
  digitalWrite(STATUS_LINE, on ? STATUS_ALARM_LEVEL : !STATUS_ALARM_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void FeedbackList::readyDiode(bool on)
{
  digitalWrite(LED_READY, on ? LED_ON_LEVEL : !LED_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void FeedbackList::failureDiode(bool on)
{
  digitalWrite(LED_FAILURE, on ? LED_ON_LEVEL : !LED_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------
void FeedbackList::testDiode(bool on)
{
  digitalWrite(LED_TEST, on ? LED_ON_LEVEL : !LED_ON_LEVEL);
}
//--------------------------------------------------------------------------------------------------

