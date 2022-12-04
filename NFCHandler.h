#ifndef NFCHandler_h
#define NFCHandler_h

#include <DFRobot_PN532.h>
#include <Arduino.h>


struct Tag {
  char* deviceUID;
  char* totpCode;

  bool isSuccess;
  char* errMsg;

  void printTagContent() {
    Serial.print("Device UID: ");
    Serial.println(deviceUID); 

    Serial.print("TOTP Code: ");
    Serial.println(totpCode); 
  }
};

class NFCHandler {
private:
    const uint8_t BLOCK_SIZE = 16;
    const uint8_t PN532_IRQ = 2;
    const uint8_t INTERRUPT = 1;
    const uint8_t POLLING = 0;
    const uint8_t READ_BLOCK_NO = 4;

    DFRobot_PN532_IIC nfc = DFRobot_PN532_IIC(PN532_IRQ, POLLING);
    uint8_t dataRead[48] = {0};

public:
    void setupNFC();

    Tag searchNFCCard();
    
    void handleCardRead(uint8_t blockNumber, uint8_t offset);
    void handleCardReadFailure(uint8_t blockNumber);

    bool readBlock(uint8_t blockNumber, uint8_t* data, uint8_t offset);
};

#endif
