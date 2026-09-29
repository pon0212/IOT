#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi cua Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

void connectWiFi() {
  Serial.print("Dang ket noi WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Da ket noi WiFi");
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Thanh cong");
    } else {
      Serial.println("That bai, thu lai...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("--------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 2 - PUBLISH JSON LEN MQTT");
  Serial.println("--------------------------------");

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();

  if (millis() - lastMsg >= 5000) {
    lastMsg = millis();

    float temperature = random(200, 350) / 10.0;
    float humidity = random(400, 800) / 10.0;

    StaticJsonDocument<200> doc;

    doc["device_id"] = "ESP32_01";
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["timestamp"] = millis();

    String jsonString;
    serializeJson(doc, jsonString);

    client.publish(
      "home/living_room/sensor",
      jsonString.c_str()
    );

    Serial.print("Gui MQTT: ");
    Serial.println(jsonString);
  }
}