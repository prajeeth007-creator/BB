#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== Emotion System =====
enum Mood { NEUTRAL, HAPPY, ANGRY, SAD };
Mood currentMood = NEUTRAL;

unsigned long lastInteraction = 0;
const unsigned long lonelyTime = 15000; // 15 seconds → sad

// ===== Eye Settings =====
int baseY = 16;
int eyeHeight = 36;

// ===== Draw Face =====
void drawSpiderEyes(Mood mood) {

  display.clearDisplay();

  int lx = 18;
  int rx = 70;
  int y  = baseY;

  if (mood == NEUTRAL) {
    display.fillRoundRect(lx, y, 40, eyeHeight, 20, WHITE);
    display.fillRoundRect(rx, y, 40, eyeHeight, 20, WHITE);
  }

  else if (mood == HAPPY) {
    display.fillTriangle(lx, y+15, lx+40, y+5, lx+20, y+eyeHeight, WHITE);
    display.fillTriangle(rx+40, y+15, rx, y+5, rx+20, y+eyeHeight, WHITE);
  }

  else if (mood == ANGRY) {
    display.fillTriangle(lx, y, lx+40, y+20, lx+20, y+eyeHeight, WHITE);
    display.fillTriangle(rx+40, y, rx, y+20, rx+20, y+eyeHeight, WHITE);
  }

  else if (mood == SAD) {
    display.fillTriangle(lx, y+20, lx+40, y+eyeHeight, lx+20, y, WHITE);
    display.fillTriangle(rx+40, y+20, rx, y+eyeHeight, rx+20, y, WHITE);
  }

  display.display();
}

// ===== Input System (keyboard now, sensors later) =====
void readInput() {

  if (Serial.available()) {
    char c = Serial.read();

    if (c == 'n') currentMood = NEUTRAL;
    if (c == 'h') currentMood = HAPPY;
    if (c == 'a') currentMood = ANGRY;
    if (c == 's') currentMood = SAD;

    lastInteraction = millis(); // remember interaction
  }
}

// ===== Brain / Behavior =====
void updateEmotion() {

  // If no interaction for long time → sad
  if (millis() - lastInteraction > lonelyTime) {
    currentMood = SAD;
  }
}

// ===== Setup =====
void setup() {

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    while (true);
  }

  display.clearDisplay();
  display.display();

  lastInteraction = millis();
}

// ===== Main Loop =====
void loop() {
  readInput();
  updateEmotion();
  drawSpiderEyes(currentMood);
  delay(100);
}
ś
