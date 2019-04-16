# Bett/SonOff

Hardware based on SonOff (ESP8266 or ESP8285)

Open Arduino IDE

## Prepare Arduino IDE for ESP82xx

* Follow the instructions (Installing with Boards Manager) on https://github.com/esp8266/Arduino#user-content-installing-with-boards-manager

Open Tools >> Board: >> Boardmanager and install "esp8266 by ESP8266 Community" in version 2.4.2 (Version is important cause of bugs in webserver in v2.5.0!)

* Set Board to "Generic ESP8285 Module" (os ESP8266)
* CPU Frequency to 160MHz
* Flash Size: 1M (512K SPIFFS)

## Prepare Arduino IDE for SPIFFS upload

* Follow the instructions on https://github.com/esp8266/arduino-esp8266fs-plugin

## Build
* Compile and Upload

## FS

* Execute the install task on Bett/site to create the required SPIFFS files
* Execute Arduino IDE >> Tools >> ESP8266 Sketch Data Upload
