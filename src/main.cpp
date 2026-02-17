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
  CONFUSED,
  CRYING,
  DEAD,
  LOVE,
  SQUINT,
  SLEEPY,
  SHY,
  BORED,
  TOTAL_EMOTIONS
};

Emotion currentEmotion = NEUTRAL;
Emotion previousEmotion = NEUTRAL;

// ===== TRIGGER =====
bool handDetected = false;
const int triggerDistance = 10;

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

// ===== BOLD EYE =====
void fillBoldCircle(int x, int y, int r) {
  display.fillCircle(x, y, r, WHITE);
  display.drawCircle(x, y, r, WHITE);
  display.drawCircle(x, y, r-1, WHITE);
}

// ===== HEART =====
void drawHeart(int x, int y) {
  display.fillCircle(x-4, y-2, 4, WHITE);
  display.fillCircle(x+4, y-2, 4, WHITE);
  display.fillTriangle(x-8, y-2, x+8, y-2, x, y+10, WHITE);
}

// ===== SPIRAL =====
void drawSpiral(int cx, int cy) {
  for (int i = 0; i < 12; i++) {
    display.drawCircle(cx, cy, i, WHITE);
  }
}

// ===== TEAR =====
void drawTear(int x, int y) {
  display.fillCircle(x, y, 3, WHITE);
  display.drawLine(x, y-4, x, y-1, WHITE);
}

// ===== X EYE =====
void drawXEye(int x, int y) {
  display.drawLine(x-10, y-10, x+10, y+10, WHITE);
  display.drawLine(x+10, y-10, x-10, y+10, WHITE);
}

// ===== SLEEP EYE =====
void drawSleepEye(int x, int y) {
  display.drawLine(x-12, y, x+12, y, WHITE);
  display.drawLine(x-10, y+2, x+10, y+2, WHITE);
}

// ===== BLINK =====
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

// ===== EASING =====
float easeInOut(float t) {
  return t * t * (3 - 2 * t);
}

// ===== SMOOTH EYELID ANIMATION =====
void animateEyes(int startOffset, int endOffset) {

  int lx = 36;
  int rx = 92;
  int y  = 32;
  int r  = 18;

  for (int i = 0; i <= 12; i++) {

    float t = (float)i / 12.0;
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

// ===== DRAW FACE =====
void drawStaticFace(Emotion e) {

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
      fillBoldCircle(lx, y-6, r);
      fillBoldCircle(rx, y-6, r);
      break;

    case SAD:
      fillBoldCircle(lx, y+8, r);
      fillBoldCircle(rx, y+8, r);
      break;

    case ANGRY:
      display.drawLine(lx-18, y-12, lx+18, y+4, WHITE);
      display.drawLine(rx+18, y-12, rx-18, y+4, WHITE);
      break;

    case SURPRISED:
      display.drawCircle(lx, y, r, WHITE);
      display.drawCircle(rx, y, r, WHITE);
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
    case LOVE:
      drawHeart(lx, y);
      drawHeart(rx, y);
      break;

    case CONFUSED:
      drawSpiral(lx, y);
      drawSpiral(rx, y);
      break;

    case CRYING:
      fillBoldCircle(lx, y, r);
      fillBoldCircle(rx, y, r);
      drawTear(lx, y+18);
      break;

    case DEAD:
      drawXEye(lx, y);
      drawXEye(rx, y);
      break;

    case SQUINT:
      display.drawLine(lx-14, y-6, lx+14, y+6, WHITE);
      display.drawLine(lx-14, y+6, lx+14, y-6, WHITE);
      display.drawLine(rx-14, y-6, rx+14, y+6, WHITE);
      display.drawLine(rx-14, y+6, rx+14, y-6, WHITE);
      break;

    case SLEEPY:
      drawSleepEye(lx, y);
      drawSleepEye(rx, y);
      break;

    case SHY:
      display.fillTriangle(lx-12,y+4,lx+12,y+4,lx,y-8,WHITE);
      display.fillTriangle(rx-12,y+4,rx+12,y+4,rx,y-8,WHITE);
      break;

    case BORED:
      display.drawLine(lx-18, y-4, lx+18, y-4, WHITE);
      display.drawLine(rx-18, y-4, rx+18, y-4, WHITE);
      break;
  }

  display.display();
}

// ===== TRANSITION =====
void playTransition(Emotion e) {
  cinematicBlink();
  if (e == HAPPY) animateEyes(0, -6);
  if (e == SAD) animateEyes(0, 8);
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

  if (distance > 0 && distance < triggerDistance && !handDetected) {
    handDetected = true;
    previousEmotion = currentEmotion;
    currentEmotion = (Emotion)((currentEmotion + 1) % TOTAL_EMOTIONS);
    playTransition(currentEmotion);
  }

  if (distance >= triggerDistance + 5) {
    handDetected = false;
  }

  drawStaticFace(currentEmotion);
  delay(40);
}


