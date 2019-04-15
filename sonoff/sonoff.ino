#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <FS.h>

#ifndef STASSID
#define STASSID "..."
#define STAPSK  "..."
#endif

const char DEVICE_NAME[] = "Bett";

const char* ssid = STASSID;
const char* password = STAPSK;
const char* index_html = "/index.html";

ESP8266WebServer server(80);
int relais = 12;
int led1 = 13;
int led2 = 0;


unsigned long stopTime = 125; //ms

enum MOVE_STATE {
  MOVE_UP, MOVE_DOWN, MOVE_IDLE
};

MOVE_STATE moveState = MOVE_IDLE;
unsigned long lastCallTime = 0;
bool ledState = 0;

void led_toggle() {
  ledState = !ledState;
  led_set(ledState);
}

void led_set(boolean state) {
  ledState = state;
  digitalWrite(led1, !ledState);
}

void handleRoot() {
  if (SPIFFS.exists(index_html)) {
    File file = SPIFFS.open(index_html, "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close();
  } else {
    server.send(200, "text/html", "no index.htm in spifss");
  }
}

void handleUp() {
  moveState = MOVE_UP;
  lastCallTime = millis();
  server.send(200, "text/plain", "");

  /*
    Serial.println("u(1)");
    led_set(1);
    lastCallTime = millis();
    moveState = MOVE_UP;
    server.send(200, "text/plain", "");
    led_set(0);
    Serial.println("u(2)");
  */
}

void handleDown() {
  lastCallTime = millis();
  moveState = MOVE_DOWN;
  server.send(200, "text/plain", "");

  /*
    Serial.println("d(1)");
    led_set(1);
    lastCallTime = millis();
    moveState = MOVE_DOWN;
    server.send(200, "text/plain", "");
    led_set(0);
    Serial.println("d(2)");
  */
}


void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  pinMode(led1, OUTPUT);
  digitalWrite(led1, 0);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, 0);
  pinMode(relais, OUTPUT);
  digitalWrite(relais, 0);

  SPIFFS.begin();

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print(dir.fileName());
    Serial.print(" ");
    if (dir.fileSize()) {
      File f = dir.openFile("r");
      Serial.println(f.size());
    }
  }

  // WIFI stuff
  WiFi.hostname(DEVICE_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // OTA
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {
      //SPIFFS.end();
      type = "filesystem";
    }

    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    led_set(0);
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    led_toggle();
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Webserver
  server.on("/", handleRoot);
  server.on("/u", handleUp);
  server.on("/d", handleDown);
  server.begin();
  Serial.println("HTTP server started");
}

bool x1;
bool x2;
unsigned long blink1 = millis();
unsigned long blink2 = millis();
void loop() {
  unsigned long now = millis();

  if (now - blink1 > 250) {
    x1 = !x1;
    digitalWrite(led2, x1);
    blink1 = now;
  }

  if (now - blink2 > 70) {
    x2 = !x2;
    digitalWrite(relais, x2);
    blink2 = now;
  }

  ArduinoOTA.handle();
  server.handleClient();


  switch (moveState) {
    case MOVE_UP:
      if (now - lastCallTime > stopTime) {
        moveState = MOVE_IDLE;
      } else {
        // TODO MOVE UP
        led_set(1);
      }
      break;

    case MOVE_DOWN:
      if (now - lastCallTime > stopTime) {
        moveState = MOVE_IDLE;
      } else {
        // TODO  MOVE_DOWN
        led_set(1);
      }
      break;

    default:
      // TODO STOP
      led_set(0);
      break;
  }
}
