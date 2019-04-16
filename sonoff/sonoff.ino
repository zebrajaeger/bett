#include "wifi.h"
#include "timer.h"
#include "webserver.h"
#include "ota.h"
#include "io.h"

Wifi wifi;
Timer timer;
Webserver webserver;
Ota ota;
Io io;

// FSM
enum MOVE_STATE {
  MOVE_UP, AFTER_MOVE_UP, MOVE_DOWN, AFTER_MOVE_DOWN, MOVE_IDLE
};
MOVE_STATE fsmState = MOVE_IDLE;

void onTimer() {
  switch (fsmState) {
    case MOVE_UP:
      Serial.println("I");
      fsmState = AFTER_MOVE_UP;
      io.idle();
      timer.start(500);
      break;
    case AFTER_MOVE_UP:
      Serial.println("I");
      fsmState = MOVE_IDLE;
      io.idle();
      break;
    case MOVE_DOWN:
      Serial.println("I");
      fsmState = AFTER_MOVE_DOWN;
      io.idle();
      timer.start(500);
      break;
    case AFTER_MOVE_DOWN:
      Serial.println("I");
      fsmState = MOVE_IDLE;
      io.idle();
      break;
  }
}

void onUp() {
  switch (fsmState) {
    case AFTER_MOVE_UP:
    case MOVE_IDLE:
      Serial.println("U");
    case MOVE_UP:
      timer.start(100);
      fsmState = MOVE_UP;
      io.up();
      break;
  }
}

void onDown() {
  switch (fsmState) {
    case AFTER_MOVE_DOWN:
    case MOVE_IDLE:
      Serial.println("D");
    case MOVE_DOWN:
      timer.start(100);
      fsmState = MOVE_DOWN;
      io.down();
      break;
  }
}

void setup() {
  io.setup();
  timer.setup(onTimer);
  wifi.setup();
  ota.setup();
  webserver.setup(onUp, onDown);
}

void loop() {
  io.loop();
  timer.loop();
  wifi.loop();
  ota.loop();
  webserver.loop();
}
