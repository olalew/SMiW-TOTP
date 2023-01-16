#ifndef RTCHandler_h
#define RTCHandler_h

#include <Arduino.h>
#include "RTClib.h"

class RTCHandler {
private:
    
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
public:
    RTC_DS1307 rtc;

    void setupRTC();
    String printTime();

    unsigned long getTimestamp();
};

#endif
