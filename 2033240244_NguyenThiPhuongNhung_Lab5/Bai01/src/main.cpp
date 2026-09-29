
#include <Arduino.h>
#include <WiFi.h>

// Thong tin WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Thoi gian kiem tra WiFi
unsigned long previousMillis = 0;
const unsigned long interval = 5000;

void connectWiFi() {
  Serial.println("Dang ket noi WiFi...");

  WiFi.begin(ssid, password);
}

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 1 - KET NOI WIFI ESP32");
  Serial.println("-----------------------------");

  WiFi.mode(WIFI_STA);

  // Bat dau ket noi WiFi
  connectWiFi();
}

void loop() {

  // Neu WiFi da ket noi
  if (WiFi.status() == WL_CONNECTED) {

    if (millis() - previousMillis >= interval) {
      previousMillis = millis();

      Serial.println("-----------------------------");
      Serial.println("WiFi da ket noi thanh cong!");
      Serial.print("Dia chi IP: ");
      Serial.println(WiFi.localIP());
    }

  } else {

    // Neu WiFi bi mat ket noi
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();

      Serial.println("Mat ket noi WiFi!");
      Serial.println("Dang thu ket noi lai...");

      WiFi.disconnect();
      connectWiFi();
    }
  }
}