#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Sensor and buzzer pins
#define TRIG_PIN 9
#define ECHO_PIN 10
#define BUZZER_PIN 8

// Detection distance in centimetres
#define DETECT_DISTANCE 50

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1
);

int dots = 1;
bool personDetected = false;

unsigned long previousMillis = 0;
const int dotInterval = 400;

// Centre text on OLED
void centreText(const char* text, int y, int size) {
  display.setTextSize(size);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(
    text, 0, y, &x1, &y1, &w, &h
  );

  int x = (SCREEN_WIDTH - w) / 2 - x1;

  display.setCursor(x, y);
  display.print(text);
}

// Measure distance
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999;
  }

  return duration * 0.0343 / 2;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);

  // Start OLED
  if (!display.begin(
    SSD1306_SWITCHCAPVCC, OLED_ADDRESS
  )) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  float distance = getDistance();

  bool nearPerson = (distance <= DETECT_DISTANCE);

  // Welcome beep only when someone enters
  if (nearPerson && !personDetected) {
    tone(BUZZER_PIN, 2000, 200);
  }

  personDetected = nearPerson;

  display.clearDisplay();

  // Animated border
  display.drawRoundRect(
    1, 1, 126, 62, 6, SSD1306_WHITE
  );

  if (nearPerson) {

    // Welcome message
    centreText("WELCOME!", 13, 2);
    centreText("Atal Tinkering Lab", 35, 1);
    centreText("of PM SHRI KV Picket", 47, 1);

    // Moving decorative dot
    int x = 5 + (millis() / 20) % 110;
    display.fillCircle(x, 5, 2, SSD1306_WHITE);

  } else {

    // Waiting animation: . -> .. -> ...
    if (millis() - previousMillis >= dotInterval) {
      previousMillis = millis();

      dots++;
      if (dots > 3) {
        dots = 1;
      }
    }

    if (dots >= 1)
      display.fillCircle(55, 32, 2, SSD1306_WHITE);

    if (dots >= 2)
      display.fillCircle(64, 32, 2, SSD1306_WHITE);

    if (dots >= 3)
      display.fillCircle(73, 32, 2, SSD1306_WHITE);
  }

  display.display();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(50);
}