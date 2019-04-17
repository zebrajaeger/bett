#ifndef _webserver_h_
#define _webserver_h_

#include <ESP8266WebServer.h>
#include <FS.h>
#include <functional>

class Webserver {
  public:
    Webserver();
    void setup(void(*cbUp)(), void(*vbDown)(), void(*cbStop)());
    void loop();

  private:
    ESP8266WebServer server;
    void(*callbackUp)();
    void(*callbackDown)();
    void(*callbackStop)();

    void handleUp();
    void handleDown();
    void handleStop();

    void handleNotFound();

    void send404();
    bool sendFromSPIFF(String path);
};

#endif // _webserver_h_
