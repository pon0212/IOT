#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

// WiFi Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Dia chi de kiem tra Internet
IPAddress pingAddress(8, 8, 8, 8);

// 5 phut
const unsigned long checkInterval = 5UL * 60UL * 1000UL;

unsigned long previousMillis = 0;
int failedCount = 0;

void connectWiFi() {
  Serial.print("Dang ket noi WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Ket noi WiFi thanh cong!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 6 - WATCHDOG WIFI");
  Serial.println("-----------------------------");

  connectWiFi();

  // De kiem tra ngay khi khoi dong
  previousMillis = millis() - checkInterval;
}

void loop() {

  // Neu mat WiFi thi ket noi lai
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mat ket noi WiFi!");
    connectWiFi();
  }

  // Moi 5 phut kiem tra Internet 1 lan
  if (millis() - previousMillis >= checkInterval) {

    previousMillis = millis();

    Serial.println("-----------------------------");
    Serial.println("Dang kiem tra Internet...");

    bool success = Ping.ping(pingAddress, 1);

    if (success) {
      Serial.println("Ping thanh cong - Internet OK");
      failedCount = 0;
    }
    else {
      failedCount++;

      Serial.print("Ping that bai! So lan loi: ");
      Serial.println(failedCount);

      if (failedCount >= 3) {
        Serial.println("That bai 3 lan!");
        Serial.println("ESP32 se khoi dong lai...");

        delay(2000);
        ESP.restart();
      }
    }
  }
}