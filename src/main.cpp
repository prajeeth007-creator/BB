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

// ===== BOLD CIRCLE =====
void fillBoldCircle(int x, int y, int r) {
  display.fillCircle(x, y, r, WHITE);
  display.drawCircle(x, y, r, WHITE);
  display.drawCircle(x, y, r-1, WHITE);
}

// ===== CINEMATIC BLINK =====
void cinematicBlink() {
  int lx = 36;
  int rx = 92;
  int y  = 32;

  for (int i = 0; i < 3; i++) {
    display.clearDisplay();
    display.drawLine(lx-20, y, lx+20, y, WHITE);
    display.drawLine(rx-20, y, rx+20, y, WHITE);
    display.display();
    delay(45);
  }
}

// ===== EASE FUNCTION =====
float easeInOut(float t) {
  return t * t * (3 - 2 * t);
}

// ===== ANIMATE EYES =====
void animateEyes(int startOffset, int endOffset, int frames = 12) {

  int lx = 36;
  int rx = 92;
  int y  = 32;
  int r  = 18;

  for (int i = 0; i <= frames; i++) {

    float t = (float)i / frames;
    float e = easeInOut(t);

    int offset = startOffset + (endOffset - startOffset) * e;

    display.clearDisplay();

    fillBoldCircle(lx, y, r);
    fillBoldCircle(rx, y, r);

    display.fillCircle(lx, y + offset, r, BLACK);
    display.fillCircle(rx, y + offset, r, BLACK);

    display.display();
    delay(25);
  }
}

// ===== DRAW STATIC EXPRESSIONS =====
void drawFace(Emotion e) {

  display.clearDisplay();

  int lx = 36;
  int rx = 92;
  int y  = 32;
  int r  = 18;

  switch (e) {

    case NEUTRAL:
      fillBoldCircle(lx, y, r);
      fillBoldCircle(rx, y, r);
      break;

    case HAPPY:
      animateEyes(0, -6);
      return;

    case SAD:
      animateEyes(0, 8);
      return;

    case ANGRY:
      display.drawLine(lx-18, y-12, lx+18, y+4, WHITE);
      display.drawLine(rx+18, y-12, rx-18, y+4, WHITE);
      break;

    case SURPRISED:
      display.drawCircle(lx, y, r, WHITE);
      display.drawCircle(rx, y, r, WHITE);
      display.drawCircle(lx, y, r-1, WHITE);
      display.drawCircle(rx, y, r-1, WHITE);
      break;

    case WINK:
      fillBoldCircle(lx, y, r);
      display.drawLine(rx-20, y, rx+20, y, WHITE);
      break;

    case TIRED:
      display.drawLine(lx-20, y, lx+20, y, WHITE);
      display.drawLine(rx-20, y, rx+20, y, WHITE);
      break;

    case INLOVE:
      display.fillTriangle(lx-10,y-2,lx+10,y-2,lx,y+12,WHITE);
      display.fillTriangle(rx-10,y-2,rx+10,y-2,rx,y+12,WHITE);
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

  if (distance > 0 && distance < triggerDistance && !handDetected) {
    handDetected = true;
    cinematicBlink();
    currentEmotion = (Emotion)((currentEmotion + 1) % TOTAL_EMOTIONS);
  }

  if (distance >= triggerDistance + 5) {
    handDetected = false;
  }

  drawFace(currentEmotion);
  delay(40);
}


