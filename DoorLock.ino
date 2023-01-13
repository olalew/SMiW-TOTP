#include <string.h>

#include "NFCHandler.h"
#include "RTCHandler.h"
#include "CryptoHandler.h"
#include "ESPWifi.h"
#include <Time.h>
#include <LiquidCrystal.h>

const int rs = 6, rw = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs,rw, en, d4, d5, d6, d7);

NFCHandler nfcHandler;
RTCHandler rtcHandler;
CryptoHandler* cryptoHandler;

ESPWiFi wifiHandler;

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
  printLCDMessage("Starting ...", 0);

  // rtcHandler.setupRTC();
  // rtcHandler.rtc.adjust(DateTime(2023, 1, 13, 21, 26, 10));

  delay(1000);

  printLCDMessage("Setting up WiFi", 0);

  wifiHandler.setupWiFi();

  // wifiHandler.printWifiData();
  // wifiHandler.printCurrentNet();

  printLCDMessage("Setting up NFC", 0);

  nfcHandler.setupNFC();

  printLCDMessage("Setting up RTC", 0);
  
  rtcHandler.setupRTC();

  // rtcHandler.rtc.adjust(DateTime(2023, 1, 12, 19, 24, 0));

  String time = rtcHandler.printTime();
  
  printLCDMessage("Printing Time:", 0);
  printLCDMessage(time, 1);

  delay(1000);

  Serial.println("Starting ....");

  cryptoHandler = new CryptoHandler(&rtcHandler);
}

void loop() {
  Tag tag = nfcHandler.searchNFCCard();

  if (!tag.isSuccess) {
    printLCDMessage("Insert card ...", 0);
    printLCDMessage("", 1);

    delay(2000);
    return; 
  } 

  // display tag found - verification of qrcode please wait

  tag.printTagContent();
  // rtcHandler.printTime();

  printLCDMessage("Card inserted ..", 0);
  printLCDMessage("Validating creds", 1);

  unsigned long timestamp = rtcHandler.getTimestamp();
  CodeResponse response = wifiHandler.executeRequest(tag.deviceUID);

  if (response.status == RESPONSE_STATUS::UNATHORIZE) {
    printLCDMessage("Invalid card", 0);
    printLCDMessage("Unathorized", 1);

    displayError("Invalid Card");
    delay(5000);
    return;
  } else if (response.status == RESPONSE_STATUS::ERROR) {
    printLCDMessage("Error", 0);
    printLCDMessage("Unexpected Error", 1);

    displayError("Unexpected Error");
    delay(5000);
    return;
  }

  String codeTOTP = cryptoHandler->generateTOTPCode(response.authorizationKey, timestamp);

  // and compare now
  if (codeTOTP == tag.totpCode) {
    displaySuccess();
    delay(10000);
  } else {
    displayError("Invalid Code");
    delay(5000);
  }
}

void displaySuccess() {
    printLCDMessage("Success", 0);
    printLCDMessage("Lock is open ...", 1);
}

void displayError(String error) {
    printLCDMessage("Error", 0);
    printLCDMessage(error, 1);
}
