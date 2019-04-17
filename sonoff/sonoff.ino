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

#define TIMEOUT_TIME_MS 200
#define DIRSWITCH_DELAY_TIME_MS 500

// FSM
enum MOVE_STATE {
  MOVE_UP, AFTER_MOVE_UP, MOVE_DOWN, AFTER_MOVE_DOWN, MOVE_IDLE
};
MOVE_STATE fsmState = MOVE_IDLE;

//------------------------------------------------------------------------------
void onTimer() 
//------------------------------------------------------------------------------
{
  switch (fsmState) {
    case MOVE_UP:
      Serial.println("U_IDLE");
      fsmState = AFTER_MOVE_UP;
      io.idle();
      timer.start(DIRSWITCH_DELAY_TIME_MS);
      break;
    case AFTER_MOVE_UP:
      Serial.println("IDLE");
      fsmState = MOVE_IDLE;
      io.idle();
      break;
    case MOVE_DOWN:
      Serial.println("D-IDLE");
      fsmState = AFTER_MOVE_DOWN;
      io.idle();
      timer.start(DIRSWITCH_DELAY_TIME_MS);
      break;
    case AFTER_MOVE_DOWN:
      Serial.println("IDLE");
      fsmState = MOVE_IDLE;
      io.idle();
      break;
  }
}

//------------------------------------------------------------------------------
void onUp() 
//------------------------------------------------------------------------------
{
  switch (fsmState) {
    case AFTER_MOVE_UP:
    case MOVE_IDLE:
      Serial.println("UP");
    case MOVE_UP:
      timer.start(TIMEOUT_TIME_MS);
      fsmState = MOVE_UP;
      io.up();
      break;
  }
}

//------------------------------------------------------------------------------
void onDown() 
//------------------------------------------------------------------------------
{
  switch (fsmState) {
    case AFTER_MOVE_DOWN:
    case MOVE_IDLE:
      Serial.println("DOWN");
    case MOVE_DOWN:
      timer.start(TIMEOUT_TIME_MS);
      fsmState = MOVE_DOWN;
      io.down();
      break;
  }
}

//------------------------------------------------------------------------------
void onStop() 
//------------------------------------------------------------------------------
{
  switch (fsmState) {
    case MOVE_UP:
      fsmState = AFTER_MOVE_UP;
      io.idle();
      timer.start(DIRSWITCH_DELAY_TIME_MS);
      break;

    case MOVE_DOWN:
      fsmState = AFTER_MOVE_DOWN;
      io.idle();
      timer.start(DIRSWITCH_DELAY_TIME_MS);
      break;
  }
}

//------------------------------------------------------------------------------
void setup() 
//------------------------------------------------------------------------------
{
  io.setup();
  timer.setup(onTimer);
  wifi.setup();
  ota.setup(&io);
  webserver.setup(onUp, onDown, onStop);
  io.led(1);
}

//------------------------------------------------------------------------------
void loop() 
//------------------------------------------------------------------------------
{
  io.loop();
  timer.loop();
  wifi.loop();
  ota.loop();
  webserver.loop();
}
