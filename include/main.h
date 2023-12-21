/*
  This sketch receives a DMX universe via Artnet to control a 
  strip of ws2811 leds via Adafruit's NeoPixel library:

  https://github.com/rstephan/ArtnetWifi
  https://github.com/adafruit/Adafruit_NeoPixel
*/

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <WiFiClient.h>
#include <ArtnetWifi.h>
#include <Adafruit_NeoPixel.h>
#include <LittleFS.h>

#include "setup_ota.h"
#include "neopixel_mode.h"
