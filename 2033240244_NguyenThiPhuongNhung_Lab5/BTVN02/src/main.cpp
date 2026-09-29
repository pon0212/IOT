#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

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

// ============================
// LWT
// ============================
const char* statusTopic = "nhung2033240244/esp32/status";

const char* willMessage = "Offline";
const int willQoS = 0;
const bool willRetain = true;

// ============================
// MQTT
// ============================
WiFiClient espClient;
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
// KẾT NỐI MQTT + LWT
// ========================================
void connectMQTT() {

  if (client.connected()) {
    return;
  }

  Serial.print("Dang ket noi MQTT...");

  if (client.connect(
        "ESP32_NHUNG_2033240244_BTVN02",
        statusTopic,
        willQoS,
        willRetain,
        willMessage
      )) {

    Serial.println("Thanh cong");

    // Khi kết nối thành công → gửi Online retained
    client.publish(
      statusTopic,
      "Online",
      true
    );

    Serial.println("Trang thai: Online");

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
  Serial.println("BTVN 02 - MQTT LWT");
  Serial.println("=================================");

  WiFi.mode(WIFI_STA);

  connectWiFi();

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
}