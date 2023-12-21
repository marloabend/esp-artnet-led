#include "setup_ota.h"

extern AsyncWebServer server;
extern Config config;
extern int packetCounter;

bool defaultConfig() {
  config.universe = 1;
  config.offset = 0;
  config.pixels = 12;
  config.colorMapping = "RGB";
  config.white = false;
  config.brightness = 255;
  config.hsv = false;
  config.mode = 1;
  config.reverse = false;
  config.speed = 8;
  config.split = 1;
  return true;
}

bool loadConfig() {
  Serial.println("loadConfig");

  File configFile = LittleFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  StaticJsonDocument<300> jsonBuffer;
  deserializeJson(jsonBuffer, configFile);
  configFile.close();
  
  JsonObject root = jsonBuffer.as<JsonObject>();

  // if (!root.success()) {
  //   Serial.println("Failed to parse config file");
  //   return false;
  // }

  config.universe = root["universe"];
  config.offset = root["offset"];
  config.pixels = root["pixels"];
  config.colorMapping = root["colorMapping"].as<String>();
  config.white = root["white"];
  config.brightness = root["brightness"];
  config.hsv = root["hsv"];
  config.mode = root["mode"];
  config.reverse = root["reverse"];
  config.speed = root["speed"];
  config.split = root["split"];

  return true;
}

bool saveConfig() {
  Serial.println("saveConfig");
  StaticJsonDocument<300> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();

  root["universe"] = config.universe;
  root["offset"] = config.offset;
  root["pixels"] = config.pixels;
  root["colorMapping"] = config.colorMapping;
  root["white"] = config.white;
  root["brightness"] = config.brightness;
  root["hsv"] = config.hsv;
  root["mode"] = config.mode;
  root["reverse"] = config.reverse;
  root["speed"] = config.speed;
  root["split"] = config.split;

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  else {
    Serial.println("Writing to config file");
    if (serializeJson(jsonBuffer, configFile) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    configFile.close();
    return true;
  }
}

/*
void handleUpdate1() {
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  ESP.restart();
}

void handleUpdate2() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    Serial.printf("Update: %s\n", upload.filename.c_str());
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace)) { //start with max available size
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  }
  yield();
}
*/

void handleDirList(AsyncWebServerRequest *request) {
  Serial.println("handleDirList");
  String str = "";
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " ";
    str += dir.fileSize();
    str += " bytes\r\n";
  }
  request->send(200, "text/plain", str);
}
