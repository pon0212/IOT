#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// WiFi Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// NTP
const char* ntpServer = "time.google.com";

// Viet Nam UTC+7
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 5 - HIEN THI THOI GIAN NTP OLED");
  Serial.println("-----------------------------");

  // Khoi dong OLED
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Khong tim thay OLED!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Dang ket noi WiFi...");
  display.display();

  // Ket noi WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Ket noi WiFi thanh cong!");

  // Dong bo NTP
  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer
  );

  delay(2000);
}

void loop() {
  struct tm timeinfo;

  display.clearDisplay();
  display.setCursor(0, 0);

  // Trang thai WiFi
  if (WiFi.status() == WL_CONNECTED) {
    display.println("WiFi: Connected");
  } else {
    display.println("WiFi: Disconnected");
  }

  // Lay thoi gian
  if (getLocalTime(&timeinfo, 5000)) {

    char dateBuffer[20];
    char timeBuffer[20];

    sprintf(
      dateBuffer,
      "%02d/%02d/%04d",
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1,
      timeinfo.tm_year + 1900
    );

    sprintf(
      timeBuffer,
      "%02d:%02d:%02d",
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec
    );

    display.println();
    display.print("Date: ");
    display.println(dateBuffer);

    display.print("Time: ");
    display.println(timeBuffer);

    Serial.printf(
      "Ngay: %s | Gio: %s\r\n",
      dateBuffer,
      timeBuffer
    );

  } else {
    display.println();
    display.println("NTP Error!");

    Serial.println("Khong lay duoc thoi gian!");
  }

  display.display();

  delay(1000);
}