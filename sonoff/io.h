#ifndef _io_h_
#define _io_h_

class Io {
  public:
    Io();
    void setup();
    void loop();
    void up();
    void down();
    void idle();
    void led(bool value);
    void ledToggle();

  private:
    int pin_up;
    int pin_down;
    int pin_led;
    bool ledValue;
};

#endif //_io_h_
