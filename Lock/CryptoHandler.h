#ifndef CryptoHandler_h
#define CryptoHandler_h

#include "sha1.h"
#include "TOTP.h"
#include "Base32.h"
#include <Arduino.h>
#include "RTCHandler.h"

class CryptoHandler {
private:
  RTCHandler* rtcHandler;
  Base32 base32;
  
public:
  CryptoHandler(RTCHandler* _rtcHandler): rtcHandler(_rtcHandler)  {}

  void testHashingFunction();
  void testTOTPFunction(String _key);

  String generateTOTPCode(String _key, unsigned long timestamp);
};

#endif
