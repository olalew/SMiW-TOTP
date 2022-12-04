#include "CryptoHandler.h"

void printHash(uint8_t* hash) {
  int i;
  for (i=0; i<20; i++) {
    Serial.print("0123456789abcdef"[hash[i]>>4]);
    Serial.print("0123456789abcdef"[hash[i]&0xf]);
  }
  Serial.println();
}

void CryptoHandler::testHashingFunction() {
  Serial.println("Test: FIPS 180-2 C.1 and RFC3174 7.3 TEST1");
  Serial.println("Expect:a9993e364706816aba3e25717850c26c9cd0d89d");
  Serial.print("Result:");

  Sha1.init();
  Sha1.print("abc");
  printHash(Sha1.result());
}

void CryptoHandler::testTOTPFunction(char* _key) {

  // decode key from base 32 and use as hmacKey
  
  uint8_t hmacKey[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
  TOTP totp = TOTP(hmacKey, 10, 60); // 10 -> length of the key | 60 -> timestep

  unsigned long secondstime = rtcHandler->getTimestamp();
  Serial.println(secondstime);
  char* newCode = totp.getCode(secondstime);
  Serial.println(newCode);
}
