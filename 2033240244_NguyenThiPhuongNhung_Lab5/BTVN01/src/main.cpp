#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// ============================
// WIFI
// ============================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ============================
// MQTT BROKER
// ============================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Topic riêng
const char* topic_env = "nhung2033240244/esp32/env";

// ============================
// DHT22
// ============================
const int DHT_PIN = 15;
DHTesp dhtSensor;

// ============================
// MQTT
// ============================
WiFiClient espClient;
PubSubClient client(espClient);

// Gửi dữ liệu mỗi 10 giây
unsigned long previousPublish = 0;
const unsigned long publishInterval = 3000;

// Thời gian thử kết nối lại
unsigned long previousReconnect = 0;
const unsigned long reconnectInterval = 3000;


// ========================================
// KẾT NỐI WIFI
// ========================================
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("Dang ket noi WiFi...");

  WiFi.begin(ssid, password);
}


// ========================================
// KẾT NỐI MQTT
// ========================================
void connectMQTT() {
  if (client.connected()) {
    return;
  }

  Serial.print("Dang ket noi MQTT...");

  if (client.connect("ESP32_NHUNG_2033240244_BTVN01")) {

    Serial.println("Thanh cong");

  } else {

    Serial.print("That bai, rc = ");
    Serial.println(client.state());
  }
}


// ========================================
// ĐỌC DHT22 VÀ PUBLISH MQTT
// ========================================
void publishDHTData() {

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  float temperature = data.temperature;
  float humidity = data.humidity;

  // Tạo JSON
  String payload = "{";
  payload += "\"temp\":";
  payload += String(temperature, 1);
  payload += ",";
  payload += "\"hum\":";
  payload += String(humidity, 1);
  payload += "}";

  // Gửi MQTT
  client.publish(topic_env, payload.c_str());

  // Hiển thị Terminal
  Serial.print("Gui MQTT: ");
  Serial.println(payload);
}


// ========================================
// SETUP
// ========================================
void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("=================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BTVN 01 - DHT22 MQTT");
  Serial.println("=================================");

  // Khởi tạo cảm biến DHT22
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  // WiFi
  WiFi.mode(WIFI_STA);
  connectWiFi();

  // MQTT
  client.setServer(mqtt_server, mqtt_port);
}


// ========================================
// LOOP
// ========================================
void loop() {

  unsigned long currentMillis = millis();

  // ============================
  // KIỂM TRA WIFI
  // ============================
  if (WiFi.status() != WL_CONNECTED) {

    if (currentMillis - previousReconnect >= reconnectInterval) {

      previousReconnect = currentMillis;
      connectWiFi();
    }

    return;
  }


  // ============================
  // KIỂM TRA MQTT
  // ============================
  if (!client.connected()) {

    if (currentMillis - previousReconnect >= reconnectInterval) {

      previousReconnect = currentMillis;
      connectMQTT();
    }

    return;
  }


  // Duy trì kết nối MQTT
  client.loop();


  // ============================
  // GỬI DỮ LIỆU MỖI 10 GIÂY
  // ============================
  if (currentMillis - previousPublish >= publishInterval) {

    previousPublish = currentMillis;

    publishDHTData();
  }
}