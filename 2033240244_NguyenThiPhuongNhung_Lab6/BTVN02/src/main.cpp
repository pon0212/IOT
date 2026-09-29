#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// =======================
// WiFi
// =======================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// =======================
// MQTT
// =======================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// =======================
// Thiet bi
// =======================
#define LIGHT_PIN 2
#define FAN_PIN 4

bool lightState = false;
int fanSpeed = 0;

// =======================
// Callback MQTT
// =======================
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("--------------------------------");
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("JSON nhan duoc: ");
  Serial.println(message);

  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.println("Loi parse JSON!");
    return;
  }

  String target = doc["target"];

  // =======================
  // Dieu khien DEN
  // =======================
  if (target == "light") {

    String action = doc["action"];

    if (action == "toggle") {
      lightState = !lightState;

      digitalWrite(LIGHT_PIN, lightState ? HIGH : LOW);

      if (lightState) {
        Serial.println("DEN DA BAT");
      } else {
        Serial.println("DEN DA TAT");
      }
    }
  }

  // =======================
  // Dieu khien QUAT
  // =======================
  else if (target == "fan") {

    fanSpeed = doc["speed"];

    if (fanSpeed > 0) {
      digitalWrite(FAN_PIN, HIGH);

      Serial.print("QUAT DA BAT - SPEED: ");
      Serial.println(fanSpeed);
    }
    else {
      digitalWrite(FAN_PIN, LOW);

      Serial.println("QUAT DA TAT");
    }
  }

  Serial.println("--------------------------------");
}

// =======================
// Ket noi WiFi
// =======================
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

// =======================
// Ket noi MQTT
// =======================
void connectMQTT() {
  while (!client.connected()) {

    Serial.print("Dang ket noi MQTT...");

    String clientId = "ESP32-BTVN2-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("Thanh cong");

      client.subscribe("home/device/control");

      Serial.println("Da subscribe:");
      Serial.println("home/device/control");
    }
    else {
      Serial.println("That bai, thu lai...");
      delay(2000);
    }
  }
}

// =======================
// Setup
// =======================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  Serial.println("--------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BTVN 2 - DIEU KHIEN DA THIET BI");
  Serial.println("--------------------------------");

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// =======================
// Loop
// =======================
void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
}