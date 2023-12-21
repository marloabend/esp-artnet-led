#include "main.h"

Config config;
AsyncWebServer server(80);
DNSServer dns;

const char* host = "Art-Net LED";
const char* password = "12345678";
const char* version = __DATE__ " / " __TIME__;

float fps = 0;

// Neopixel settings
Adafruit_NeoPixel strip = Adafruit_NeoPixel();

// Artnet settings
ArtnetWifi artnet;
unsigned int packetCounter = 0;

// Global universe buffer
struct {
  uint16_t universe;
  uint16_t length;
  uint8_t sequence;
  uint8_t *data;
} global;

// Use an array of function pointers to jump to the desired mode
void (*mode[]) (uint16_t, uint16_t, uint8_t, uint8_t *) {
  mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15
};

// Keep the timing of the function calls
long tic_loop = 0;
long tic_fps = 0;
long tic_packet = 0;
long tic_web = 0;
long frameCounter = 0;

// This will be called for each UDP packet received
void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {  
  packetCounter++;

  // Copy the data from the UDP packet over to the global universe buffer
  global.universe = universe;
  global.sequence = sequence;
  if (length < 512)
    global.length = length;
  for (int i = 0; i < global.length; i++)
    global.data[i] = data[i];
}

void updateNeopixelStrip() {
  strip.updateType(strip.str2order(config.colorMapping.c_str()) + NEO_KHZ800);
  strip.updateLength(config.pixels);
  strip.setBrightness(config.brightness);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println();

  global.universe = 0;
  global.sequence = 0;
  global.length = 512;
  global.data = (uint8_t *)malloc(512);

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  strip.setPin(LED_PIN);
  defaultConfig();
  updateNeopixelStrip();
  strip.begin();

  if (loadConfig()) {
    updateNeopixelStrip();
    singleYellow();
  } else {
    singleRed();
  }

  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect(host, password);
  Serial.println("connected");

  if (WiFi.status() == WL_CONNECTED)
    singleGreen();

  // Webserver
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->redirect("/");
  });

  server.on("/discovery", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    StaticJsonDocument<300> data;
    data["chipId"] = ESP.getFlashChipId();
    data["version"] = version;
    data["mac"] = WiFi.macAddress();
    data["ip"] = WiFi.localIP();

    String json;
    serializeJson(data, json);

    request->send(200, "application/json", json);
  });

  server.on("/defaults", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    Serial.println("GET /defaults");
    request->send(200, "text/plain", "");
    delay(2000);
    singleRed();
    defaultConfig();
    saveConfig();
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.resetSettings();
    ESP.restart();
  });

  server.on("/reconnect", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    Serial.println("GET /reconnect");
    request->send(200, "text/plain", "");
    delay(2000);
    singleYellow();
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.startConfigPortal(host, password);
    Serial.println("connected");
    if (WiFi.status() == WL_CONNECTED) {
      singleGreen();
    }
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    Serial.println("handleReset");
    request->send(200, "text/plain", "");
    delay(2000);
    singleRed();
    ESP.restart();
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP_GET /monitor");
    tic_web = millis();
    StaticJsonDocument<300> data;
    data["uptime"]  = long(millis() / 1000);
    data["packets"] = packetCounter;
    data["fps"]     = fps;
    data["heap"]    = ESP.getFreeHeap();

    String json;
    serializeJson(data, json);

    request->send(200, "application/json", json);
  });

  server.on("/dir", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    Serial.println("handleDirList");
    handleDirList(request);
  });

  server.on("/channel", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<300> data;
    data["universe"] = config.universe;
    data["offset"] = config.offset;
    
    String json;
    serializeJson(data, json);

    request->send(200, "application/json", json);
  });

  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<300> data;
    data["mode"] = config.mode;
    
    String json;
    serializeJson(data, json);

    request->send(200, "application/json", json);
  });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP_GET /config");
    tic_web = millis();
    StaticJsonDocument<300> data;
    data["pixels"] = config.pixels;
    data["colorMapping"] = config.colorMapping;
    data["white"] = config.white;
    data["brightness"] = config.brightness;
    data["hsv"] = config.hsv;
    data["reverse"] = config.reverse;
    data["speed"] = config.speed;
    data["split"] = config.split;

    String json;
    serializeJson(data, json);

    request->send(200, "application/json", json);
  });

  AsyncCallbackJsonWebHandler* channelHandler = new AsyncCallbackJsonWebHandler("/channel", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<300> data;
    data = json.as<JsonObject>();
    JsonObject jsonObj = json.as<JsonObject>();

    config.universe = jsonObj["universe"];
    config.offset = jsonObj["offset"];

    if (saveConfig() && loadConfig()) {
      updateNeopixelStrip();
    }

    String serializedJson;
    serializeJson(jsonObj, serializedJson);

    request->send(200, "application/json", serializedJson);
  });
  server.addHandler(channelHandler);

  AsyncCallbackJsonWebHandler* modeHandler = new AsyncCallbackJsonWebHandler("/mode", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<300> data;
    data = json.as<JsonObject>();
    JsonObject jsonObj = json.as<JsonObject>();

    config.mode = jsonObj["mode"];

    if (saveConfig() && loadConfig()) {
      updateNeopixelStrip();
    }

    String serializedJson;
    serializeJson(jsonObj, serializedJson);

    request->send(200, "application/json", serializedJson);
  });
  server.addHandler(modeHandler);

  AsyncCallbackJsonWebHandler* configHandler = new AsyncCallbackJsonWebHandler("/config", [](AsyncWebServerRequest *request, JsonVariant &json) {
    Serial.println("HTTP_POST /config");
    tic_web = millis();
    handleJSON(request, json);
  });
  server.addHandler(configHandler);

  // server.on("/update", HTTP_POST, handleUpdate1, handleUpdate2);

  server.begin();

  // Announce the hostname and web server through zeroconf
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);

  artnet.begin();
  artnet.setArtDmxCallback(onDmxPacket);

  // Initialize all timers
  tic_loop   = millis();
  tic_packet = millis();
  tic_fps    = millis();
  tic_web    = 0;

  Serial.println("setup done");
  singleGreen();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    singleRed();
  } else if (WiFi.status() == WL_CONNECTED) {
    artnet.read();
    
    // This section gets executed at a maximum rate of around 1Hz
    if ((millis() - tic_loop) >= 1000) {
      updateNeopixelStrip();
    }

    // This section gets executed at a maximum rate of around 100Hz (10/1000ms)
    if ((millis() - tic_loop) >= 10) {
      if (config.mode >= 0 && config.mode < sizeof(mode) / 4) {
        // Call the function corresponding to the current mode
        (*mode[config.mode]) (global.universe, global.length, global.sequence, global.data);
      }
      
      tic_loop = millis();
      frameCounter++;
    }

    if ((millis() - tic_fps) >= 1000 && frameCounter >= 100) {
      fps = 1000 * frameCounter / (millis() - tic_fps);
      frameCounter = 0;
      tic_fps = millis();
    }
  }
  delay(1);
}
