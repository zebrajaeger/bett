#include "ota.h"

//------------------------------------------------------------------------------
Ota::Ota()
//------------------------------------------------------------------------------
  : io(NULL)
{}

//------------------------------------------------------------------------------
void Ota::setup(Io* pIo)
//------------------------------------------------------------------------------
{
  io = pIo;

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {
      //SPIFFS.end();
      type = "filesystem";
    }

    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([&]() {
    if (Ota::io) {
      Ota::io->led(0);
    }
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
    if (Ota::io) {
      Ota::io->ledToggle();
    }
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

//------------------------------------------------------------------------------
void Ota::loop()
//------------------------------------------------------------------------------
{
  ArduinoOTA.handle();
}
