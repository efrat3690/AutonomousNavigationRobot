#pragma once

#include "Sensors.h"
#include "Motors.h"

struct Command {
  String action;
  int distance;
};

//מניעת הגדרות כפולות
extern bool isDriving;
extern String currentStatus;
extern float distanceTravelled;


//עקיפת מכשולים במהלך הנסיעה
bool tryAvoidObstacle() {
  Serial.println("🔄 מנסה לעקוף את המכשול...");

  // שלב 1: מדידת מרחקים צדדיים
  long left = readUltrasonic(trigLeft, echoLeft);
  long right = readUltrasonic(trigRight, echoRight);
  Serial.printf("מרחקים | שמאל: %ld | ימין: %ld\n", left, right);

  // שלב 2: בדיקה אם שני הצדדים חסומים
  if (left < 25 && right < 25) {
    Serial.println("אין אפשרות לעקוף – שני הצדדים חסומים");
    return false;
  }

  // שלב 3: בחירת צד בטוח יותר
  String side = (left > right) ? "left" : "right";
  String opposite = (side == "left") ? "right" : "left";

  // שלב 4.1: פנייה הצידה
  Serial.println(" פנייה ל" + side);
  moveByAction(side);
  delay(600);
  moveByAction("stop");
  delay(200);

  // שלב 4.2: נסיעה קדימה קצרה
  Serial.println(" תנועה קדימה לעקיפה");
  moveByAction("forward");
  delay(500);
  moveByAction("stop");
  delay(200);

  // שלב 4.3: חזרה למסלול
  Serial.println(" חזרה למסלול דרך " + opposite);
  moveByAction(opposite);
  delay(600);
  moveByAction("stop");
  delay(200);

  // שלב 4.4: בדיקה האם הדרך פנויה
  if (isPathClear()) {
    Serial.println(" העקיפה הצליחה! ממשיכים בנסיעה");
    return true;
  } else {
    Serial.println(" העקיפה נכשלה – הדרך עדיין חסומה");
    return false;
  }
}


//ביצוע פעולת תנועה יחידה
void performCommand(const Command& cmd) {
  if (cmd.action == "forward" && !isPathClear()) {
    if (!tryAvoidObstacle()) {
      moveByAction("stop");
      return;
    }
  }
  
  moveByAction(cmd.action);
  delay(cmd.distance * 10);
  moveByAction("stop");
  delay(400);

  if (cmd.action == "forward" || cmd.action == "backward") {
    distanceTravelled += cmd.distance;
  }
}

//הרצת הרכב במסלול פקודה אחר פקודה
void executePath(const std::vector<Command>& commands) {
  isDriving = true;
  currentStatus = "driving";
  for (auto& cmd : commands)
    performCommand(cmd);
  currentStatus = "idle";
  isDriving = false;
}


// הפיכת מסלול לכיוון ההפוך
std::vector<Command> reversePath(const std::vector<Command>& original) {
  std::vector<Command> reversed;
  for (int i = original.size() - 1; i >= 0; --i) {
    Command cmd = original[i];
    Command reversedCmd;
    reversedCmd.distance = cmd.distance;

    if (cmd.action == "forward")
      reversedCmd.action = "backward";
    else if (cmd.action == "backward")
      reversedCmd.action = "forward";
    else if (cmd.action == "left")
      reversedCmd.action = "right";
    else if (cmd.action == "right")
      reversedCmd.action = "left";
    else
      reversedCmd.action = "stop";

    reversed.push_back(reversedCmd);
  }
  return reversed;
}
