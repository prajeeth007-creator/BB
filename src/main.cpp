#include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// ===== OLED =====
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== IR SENSOR =====
#define IR_PIN 19   // change if needed

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

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  // Wire.begin(21, 22);
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display.clearDisplay();
  // display.display();
}

// ===== TRANSITION (OLED DISABLED) =====
void playTransition(Emotion e) {
  Serial.print("Transition to emotion: ");
  Serial.println(e);
}

// ===== LOOP =====
void loop() {

  bool objectDetected = digitalRead(IR_PIN) == LOW;
  // ↑ change LOW to HIGH if your sensor works reversed

  if (objectDetected && !handDetected) {
    handDetected = true;

    previousEmotion = currentEmotion;
    currentEmotion = (Emotion)((currentEmotion + 1) % TOTAL_EMOTIONS);

    playTransition(currentEmotion);
  }

  if (!objectDetected) {
    handDetected = false;
  }

  delay(40);
}


