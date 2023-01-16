#ifndef ESPWiFi_h
#define ESPWiFi_h

#include <WiFiEsp.h>

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(19, 18); // RX, TX
#endif

enum class RESPONSE_STATUS {
  OK = 0,
  UNATHORIZE = 1,
  ERROR = 2
};

struct CodeResponse {
  RESPONSE_STATUS status;
  String authorizationKey;
};

class ESPWiFi {
private:
    
    WiFiEspClient client;
    
    CodeResponse readResponse();

public:
    void setupWiFi();

    void sendSimpleHttpGetRequest(String deviceId);

    void readHeader();

    String readLine();
    String readBody();

    CodeResponse executeRequest(String deviceId);
};

#endif
