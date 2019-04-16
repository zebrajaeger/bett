#ifndef _webserver_h_
#define _webserver_h_

#include <ESP8266WebServer.h>
#include <FS.h>
#include <functional>

class Webserver {
  public:
    Webserver();
    void setup(void(*up)(), void(*down)());
    void loop();

  private:
    ESP8266WebServer server;
    void(*callbackUp)();
    void(*callbackDown)();

    void handleUp();
    void handleDown();
    void handleNotFound();

    void send404();
    bool sendFromSPIFF(String path);
};

#endif // _webserver_h_
