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

void printHexBytes(ParsedKey key) {
  char HEX_ARRAY[] = "0123456789ABCDEF";
	for (int j = 0; j < key.keyLength; j++) {
	    int v = key.hmacKey[j] & 0xFF;
	    Serial.print(HEX_ARRAY[(unsigned int)v >> 4]);
	    Serial.print(HEX_ARRAY[v & 0x0F]);
	}
}

void CryptoHandler::testTOTPFunction(String _key) {

  // decode key from base 32 and use as hmacKey
  Base32 base32Parser;
  ParsedKey parsedKey = base32Parser.fromBase32String(_key);

  Serial.println("BYTES -------------------------------");
  printHexBytes(parsedKey);
  Serial.println();
  Serial.println("BYTES END -------------------------------");

  TOTP totp = TOTP(parsedKey.hmacKey, parsedKey.keyLength, 60 * 5);

  unsigned long secondstime = rtcHandler->getTimestamp();
  Serial.println(secondstime);
  char* newCode = totp.getCode(secondstime);
  Serial.println(newCode);
}

String CryptoHandler::generateTOTPCode(String _key, unsigned long timestamp) {

  // decode key from base 32 and use as hmacKey
  Base32 base32Parser;
  ParsedKey parsedKey = base32Parser.fromBase32String(_key);

  Serial.println("BYTES -------------------------------");
  printHexBytes(parsedKey);
  Serial.println();
  Serial.println("BYTES END -------------------------------");

  TOTP totp = TOTP(parsedKey.hmacKey, parsedKey.keyLength, 60 * 10);
  char* newCode = totp.getCode(timestamp);
  Serial.println(newCode);

  return String(newCode);
}
