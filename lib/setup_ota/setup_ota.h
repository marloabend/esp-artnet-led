#ifndef _SETUP_OTA_H_
#define _SETUP_OTA_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <LittleFS.h>

#define KEYVAL_TO_CONFIG(x, y, request) { if (request->hasArg(y))    { String str = request->arg(y); config.x = str.toInt(); } }

bool defaultConfig(void);
bool loadConfig(void);
bool saveConfig(void);

// void handleUpdate1(void);
// void handleUpdate2(void);
void handleDirList(AsyncWebServerRequest *request);
void handleJSON(AsyncWebServerRequest *request, JsonVariant &json);

struct Config {
  int universe;
  int offset;
  int pixels;
  String colorMapping;
  bool white; // deprecated
  int brightness;
  bool hsv;
  int mode;
  bool reverse;
  int speed;
  int split;
};

#endif // _SETUP_OTA_H_
