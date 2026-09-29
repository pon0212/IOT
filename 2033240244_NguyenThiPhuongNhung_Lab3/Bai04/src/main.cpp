#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// NTP Server
const char* ntpServer = "time.google.com";

// Viet Nam UTC+7
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 4 - DONG BO THOI GIAN NTP");
  Serial.println("-----------------------------");

  // Ket noi WiFi
  Serial.print("Dang ket noi WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Ket noi WiFi thanh cong!");

  // Dong bo NTP
  Serial.println("Dang dong bo thoi gian NTP...");

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer
  );

  delay(2000);
}

void loop() {
  struct tm timeinfo;

  if (getLocalTime(&timeinfo, 5000)) {

    Serial.printf(
      "Ngay: %02d/%02d/%04d | Gio: %02d:%02d:%02d\r\n",
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1,
      timeinfo.tm_year + 1900,
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec
    );

  } else {
    Serial.println("Chua dong bo duoc NTP, dang thu lai...");
  }

  delay(1000);
}