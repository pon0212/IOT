#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

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
// DHT22
// =======================
#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// =======================
// LDR
// =======================
#define LDR_PIN 34

unsigned long lastMsg = 0;

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

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}

// =======================
// Ket noi MQTT
// =======================
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT...");

    String clientId = "ESP32-BTVN1-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Thanh cong");
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

  Serial.println("--------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BTVN 1 - TRAM GIAM SAT MOI TRUONG");
  Serial.println("--------------------------------");

  dht.begin();

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
}

// =======================
// Loop
// =======================
void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();

  // Gui du lieu moi 10 giay
  if (millis() - lastMsg >= 10000) {
    lastMsg = millis();

    // Doc DHT22
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Doc LDR
    int light = analogRead(LDR_PIN);

    // Doc cuong do WiFi
    int rssi = WiFi.RSSI();

    // Kiem tra DHT
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Khong doc duoc DHT22!");
      return;
    }

    // Tao JSON
    StaticJsonDocument<256> doc;

    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["light"] = light;
    doc["rssi"] = rssi;

    String jsonString;
    serializeJson(doc, jsonString);

    // Publish MQTT
    client.publish(
      "vnhome/sensor/data",
      jsonString.c_str()
    );

    Serial.println("--------------------------------");
    Serial.println("Gui du lieu MQTT:");
    Serial.println(jsonString);
    Serial.println("--------------------------------");
  }
}