#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== ULTRASONIC =====
#define TRIG_PIN 5
#define ECHO_PIN 18

// ===== EMOTIONS =====
enum Emotion {
  NEUTRAL,
  HAPPY,
  SAD,
  ANGRY,
  SURPRISED,
  WINK,
  TIRED,
  INLOVE,
  TOTAL_EMOTIONS
};

Emotion currentEmotion = NEUTRAL;

// ===== TRIGGER SYSTEM =====
bool handDetected = false;
const int triggerDistance = 10;   // cm

// ===== READ DISTANCE =====
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

// ===== DRAW BIG CUTE EYES =====
void drawFace(Emotion e) {

  display.clearDisplay();

  int lx = 36;
  int rx = 92;
  int y  = 32;
  int r  = 14;

  switch (e) {

    case NEUTRAL:
      display.fillCircle(lx, y, r, WHITE);
      display.fillCircle(rx, y, r, WHITE);
      break;

    case HAPPY:
      display.fillCircle(lx, y, r, WHITE);
      display.fillCircle(rx, y, r, WHITE);
      display.fillCircle(lx, y-6, r, BLACK);
      display.fillCircle(rx, y-6, r, BLACK);
      break;

    case SAD:
      display.fillCircle(lx, y, r, WHITE);
      display.fillCircle(rx, y, r, WHITE);
      display.fillCircle(lx, y+8, r, BLACK);
      display.fillCircle(rx, y+8, r, BLACK);
      break;

    case ANGRY:
      display.drawLine(lx-14, y-10, lx+14, y+2, WHITE);
      display.drawLine(rx+14, y-10, rx-14, y+2, WHITE);
      break;

    case SURPRISED:
      display.drawCircle(lx, y, r, WHITE);
      display.drawCircle(rx, y, r, WHITE);
      break;

    case WINK:
      display.fillCircle(lx, y, r, WHITE);
      display.drawLine(rx-14, y, rx+14, y, WHITE);
      break;

    case TIRED:
      display.drawLine(lx-14, y, lx+14, y, WHITE);
      display.drawLine(rx-14, y, rx+14, y, WHITE);
      break;

    case INLOVE:
      display.fillTriangle(lx-8,y-2,lx+8,y-2,lx,y+10,WHITE);
      display.fillTriangle(rx-8,y-2,rx+8,y-2,rx,y+10,WHITE);
      break;
  }

  display.display();
}

// ===== SETUP =====
void setup() {

  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

// ===== LOOP =====
void loop() {

  long distance = readDistanceCM();
  Serial.println(distance);

  // Hand comes close → trigger ONCE
  if (distance > 0 && distance < triggerDistance && !handDetected) {
    handDetected = true;
    currentEmotion = (Emotion)((currentEmotion + 1) % TOTAL_EMOTIONS);
  }

  // Hand removed → reset trigger
  if (distance >= triggerDistance + 5) {
    handDetected = false;
  }

  drawFace(currentEmotion);
  delay(80);
}


