#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 3 - QUAN LY WIFI VOI WIFIMANAGER");
  Serial.println("-----------------------------");

  WiFiManager wifiManager;

  Serial.println("Dang kiem tra WiFi...");

  bool ketNoiThanhCong = wifiManager.autoConnect("WFM-PhuongNhung", "12345678");

  if (!ketNoiThanhCong) {
    Serial.println("Khong the ket noi WiFi!");
    ESP.restart();
  }

  Serial.println("Ket noi WiFi thanh cong!");
  Serial.print("Dia chi IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

}