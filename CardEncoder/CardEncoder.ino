#include "NFCHandler.h"
#include "RTCHandler.h"
#include "CryptoHandler.h"
#include <Time.h>
#include <LiquidCrystal.h>

const int rs = 6, rw = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs,rw, en, d4, d5, d6, d7);

NFCHandler nfcHandler;
RTCHandler rtcHandler;
CryptoHandler* cryptoHandler;

#define  WRITE_BLOCK_NO      4
#define  BLOCK_SIZE          16

void printLCDMessage(String message, int line) {
  if (message.length() < 16) {
    for (int i = message.length(); i < 16; i++) {
      message += ' ';
    }
  }

  Serial.println(message);

  lcd.setCursor(0, line);
  lcd.print(message);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  lcd.begin(16, 2);

  nfcHandler.setupNFC();
  rtcHandler.setupRTC();
  
  String time = rtcHandler.printTime();
  
  printLCDMessage("Printing Time:", 0);
  printLCDMessage(time, 1);

  delay(2000);

  cryptoHandler = new CryptoHandler(&rtcHandler);
}

void loop() {
 

  if (nfcHandler.nfc.scan()) {

      printLCDMessage("Card inserted", 0);
      printLCDMessage("Please wait ...", 1);

      unsigned long timestamp = rtcHandler.getTimestamp();
      String codeTOTP = cryptoHandler->generateTOTPCode("A73JIH7IRLDT2NB6VZ4OMGP2YMLKYPAKED3DEPJZO6NGKZO6BSCQ0", timestamp);

      String content = "4B06B2A8-20E9-404D-B99D-BEC94B22B60D&" + codeTOTP;

      int contentLength = content.length();
      char dataToWrite[48];

      for (int i = 0; i < contentLength; i++) {
        dataToWrite[i] = content.charAt(i);
      }

      for (int i = contentLength; i < 48; i++) {
        dataToWrite[i] = 0;
      }
      

      Serial.println("TOTP Code: ");
      Serial.println(content);

    if (nfcHandler.writeToBlock(WRITE_BLOCK_NO,dataToWrite, 0) && nfcHandler.writeToBlock(WRITE_BLOCK_NO + 1, dataToWrite, BLOCK_SIZE) && 
        nfcHandler.writeToBlock(WRITE_BLOCK_NO + 2, dataToWrite, 2 * BLOCK_SIZE)) {
          printLCDMessage("Success", 0);
          printLCDMessage("Remove Card ...", 1);
        }
  } else {
    printLCDMessage("Insert card ...", 0);
    printLCDMessage("", 1);
  }
  delay(5000);
}
