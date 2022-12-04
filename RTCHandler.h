#ifndef RTCHandler_h
#define RTCHandler_h

#include <Arduino.h>
#include "RTClib.h"

class RTCHandler {
private:
    RTC_DS1307 rtc;
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
public:
    void setupRTC();
    void printTime();

    unsigned long getTimestamp();
};

#endif
