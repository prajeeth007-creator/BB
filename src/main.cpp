#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int baseY = 16;
int eyeHeight = 36;

void drawSpiderEyes(int mood) {

  display.clearDisplay();

  int lx = 18;
  int rx = 70;
  int y  = baseY;

  // NEUTRAL
  if (mood == 0) {
    display.fillRoundRect(lx, y, 40, eyeHeight, 20, WHITE);
    display.fillRoundRect(rx, y, 40, eyeHeight, 20, WHITE);
  }

  // HAPPY
  if (mood == 1) {
    display.fillTriangle(lx, y+15, lx+40, y+5, lx+20, y+eyeHeight, WHITE);
    display.fillTriangle(rx+40, y+15, rx, y+5, rx+20, y+eyeHeight, WHITE);
  }

  // ANGRY
  if (mood == 2) {
    display.fillTriangle(lx, y, lx+40, y+20, lx+20, y+eyeHeight, WHITE);
    display.fillTriangle(rx+40, y, rx, y+20, rx+20, y+eyeHeight, WHITE);
  }

  // SAD
  if (mood == 3) {
    display.fillTriangle(lx, y+20, lx+40, y+eyeHeight, lx+20, y, WHITE);
    display.fillTriangle(rx+40, y+20, rx, y+eyeHeight, rx+20, y, WHITE);
  }

  display.display();
}

void setup() {

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    while (true);
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  drawSpiderEyes(0);
  delay(1200);

  drawSpiderEyes(1);
  delay(1200);

  drawSpiderEyes(2);
  delay(1200);

  drawSpiderEyes(3);
  delay(1200);
}

