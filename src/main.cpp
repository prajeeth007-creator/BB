#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED SETTINGS =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== ULTRASONIC PINS =====
#define TRIG_PIN 5
#define ECHO_PIN 18

// ===== Emotion System =====
enum Mood { NEUTRAL, HAPPY, ANGRY, SAD };
Mood currentMood = NEUTRAL;

unsigned long lastInteraction = 0;

// ===== Eye Settings =====
int baseY = 16;
int eyeHeight = 36;

// ===== Distance Function =====
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return -1; // no signal

  long distance = duration * 0.034 / 2;
  return distance;
}

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

// ===== Behavior =====
void updateEmotion() {

  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.println(distance);

  // VERY CLOSE → HAPPY
  if (distance > 0 && distance < 10) {
    currentMood = HAPPY;
    lastInteraction = millis();
  }
  // MEDIUM → SAD
  else if (distance >= 10 && distance < 40) {
    currentMood = SAD;
    lastInteraction = millis();
  }
  // FAR / NOTHING → ANGRY
  else {
    currentMood = ANGRY;
  }
}

// ===== Setup =====
void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);  // SDA, SCL

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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
  updateEmotion();
  drawSpiderEyes(currentMood);
  delay(120);
}
