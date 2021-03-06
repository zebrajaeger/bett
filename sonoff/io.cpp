#include "io.h"
#include "Arduino.h"

//------------------------------------------------------------------------------
Io::Io()
//------------------------------------------------------------------------------
  : pin_up(12)
  , pin_down(13)
  , pin_led(0)
  , ledValue(false)
{};

//------------------------------------------------------------------------------
void Io::setup()
//------------------------------------------------------------------------------
{
  pinMode(pin_up, OUTPUT);
  digitalWrite(pin_up, 0);
  pinMode(pin_down, OUTPUT);
  digitalWrite(pin_down, 0);
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led, 0);

  Serial.begin(115200);
  Serial.println("Booting");
}

//------------------------------------------------------------------------------
void Io::loop()
//------------------------------------------------------------------------------
{
  // nothing to do
}

//------------------------------------------------------------------------------
void Io::up()
//------------------------------------------------------------------------------
{
  digitalWrite(pin_up, 1);
}

//------------------------------------------------------------------------------
void Io::down()
//------------------------------------------------------------------------------
{
  digitalWrite(pin_down, 1);
}

//------------------------------------------------------------------------------
void Io::idle()
//------------------------------------------------------------------------------
{
  digitalWrite(pin_up, 0);
  digitalWrite(pin_down, 0);
}

//------------------------------------------------------------------------------
void Io::led(bool value)
//------------------------------------------------------------------------------
{
  if (value) {
    digitalWrite(pin_led, 0);
    pinMode(pin_led, INPUT);
  } else {
    pinMode(pin_led, OUTPUT);
    digitalWrite(pin_led, 0);
  }
  ledValue = value;
}

//------------------------------------------------------------------------------
void Io::ledToggle()
//------------------------------------------------------------------------------
{
  led(!ledValue);
}

