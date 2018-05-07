#pragma once
//--------------------------------------------------------------------------------------------------
#include <Arduino.h>
//--------------------------------------------------------------------------------------------------
class FeedbackList
{
  public:
    FeedbackList();
    void begin();

    void readyDiode(bool on=true);
    void failureDiode(bool on=true);
    void testDiode(bool on=true);

    void alarm(bool on=true);

  private:
};
//--------------------------------------------------------------------------------------------------
extern FeedbackList Feedback;
//--------------------------------------------------------------------------------------------------

