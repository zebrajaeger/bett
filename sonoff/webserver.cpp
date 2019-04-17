#include "webserver.h"

//------------------------------------------------------------------------------
Webserver::Webserver()
//------------------------------------------------------------------------------
  : server(80)
  , callbackUp(NULL)
  , callbackDown(NULL)
  , callbackStop(NULL)
{
}

//------------------------------------------------------------------------------
void Webserver::setup(void(*cbUp)(), void(*cbDown)(), void(*cbStop)())
//------------------------------------------------------------------------------
{
  callbackUp = cbUp;
  callbackDown = cbDown;
  callbackStop = cbStop;

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

  //server.on("/", std::bind(&Webserver::handleRoot, this));
  server.on("/u", std::bind(&Webserver::handleUp, this));
  server.on("/d", std::bind(&Webserver::handleDown, this));
  server.on("/s", std::bind(&Webserver::handleStop, this));
  server.onNotFound(std::bind(&Webserver::handleNotFound, this));

  server.begin();
  Serial.println("HTTP server started");
}

//------------------------------------------------------------------------------
void Webserver::loop()
//------------------------------------------------------------------------------
{
  server.handleClient();
}

//------------------------------------------------------------------------------
void Webserver::handleUp()
//------------------------------------------------------------------------------
{
  if (callbackUp) callbackUp();
  server.send(200, "text/plain", "");
}

//------------------------------------------------------------------------------
void Webserver::handleDown()
//------------------------------------------------------------------------------
{
  if (callbackDown) callbackDown();
  server.send(200, "text/plain", "");
}

//------------------------------------------------------------------------------
void Webserver::handleStop()
//------------------------------------------------------------------------------
{
  if (callbackStop) callbackStop();
  server.send(200, "text/plain", "");
}

//------------------------------------------------------------------------------
void Webserver::handleNotFound()
//------------------------------------------------------------------------------
{
  if (!sendFromSPIFF(server.uri())) {
    send404();
  }
}

//------------------------------------------------------------------------------
void Webserver::send404()
//------------------------------------------------------------------------------
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//------------------------------------------------------------------------------
bool Webserver::sendFromSPIFF(String path)
//------------------------------------------------------------------------------
{
  String dataType = "text/plain";
  if (path.endsWith("/")) {
    path += "index.html";
  }

  Serial.print("Request File: ");
  Serial.println(path);

  if (path.endsWith(".src")) {
    path = path.substring(0, path.lastIndexOf("."));
  } else if (path.endsWith(".htm")) {
    dataType = "text/html";
  } else if (path.endsWith(".css")) {
    dataType = "text/css";
  } else if (path.endsWith(".js")) {
    dataType = "application/javascript";
  } else if (path.endsWith(".png")) {
    dataType = "image/png";
  } else if (path.endsWith(".gif")) {
    dataType = "image/gif";
  } else if (path.endsWith(".jpg")) {
    dataType = "image/jpeg";
  } else if (path.endsWith(".ico")) {
    dataType = "image/x-icon";
  } else if (path.endsWith(".xml")) {
    dataType = "text/xml";
  } else if (path.endsWith(".pdf")) {
    dataType = "application/pdf";
  } else if (path.endsWith(".zip")) {
    dataType = "application/zip";
  }

  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close();
    return true;
  } else {
    return false;
  }
}
