#include "wifi.h"

//------------------------------------------------------------------------------
Wifi::Wifi()
//------------------------------------------------------------------------------
{};

//------------------------------------------------------------------------------
void Wifi::setup()
//------------------------------------------------------------------------------
{
  WiFi.hostname(WIFI_DEVICENAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//------------------------------------------------------------------------------
void Wifi::loop()
//------------------------------------------------------------------------------
{
  // nothing to do
}
