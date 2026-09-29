#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// ============================
// WIFI
// ============================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ============================
// HIVE MQ CLOUD
// ============================
const char* mqtt_server =
  "ba826690c43f4cfeb610dd1ef9d92064.s1.eu.hivemq.cloud";

const int mqtt_port = 8883;

// ============================
// MQTT ACCOUNT
// ============================
const char* mqtt_username = "nhung2033240244";

const char* mqtt_password = "nhung2033240244";

// ============================
// MQTT CLIENT
// ============================
WiFiClientSecure espClient;
PubSubClient client(espClient);

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

  Serial.print("Dang ket noi MQTT Secure...");

  if (client.connect(
        "ESP32_NHUNG_2033240244_BTVN03",
        mqtt_username,
        mqtt_password
      )) {

    Serial.println("Thanh cong");

  } else {

    Serial.print("That bai, rc = ");
    Serial.println(client.state());
  }
}


// ========================================
// SETUP
// ========================================
void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("=================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BTVN 03 - MQTT USERNAME PASSWORD");
  Serial.println("=================================");

  WiFi.mode(WIFI_STA);

  connectWiFi();

  // Bỏ qua kiểm tra chứng chỉ để mô phỏng Wokwi
  espClient.setInsecure();

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


  client.loop();
}