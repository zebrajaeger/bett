#ifndef _ota_h_
#define _ota_h_

#include <ArduinoOTA.h>
#include "io.h"

class Ota {
  public:
    Ota();
    void setup(Io* pIo);
    void loop();

  private:
    Io *io;
};

#endif //_ota_h_
