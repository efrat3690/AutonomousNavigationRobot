#pragma once
#include <Arduino.h>

// הגדרת הפינים
const int IN1 = 18, IN2 = 19, IN3 = 13, IN4 = 25;

// כיווני תנועה
enum Direction {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  STOP
};

// טבלת תנועה - לכל כיוון, מצב HIGH/LOW של הפינים
const int motorStates[][4] = {
  /* FORWARD  */ {HIGH, LOW, HIGH, LOW},
  /* BACKWARD */ {LOW, HIGH, LOW, HIGH},
  /* LEFT     */ {LOW, HIGH, HIGH, LOW},
  /* RIGHT    */ {HIGH, LOW, LOW, HIGH},
  /* STOP     */ {LOW, LOW, LOW, LOW}
};

// אתחול הפינים
void setupPins() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// פונקציה שמבצעת תנועה לפי כיוון
void move(Direction dir) {
  digitalWrite(IN1, motorStates[dir][0]);
  digitalWrite(IN2, motorStates[dir][1]);
  digitalWrite(IN3, motorStates[dir][2]);
  digitalWrite(IN4, motorStates[dir][3]);
}

// אופציונלית: לעבודה עם מחרוזת (כמו בקוד המקורי שלך)
void moveByAction(String action) {
  if (action == "forward") move(FORWARD);
  else if (action == "backward") move(BACKWARD);
  else if (action == "left") move(LEFT);
  else if (action == "right") move(RIGHT);
  else move(STOP);
}
