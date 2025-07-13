#pragma once

#include "PathLogic.h"
#include "Storage.h"
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// ⚠️ ודאי שהקובץ main.ino כולל:
// #define TOUCH_CS 21
// #define TOUCH_IRQ 22
// XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

extern XPT2046_Touchscreen touch;
extern std::vector<Command> path;
extern bool isDriving;

// הגדרות קבועות לגודל כפתורים
#define BUTTON_WIDTH  220
#define BUTTON_HEIGHT 40
#define BUTTON_SPACING 10

unsigned long lastTouchTime = 0;
const unsigned long touchDebounce = 500; // מניעת מגע כפול תוך 0.5 שניות

// מצייר את כל הכפתורים על המסך
void drawButtons(TFT_eSPI& tft, const std::vector<String>& names) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  int y = 10;

  for (size_t i = 0; i < names.size(); ++i) {
    tft.fillRoundRect(10, y, BUTTON_WIDTH, BUTTON_HEIGHT, 5, TFT_BLUE);
    tft.setCursor(20, y + 10);
    tft.print(names[i]);
    y += BUTTON_HEIGHT + BUTTON_SPACING;
  }
}

// מזהה איזה כפתור נלחץ לפי מיקום מגע
int detectButtonPressed(int x, int y, size_t count) {
  int btnY = 10;
  for (size_t i = 0; i < count; ++i) {
    if (x >= 10 && x <= 10 + BUTTON_WIDTH &&
        y >= btnY && y <= btnY + BUTTON_HEIGHT)
      return i;
    btnY += BUTTON_HEIGHT + BUTTON_SPACING;
  }
  return -1;
}

// מטפל באירועי מגע ומפעיל נסיעה
void handleTouch(TFT_eSPI& tft, std::vector<String>& names) {
  unsigned long now = millis();

  if (touch.touched() && !isDriving && (now - lastTouchTime > touchDebounce)) {
    TS_Point p = touch.getPoint();

    // ייתכן וצריך להחליף X/Y או להפוך אותם – נבדוק בתכל'ס
    int x = map(p.x, 200, 3900, 0, 240);  // שנה את הערכים אם צריך
    int y = map(p.y, 200, 3900, 0, 320);

    int index = detectButtonPressed(x, y, names.size());
    if (index >= 0) {
      Serial.printf("🖐 יעד נבחר: %s\n", names[index].c_str());
      loadPathByName(names[index], path);   // שומר את המסלול במערך הראשי
      executePath(path);                    // מתחיל נסיעה
      lastTouchTime = now;
    }
  }
}
