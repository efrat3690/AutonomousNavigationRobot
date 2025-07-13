//מניעת כפילויות
#pragma once

extern HardwareSerial lidarSerial;

const int trigFront = 27, echoFront = 26;
const int trigRight = 14, echoRight = 12;
const int trigLeft = 33, echoLeft = 32;

void setupPinsSensors() {
  pinMode(trigFront, OUTPUT); pinMode(echoFront, INPUT);
  pinMode(trigRight, OUTPUT); pinMode(echoRight, INPUT);
  pinMode(trigLeft, OUTPUT); pinMode(echoLeft, INPUT);
}

//קריאת מידע מחיישן אולטרסוניק
long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 25000);
  return (duration == 0) ? 300 : duration * 0.034 / 2;
}

//קריאת מידע מחיישן לידר
int readLiDAR() {
  while (lidarSerial.available() >= 9) {
    if (lidarSerial.read() == 0x59 && lidarSerial.peek() == 0x59) {
      lidarSerial.read();  
      uint8_t low = lidarSerial.read();
      uint8_t high = lidarSerial.read();
      int distance = (high << 8) + low;

      // סינון ערכים לא סבירים
      if (distance >= 10 && distance <= 1200) {
        return distance;
      } else {
        return 999;
      }
    } else {
      // ניקוי באפר
      lidarSerial.read();
    }
  }

  return 999;
}

//בדיקת מסלול קידמי נקי ממכשולים
bool isPathClear() {
  long front = readUltrasonic(trigFront, echoFront);
  int lidarDist = readLiDAR();
  Serial.printf("🔍 מרחק קדמי: %ld | LiDAR: %d\n", front, lidarDist);
  return (front > 20 && lidarDist > 20);
}

//bool tryAvoidObstacle();