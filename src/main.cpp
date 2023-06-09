#include <Arduino.h>
#include <ArduinoOTA.h>

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

// #include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
// #include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <FS.h>
#include <LittleFS.h>

#include <ezTime.h>
#include "logger.h"

#define FIRMWARE_VERSION "0.1"

// +++++++++++++++++++

const char COMPILE_INFO[] PROGMEM = {__DATE__ " " __TIME__ " - v" FIRMWARE_VERSION};

// +++++++++++++++++++

#define DEFAULT_NODE_NAME "F42-IONODE"
#define MQTT_SERVER "mqtt.thomo.de"
#define MY_NTP_SERVER "ntp.thomo.de"

// +++++++++++++++++++

#define I2C_SDA_PIN  4
#define I2C_SCL_PIN  5

// const String CONFIG_HTML = "/config.html";
// const String CONFIG_FILE = "/config.cfg";

#define MYDEBUG 0
#define debug_print(line) \
            do { if (MYDEBUG) Serial.print(line); } while (0)
#define debug_println(line) \
            do { if (MYDEBUG) Serial.println(line); } while (0)
#define debug_printf(frmt, ...) \
            do { if (MYDEBUG) Serial.printf(frmt, __VA_ARGS__); } while (0)

// --- Network ---
WiFiManager wifiManager;

// WiFiClient espClient;


// Helper 
Timezone myTZ;
Logger myLog = Logger(myTZ);

void initOTA() {
  // ArduinoOTA.setPort(8266);
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
      myLog.info(F("OTA Start updating program code"));
    } else {
      // U_FS
      type = "filesystem";
      LittleFS.end();
      myLog.info(F("OTA Start updating filesystem"));
    }
  });

  ArduinoOTA.onEnd([]() {
    myLog.info(F("OTA Finished."));
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
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

  ArduinoOTA.begin(false);
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.println();
  myLog.infof("Firmware Version: %s", COMPILE_INFO);
  myLog.info("Setup start");

  initOTA();

  myLog.info("Setup finished");
}

void loop() { 
  ArduinoOTA.handle();
  Serial.print(".");
  delay(500);
}
