
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Thong tin WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Thong tin MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Tao doi tuong WiFi va MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Thoi gian kiem tra ket noi
unsigned long previousMillis = 0;
const unsigned long interval = 5000;

// Ham ket noi WiFi
void connectWiFi() {

  Serial.println("Dang ket noi WiFi...");

  WiFi.begin(ssid, password);
}

// Ham ket noi MQTT
void connectMQTT() {

  Serial.println("Dang ket noi MQTT Broker...");

  if (client.connect("ESP32_NHUNG_2033240244")) {

    Serial.println("Ket noi MQTT thanh cong!");

    Serial.print("Broker: ");
    Serial.println(mqtt_server);

    Serial.print("Port: ");
    Serial.println(mqtt_port);

  } else {

    Serial.println("Ket noi MQTT that bai!");

    Serial.print("Ma loi MQTT: ");
    Serial.println(client.state());
  }
}

void setup() {

  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 2 - KET NOI MQTT BROKER");
  Serial.println("-----------------------------");

  // Cau hinh WiFi
  WiFi.mode(WIFI_STA);

  // Ket noi WiFi
  connectWiFi();

  // Cau hinh MQTT Broker
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {

  // Kiem tra WiFi
  if (WiFi.status() != WL_CONNECTED) {

    // Neu mat WiFi thi thu ket noi lai
    if (millis() - previousMillis >= interval) {

      previousMillis = millis();

      Serial.println("WiFi chua ket noi!");

      WiFi.reconnect();
    }

    return;
  }

  // Kiem tra MQTT
  if (!client.connected()) {

    if (millis() - previousMillis >= interval) {

      previousMillis = millis();

      connectMQTT();
    }

  } else {

    // Duy tri ket noi MQTT
    client.loop();
  }
}