#include <Arduino.h>
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
  SKEPTIC,
  SAD,
  BROKEN,
  TIRED,
  CRAZY,
  WINK,
  SURPRISED,
  ANGRY,
  INLOVE,
  HAPPY,
  DENYING,
  TOTAL_EMOTIONS
};

Emotion currentEmotion = NEUTRAL;

long lastDistance = 0;
unsigned long lastSwitch = 0;

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

// ===== EYE HELPERS =====
void eyeCircle(int x, int y, int r) {
  display.fillCircle(x, y, r, WHITE);
}

void eyeLine(int x1, int y1, int x2, int y2) {
  display.drawLine(x1, y1, x2, y2, WHITE);
}

// ===== DRAW EMOTIONS =====
void drawFace(Emotion e) {

  display.clearDisplay();

  int lx = 40;
  int rx = 88;
  int y  = 32;

  switch (e) {

    case NEUTRAL:
      eyeCircle(lx, y, 8);
      eyeCircle(rx, y, 8);
      break;

    case HAPPY:
      display.fillCircle(lx, y, 8, WHITE);
      display.fillCircle(rx, y, 8, WHITE);
      display.fillCircle(lx, y-3, 8, BLACK);
      display.fillCircle(rx, y-3, 8, BLACK);
      break;

    case SAD:
      display.fillCircle(lx, y, 8, WHITE);
      display.fillCircle(rx, y, 8, WHITE);
      display.fillCircle(lx, y+4, 8, BLACK);
      display.fillCircle(rx, y+4, 8, BLACK);
      break;

    case ANGRY:
      eyeLine(lx-8, y-6, lx+8, y+2);
      eyeLine(rx+8, y-6, rx-8, y+2);
      break;

    case SURPRISED:
      display.drawCircle(lx, y, 8, WHITE);
      display.drawCircle(rx, y, 8, WHITE);
      break;

    case WINK:
      eyeCircle(lx, y, 8);
      eyeLine(rx-8, y, rx+8, y);
      break;

    case TIRED:
      eyeLine(lx-8, y, lx+8, y);
      eyeLine(rx-8, y, rx+8, y);
      break;

    case SKEPTIC:
      eyeCircle(lx, y, 8);
      eyeLine(rx-8, y-4, rx+8, y-4);
      break;

    case CRAZY:
      display.drawCircle(lx, y, 8, WHITE);
      eyeCircle(rx, y, 3);
      break;

    case INLOVE:
      display.fillTriangle(lx-5,y-2,lx+5,y-2,lx,y+6,WHITE);
      display.fillTriangle(rx-5,y-2,rx+5,y-2,rx,y+6,WHITE);
      break;

    case BROKEN:
      eyeLine(lx-6,y-6,lx+6,y+6);
      eyeLine(lx+6,y-6,lx-6,y+6);
      eyeLine(rx-6,y-6,rx+6,y+6);
      eyeLine(rx+6,y-6,rx-6,y+6);
      break;

    case DENYING:
      eyeLine(lx-8, y, lx+8, y);
      eyeLine(rx-8, y, rx+8, y);
      display.drawLine(30,50,98,50,WHITE);
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

  if (abs(distance - lastDistance) > 4 && millis() - lastSwitch > 500) {
    currentEmotion = (Emotion)((currentEmotion + 1) % TOTAL_EMOTIONS);
    lastSwitch = millis();
  }

  lastDistance = distance;

  drawFace(currentEmotion);
  delay(60);
}


