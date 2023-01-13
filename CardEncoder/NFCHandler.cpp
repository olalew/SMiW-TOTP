#include "NFCHandler.h"

void NFCHandler::setupNFC() {
   while (!nfc.begin()) {
     Serial.print(".");
     delay (1000);
   }
   Serial.println();
   Serial.println("Waiting for a card......");  
}

Tag NFCHandler::searchNFCCard() {
     if (nfc.scan()) {
       bool success = readBlock(READ_BLOCK_NO, dataRead, 0);
       success &= readBlock(READ_BLOCK_NO + 1, dataRead, BLOCK_SIZE);
       success &= readBlock(READ_BLOCK_NO + 2, dataRead, BLOCK_SIZE * 2);

       if (!success) {
         Tag resultTag;
         resultTag.isSuccess = false;
         resultTag.errMsg = "Wystąpił błąd.";
         return resultTag;
       }

       struct Tag tag;
       tag.deviceUID = String(strtok((char*)dataRead, "&"));
       tag.totpCode = String(strtok(NULL, "&"));
       tag.isSuccess = true;
       tag.errMsg = "";

       return tag;
   }

    Tag resultTag;
    resultTag.isSuccess = false;
    resultTag.errMsg = "Nie przyłożono karty.";
    return resultTag;
}
    
void NFCHandler::handleCardRead(uint8_t blockNumber, uint8_t offset) {
     Serial.print("Block ");
     Serial.print(blockNumber);
     Serial.println(" read success!");
      
     Serial.print("Data read(string):");
     Serial.println((char *)dataRead + offset); 

     Serial.print("Data read(HEX):");
     for (int i = 0; i < BLOCK_SIZE; i++) {
       Serial.print(dataRead[i + offset], HEX);
       Serial.print(" ");
     }
     Serial.println();
}

void NFCHandler::handleCardReadFailure(uint8_t blockNumber) {
     Serial.print("Block ");
     Serial.print(blockNumber);
     Serial.println(" read failure!");
}

bool NFCHandler::readBlock(uint8_t blockNumber, uint8_t* data, uint8_t offset) {
     if (nfc.readData(data + offset, blockNumber) != 1) {
         handleCardReadFailure(blockNumber);
         return false;
     }
  
     handleCardRead(blockNumber, offset);
     return true;
}

bool NFCHandler::writeToBlock(uint8_t blockNumber, uint8_t* data, uint8_t offset) {
  if (nfc.writeData(blockNumber, data + offset) != 1) {
      Serial.print("Block ");
      Serial.print(blockNumber);
      Serial.println(" write failure!");
  }
    else {
      Serial.print("Block ");
      Serial.print(blockNumber);
      Serial.println(" write success!");
      Serial.print("Data written(string):");
      Serial.println((char *)data + offset);
      Serial.print("Data written(HEX):");
      for (int i = 0; i < BLOCK_SIZE; i++) {
        Serial.print(data[offset + i], HEX);
        Serial.print(" ");
      }
  }
}