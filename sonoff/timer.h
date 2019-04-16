#ifndef _timer_h_
#define _timer_h_

class Timer {
  public:
    Timer() : active(false) {}

    void setup(void(*cb)()) {
      callback = cb;
      active = false;
    }

    void loop() {
      if (active && millis() >= timeMs) {
        active = false;
        callback();
      }
    }

    void start(unsigned long durationMs) {
      timeMs = millis() + durationMs;
      active = true;
    }

  private:
    bool active;
    unsigned long timeMs;
    void(*callback)();
};

#endif //_timer_h_
