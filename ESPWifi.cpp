#include "ESPWiFi.h"

void ESPWiFi::setupWiFi() {
    char ssid[] = "Galaxy";
    char pass[] = "lgcu0355";
    // char ip[] = "192.168.121.94";

    Serial.println("BEFORE INITIALIZATION !!!");

    WiFi.init(&Serial1);

    Serial.println("AFTER INITIALIZATION !!!");
 
    Serial.print("Connecting to ");
    Serial.println(ssid);
 
    int status = WL_IDLE_STATUS;
 
    //Aguarda conectar à rede WiFi
    while (status != WL_CONNECTED)
    {
        status = WiFi.begin(ssid, pass);
    }
 
    Serial.println(status);
    Serial.println("Conectado");
 
    //Configura o IP
    // IPAddress ipAddress;
    // ipAddress.fromString(ip);
    // WiFi.config(ipAddress);
 
    //Veririca o IP
    IPAddress localIP = WiFi.localIP();
    Serial.print("IP: ");
    Serial.println(localIP);
}

void ESPWiFi::sendSimpleHttpGetRequest(String deviceId) {
    char server[] = "polisa-new-test.voyager.pl";

  Serial.println();
  Serial.println("Starting connection to server... 443");

   client.setTimeout(20000);
   if (client.connectSSL(server, 443)) {
      Serial.println("Connected to server");
      
      client.println("GET https://polisa-new-test.voyager.pl/api/Lock/GetLockAuthKey?deviceId=" + deviceId + "&lockAuthenticationKey=D3DEPJZO6NGKZO6BSCQ4OMGP2YMLKYDT2NB6VZPAKEJIH7IRLA73 HTTP/1.1");
      client.println("Host: polisa-new-test.voyager.pl");
      client.println("Accept: */*");
      client.println("Accept-Encoding: identity");
      client.println("Connection: close");
      client.println();

      if (client.connected()) {
        Serial.println("Still connected 443");
      } else {
        Serial.println("Not Connetcted");
      }
  }
}

CodeResponse ESPWiFi::readResponse() {
  CodeResponse response;

  Serial.println("HEADER -------------------");
  String httpStatus = readLine();

  if (httpStatus.indexOf("200") > 0) {
    response.status = RESPONSE_STATUS::OK;
  } else if(httpStatus.indexOf("401") > 0 || httpStatus.indexOf("403") > 0) {
    response.status = RESPONSE_STATUS::UNATHORIZE;
  } else {
    response.status = RESPONSE_STATUS::ERROR;
  }

  Serial.println(httpStatus);
  Serial.println("HEADER -------------------");
  readHeader();

  Serial.println("BODY -------------------");
  String body = readBody();
  response.authorizationKey = body.substring(2);
  response.authorizationKey.trim();

  Serial.println(body);

  Serial.println("DISCONNECTING -------------------");
  client.stop();

  return response;
}

String ESPWiFi::readLine() {
  String line = "";

  char prevCharacter;
  while(client.connected() || client.available()) {
      if (client.available()) {
          char c = client.read();

          prevCharacter = c;

          if (c == '\n') {
            // end of header
            // Serial.println(line);
            return line;
          }

          if (c == '\r')
            continue;
          
          line.concat(c);
      }
  }
}

void ESPWiFi::readHeader() {
   while(client.connected() || client.available()) {
      if (client.available()) {
          String line = readLine();
          if (line.length() == 0) {
            return;
          }
          line.trim();
          Serial.println(line);
      }
  }
}

String ESPWiFi::readBody() {
  String body = "";
  while(client.connected() || client.available()) {
      if (client.available()) {
          String line = readLine();
          line.trim();
          body.concat(line);
      }
  }
  body.trim();
  return body;
}

CodeResponse ESPWiFi::executeRequest(String deviceId) {
    CodeResponse response;
    int times = 0;
    do {
      Serial.println("SENDING REQUETS ---------------------------------");

      sendSimpleHttpGetRequest(deviceId);
      response = readResponse();

      if (response.status == RESPONSE_STATUS::ERROR) {
        delay(1000);
      }

      times++;
    } while(response.status == RESPONSE_STATUS::ERROR && times < 5);

    if (response.status == RESPONSE_STATUS::ERROR) {
      Serial.println("Ouć something went wrong ...");
    } else if (response.status == RESPONSE_STATUS::UNATHORIZE) {
      Serial.println("OHHHH invalid device ...");
    } else if (response.status == RESPONSE_STATUS::OK) {
      Serial.println("PARSING QR CODE ...");
      Serial.println(response.authorizationKey);
      Serial.println("----------------- END OF KEY -----------------");
    }

    return response;
}