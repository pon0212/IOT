#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#define LED_PIN 2

unsigned long lastMsg = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("--------------------------------");
  Serial.print("Nhan lenh: ");
  Serial.println(message);

  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.println("Loi parse JSON!");
    return;
  }

  String device = doc["device"];
  String state = doc["state"];

  if (device == "relay_1") {
    if (state == "ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED DA BAT");
    }
    else if (state == "OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED DA TAT");
    }
  }
}

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

      client.subscribe("home/living_room/control");

      Serial.println("Da subscribe:");
      Serial.println("home/living_room/control");
    }
    else {
      Serial.println("That bai");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("--------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 5 - MQTT PUB SUB DONG BO");
  Serial.println("--------------------------------");

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
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