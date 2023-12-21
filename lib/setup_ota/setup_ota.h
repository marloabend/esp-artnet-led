#ifndef _SETUP_OTA_H_
#define _SETUP_OTA_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <LittleFS.h>

#define JSON_TO_CONFIG(x, y)   { if (root.containsKey(y)) { config.x = root[y]; } }
#define CONFIG_TO_JSON(x, y)   { root.set(y, config.x); }
#define KEYVAL_TO_CONFIG(x, y, request) { if (request->hasArg(y))    { String str = request->arg(y); config.x = str.toInt(); } }

bool initialConfig(void);
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
  int leds;
  int white;
  int brightness;
  int hsv;
  int mode;
  int reverse;
  int speed;
  int split;
};

#endif // _SETUP_OTA_H_
