#include <WiFi.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <vector>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <FS.h>
#include <LittleFS.h>
#include <XPT2046_Touchscreen.h>


#include "Motors.h"
#include "Sensors.h"
#include "PathLogic.h"
#include "Display.h"
#include "ServerComm.h"
#include "Storage.h"

#define TOUCH_CS 21
#define TOUCH_IRQ 22

XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);


TFT_eSPI tft = TFT_eSPI();
HardwareSerial lidarSerial(1);
WiFiClient client;
std::vector<String> destinationNames;
bool isDriving = false;
String currentStatus = "idle";
float distanceTravelled = 0;
unsigned long lastStatusSent = 0;
unsigned long statusInterval = 3000;



// ----------- מסלול נסיעה מראש -----------

std::vector<Command> path = {
  { "forward", 5000 },
  { "right", 0 },
  { "forward", 2000 },
  { "stop", 0 }
};

void setup() {
  Serial.begin(115200);
  lidarSerial.begin(115200, SERIAL_8N1, 16, 17);
  tft.init(); tft.setRotation(1);
  LittleFS.begin();
  setupPins();
  connectToWiFi();
  connectToServer();
  loadDestinationNames(destinationNames);
  drawButtons(tft, destinationNames);
  executePath(path);
  std::vector<Command> returnPath = reversePath(path);
  executePath(returnPath);
}
void loop() {
}
// void loop() {
//   if (!isDriving) {
//     // כשהרכב לא נוסע - מטפל בבקשות חדשות מהטאץ' ומהשרת
//     handleTouch(tft, destinationNames);
//     receiveServerCommand();
//   }

//   // שליחת דיווח מצב קבוע לשרת (בכל מצב)
//   if (millis() - lastStatusSent > statusInterval) {
//     sendStatusToServer();
//     lastStatusSent = millis();
//   }
// }
