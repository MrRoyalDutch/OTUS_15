#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NewPing.h>

#define TRIGGER_PIN 26 // вход датчика
#define ECHO_PIN 27 // выход датчика

// светики
#define PIN_LED_DISTANCE_MIN 5
#define PIN_LED_DISTANCE_MID 18
#define PIN_LED_DISTANCE_MAX 19

// максимальное расстояние, обнаруживаемое датчиком (в см)
#define MAX_DISTANCE 400

// OLED дисплей (в пикселях) 
#define SCREEN_WIDTH 128 // ширина
#define SCREEN_HEIGHT 64 // высота

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Пин сброса. -1: не используется

// создаем с помощью библиотеки NewPing объект для работы с ультразвуковым датчиком
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long distance = {}; // дистанция, которую вернула библиотека NewPing

void setup() {
  pinMode(PIN_LED_DISTANCE_MIN, OUTPUT);
  pinMode(PIN_LED_DISTANCE_MID, OUTPUT);
  pinMode(PIN_LED_DISTANCE_MAX, OUTPUT);
  
  Serial.begin(115200);
  
  // Вопрос: Почему с адресом 0x78, указанным на корпусе, дисплей не работает, а с адресом 0x3C работает?
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);    
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();
}

void loop() {
  distance = sonar.ping_cm();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Distance: ");
  display.setTextSize(3);
  display.print(distance);
  display.setTextSize(2);
  display.print(" cm");
  display.display();

  digitalWrite(PIN_LED_DISTANCE_MIN, LOW);
  digitalWrite(PIN_LED_DISTANCE_MID, LOW);
  digitalWrite(PIN_LED_DISTANCE_MAX, LOW);
  if (distance < 50)
    digitalWrite(PIN_LED_DISTANCE_MIN, HIGH);
  else if (distance > 100)
    digitalWrite(PIN_LED_DISTANCE_MAX, HIGH);
  else
    digitalWrite(PIN_LED_DISTANCE_MID, HIGH);

  // Datasheet: Suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal
  delay(100);
}

