#ifndef _wifi_h_
#define _wifi_h_

#include "config.h"

#include <ESP8266WiFi.h>

#ifndef WIFI_DEVICENAME
#error "WIFI_DEVICENAME undefined (use config.h)"
#endif

#ifndef WIFI_SSID
#error "WIFI_SSID undefined (use config.h)"
#endif

#ifndef WIFI_PASSWORD
#error "WIFI_PASSWORD undefined (use config.h)"
#endif

class Wifi {
  public:
    Wifi();
    void setup();
    void loop();
};

#endif // _wifi_h_
