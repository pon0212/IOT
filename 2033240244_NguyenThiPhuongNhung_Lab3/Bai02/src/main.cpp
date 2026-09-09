#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 2 - TU DONG KET NOI LAI WIFI");
  Serial.println("-----------------------------");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Dang ket noi WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Ket noi WiFi thanh cong!");

  Serial.print("Dia chi IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mat ket noi WiFi!");
    Serial.println("Dang ket noi lai...");

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("Da ket noi lai WiFi!");

    Serial.print("Dia chi IP: ");
    Serial.println(WiFi.localIP());
  }

  delay(1000);
}