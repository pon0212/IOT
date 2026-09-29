#include <Arduino.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("--------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 1 - TAO VA CAU TRUC DU LIEU JSON");
  Serial.println("--------------------------------");

  StaticJsonDocument<200> doc;

  doc["device_id"] = "ESP32_01";
  doc["temperature"] = 25.5;
  doc["humidity"] = 60;
  doc["timestamp"] = millis();

  String jsonString;
  serializeJson(doc, jsonString);

  Serial.println("Du lieu JSON:");
  Serial.println(jsonString);
}

void loop() {
}