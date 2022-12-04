#include <string.h>

#include "NFCHandler.h"
#include "RTCHandler.h"
#include "CryptoHandler.h"

NFCHandler nfcHandler;
RTCHandler rtcHandler;
CryptoHandler* cryptoHandler;

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  noTone(4);
  
  rtcHandler.setupRTC();
  nfcHandler.setupNFC();

  // we need to download over http the hash

  cryptoHandler = new CryptoHandler(&rtcHandler);
  cryptoHandler->testTOTPFunction(strdup("RTKTCEXAJKRDRQAG7P62FO3RBKJQPU7INFHB7C2GDYPDB5WURPMQ"));
}

void loop() {
 Tag tag = nfcHandler.searchNFCCard();

 if (!tag.isSuccess) {
  // print err msg

    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);

    noTone(4);
  
    delay(2000);
    return; 
 } 

 //       /*
//         1. SEND DEVICE UID over -> get TOTP Code LIST
//         2. Compare CODES
      
//       */ 

 
  tag.printTagContent();
  rtcHandler.printTime();

  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);

  tone(4, 2000);

  delay(2000);
}
