#include "main.h"

Config config;
AsyncWebServer server(80);
DNSServer dns;

const char* host = "ARTNET";
const char* password = "12345678";
const char* version = __DATE__ " / " __TIME__;
float temperature = 0, fps = 0;

// Neopixel settings
const byte dataPin = 2;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, dataPin, NEO_GRBW + NEO_KHZ800); // start with one pixel

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

// use an array of function pointers to jump to the desired mode
void (*mode[]) (uint16_t, uint16_t, uint8_t, uint8_t *) {
  mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15, mode16
};

// keep the timing of the function calls
long tic_loop = 0, tic_fps = 0, tic_packet = 0, tic_web = 0;
long frameCounter = 0;

//this will be called for each UDP packet received
void onDmxPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t * data) {
  // print some feedback
  Serial.print("packetCounter = ");
  Serial.print(packetCounter++);
  if ((millis() - tic_fps) > 1000 && frameCounter > 100) {
    // don't estimate the FPS too quickly
    fps = 1000 * frameCounter / (millis() - tic_fps);
    tic_fps = millis();
    frameCounter = 0;
    Serial.print(",  FPS = ");
    Serial.print(fps);
  }
  Serial.println();

  // copy the data from the UDP packet over to the global universe buffer
  global.universe = universe;
  global.sequence = sequence;
  if (length < 512)
    global.length = length;
  for (int i = 0; i < global.length; i++)
    global.data[i] = data[i];
} // onDmxpacket

void updateNeopixelStrip(void) {
  // update the neopixel strip configuration
  strip.updateLength(config.pixels);
  strip.setBrightness(config.brightness);
  if (config.leds == 3)
    strip.updateType(NEO_GRB + NEO_KHZ800);
  else if (config.leds == 4 && config.white)
    strip.updateType(NEO_GRBW + NEO_KHZ800);
  else if (config.leds == 4 && !config.white)
    strip.updateType(NEO_GRBW + NEO_KHZ800);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("setup starting");

  global.universe = 0;
  global.sequence = 0;
  global.length = 512;
  global.data = (uint8_t *)malloc(512);

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  strip.begin();
  initialConfig();

  if (loadConfig()) {
    updateNeopixelStrip();
    strip.setBrightness(255);
    singleYellow();
    delay(1000);
  }
  else {
    updateNeopixelStrip();
    strip.setBrightness(255);
    singleRed();
    delay(1000);
  }

  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect(host, password);
  Serial.println("connected");

  if (WiFi.status() == WL_CONNECTED)
    singleGreen();

  // this serves all URIs that can be resolved to a file on the LittleFS filesystem
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html");
  });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

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
    Serial.println("handleDefaults");
    request->send(200, "text/plain", "");
    delay(2000);
    singleRed();
    initialConfig();
    saveConfig();
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.resetSettings();
    ESP.restart();
  });

  server.on("/reconnect", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    Serial.println("handleReconnect");
    request->send(200, "text/plain", "");
    delay(2000);
    singleYellow();
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.startConfigPortal(host, password);
    Serial.println("connected");
    if (WiFi.status() == WL_CONNECTED)
      singleGreen();
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    tic_web = millis();
    Serial.println("handleReset");
    request->send(200, "text/plain", "");
    delay(2000);
    singleRed();
    ESP.restart();
  });

  server.on("/monitor", HTTP_GET, [](AsyncWebServerRequest *request) {
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

  AsyncCallbackJsonWebHandler* jsonHandler = new AsyncCallbackJsonWebHandler("/json", [](AsyncWebServerRequest *request, JsonVariant &json) {
    Serial.println("HTTP_POST /json");
    tic_web = millis();
    handleJSON(request, json);
  });
  server.addHandler(jsonHandler);

  server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP_GET /json");
    tic_web = millis();
    StaticJsonDocument<300> data;
    data["universe"] = config.universe;
    data["offset"] = config.offset;
    data["pixels"] = config.pixels;
    data["leds"] = config.leds;
    data["white"] = config.white;
    data["brightness"] = config.brightness;
    data["hsv"] = config.hsv;
    data["mode"] = config.mode;
    data["reverse"] = config.reverse;
    data["speed"] = config.speed;
    data["split"] = config.split;
    String str;
    serializeJson(data, str);
    request->send(200, "application/json", str);
  });

  // TODO: server.on("/update", HTTP_POST, handleUpdate1, handleUpdate2);

  // start the web server
  server.begin();

  // announce the hostname and web server through zeroconf
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);

  artnet.begin();
  artnet.setArtDmxCallback(onDmxPacket);

  // initialize all timers
  tic_loop   = millis();
  tic_packet = millis();
  tic_fps    = millis();
  tic_web    = 0;

  Serial.println("setup done");
} // setup

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    singleRed();
  }
  else if ((millis() - tic_web) < 5000) {
    singleBlue();
  }
  else  {
    artnet.read();

    // this section gets executed at a maximum rate of around 1Hz
    if ((millis() - tic_loop) > 999)
      updateNeopixelStrip();

    // this section gets executed at a maximum rate of around 100Hz
    if ((millis() - tic_loop) > 9) {
      if (config.mode >= 0 && config.mode < (sizeof(mode) / 4)) {
        // call the function corresponding to the current mode
        (*mode[config.mode]) (global.universe, global.length, global.sequence, global.data);
        tic_loop = millis();
        frameCounter++;
      }
    }
  }

  delay(1);
} // loop
