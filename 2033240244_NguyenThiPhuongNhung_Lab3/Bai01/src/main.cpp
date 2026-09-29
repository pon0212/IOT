#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244"); 
  Serial.println("BAI 1 - KET NOI WIFI");
  Serial.println("-----------------------------");
  Serial.println("Dang ket noi WiFi...");

  WiFi.begin(ssid, password);

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

}