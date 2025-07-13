#pragma once

#include "Storage.h"
#include "Sensors.h"
#include "PathLogic.h"

extern WiFiClient client;
extern String currentStatus;
extern float distanceTravelled;
extern bool isDriving;
extern std::vector<Command> path;

const char* ssid = "GALGALIM1";
const char* password = "741741741";
const char* serverIP = "192.168.33.23";
const uint16_t serverPort = 8888;

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi מחובר");
}

void connectToServer() {
  if (!client.connect(serverIP, serverPort))
    Serial.println("חיבור לשרת נכשל");
}

void sendStatusToServer() {
  if (!client.connected()) return;
  String status = "{";
  status += "\"id\":\"robot1\",";
  status += "\"status\":\"" + currentStatus + "\",";
  status += "\"distanceTravelled\":" + String(distanceTravelled) + ",";
  status += "\"lidarFront\":" + String(readLiDAR()) + ",";
  status += "\"ultrasonicFront\":" + String(readUltrasonic(trigFront, echoFront)) + ",";
  status += "\"ultrasonicLeft\":" + String(readUltrasonic(trigLeft, echoLeft)) + ",";
  status += "\"ultrasonicRight\":" + String(readUltrasonic(trigRight, echoRight)) + "}";
  client.println(status);
  Serial.println("סטטוס נשלח לשרת");
}


void receiveServerCommand() {
  if (client.connected() && client.available()) {
    String line = client.readStringUntil('\n');
    DynamicJsonDocument doc(512);
    if (!deserializeJson(doc, line)) {
      if (doc["command"] == "goTo" && !isDriving) {
          String dest = doc["destination"].as<String>();
          loadPathByName(dest, path);
          executePath(path);
      }
    }
  }
}