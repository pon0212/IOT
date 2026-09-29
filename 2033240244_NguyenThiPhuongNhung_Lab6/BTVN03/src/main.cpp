#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const int LED_PIN = 2;
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* MQTT_SERVER = "broker.hivemq.com";
const char* CMD_TOPIC = "home/device/cmd";
const char* STATUS_TOPIC = "home/device/status";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMqttAttempt = 0;
const unsigned long RETRY_INTERVAL = 3000;

void callback(char* topic, byte* payload, unsigned int length) {
  JsonDocument command;
  if (deserializeJson(command, payload, length)) {
    Serial.println("JSON khong hop le");
    return;
  }

  String device = command["device"] | "";
  String state = command["state"] | "";

  if (device != "light" || (state != "ON" && state != "OFF")) {
    Serial.println("Lenh khong hop le");
    return;
  }

  // Thuc thi lenh truoc khi gui ACK.
  bool isOn = (state == "ON");
  digitalWrite(LED_PIN, isOn ? HIGH : LOW);
  Serial.println(isOn ? "LED DA BAT" : "LED DA TAT");

  JsonDocument ack;
  ack["device"] = "light";
  ack["status"] = isOn ? "ON" : "OFF";
  ack["timestamp"] = millis();
  ack["message"] = "Execution successful";

  char response[200];
  serializeJson(ack, response, sizeof(response));

  if (client.publish(STATUS_TOPIC, response)) {
    Serial.print("ACK: ");
    Serial.println(response);
  } else {
    Serial.println("Gui ACK that bai");
  }
}

void connectMQTT() {
  String clientId = "ESP32-BTVN03-";
  clientId += WiFi.macAddress();

  Serial.println("Dang ket noi MQTT...");
  if (client.connect(clientId.c_str())) {
    if (client.subscribe(CMD_TOPIC)) {
      Serial.println("Da subscribe: home/device/cmd");
    } else {
      Serial.println("Subscribe that bai");
      client.disconnect();
    }
  } else {
    Serial.print("Ket noi MQTT that bai, rc=");
    Serial.println(client.state());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("--------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BTVN 2 - DIEU KHIEN DA THIET BI");
  Serial.println("--------------------------------");
  
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(SSID, PASSWORD);

  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi WiFi");

  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  client.setBufferSize(512);

  connectMQTT();
  lastMqttAttempt = millis();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    if (millis() - lastMqttAttempt >= RETRY_INTERVAL) {
      lastMqttAttempt = millis();
      connectMQTT();
    }
  }

  client.loop();
}

