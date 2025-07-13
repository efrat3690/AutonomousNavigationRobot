#pragma once

#include <FS.h>              // נדרש בשביל fs::File
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "PathLogic.h"

void loadDestinationNames(std::vector<String>& names) {
  fs::File file = LittleFS.open("/paths.json", "r");
  if (!file) return;

  DynamicJsonDocument doc(4096);
  deserializeJson(doc, file);
  for (JsonPair kv : doc.as<JsonObject>())
    names.push_back(kv.key().c_str());
    
  file.close();
}

void loadPathByName(String name, std::vector<Command>& path) {
  path.clear();  // מנקה את הקודם

  fs::File file = LittleFS.open("/paths.json", "r");
  if (!file) return;

  DynamicJsonDocument doc(4096);
  deserializeJson(doc, file);
  JsonArray arr = doc[name];
  for (JsonObject cmd : arr)
    path.push_back({cmd["action"].as<String>(), cmd["distance"].as<int>()});

  file.close();
}
