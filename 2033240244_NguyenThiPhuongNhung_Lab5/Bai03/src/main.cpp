
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Thong tin WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Thong tin MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Topic gui du lieu
const char* mqtt_topic =
  "nhung2033240244/esp32/sensor/temperature";

// Tao doi tuong MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Thoi gian gui du lieu
unsigned long previousMillis = 0;
const unsigned long interval = 5000;

// Thoi gian thu ket noi lai
unsigned long previousReconnect = 0;
const unsigned long reconnectInterval = 5000;

// Ham ket noi WiFi
void connectWiFi() {
  Serial.println("Dang ket noi WiFi...");
  WiFi.begin(ssid, password);
}

// Ham ket noi MQTT
void connectMQTT() {
  Serial.println("Dang ket noi MQTT Broker...");

  // ID phai khac voi cac ESP32 dang ket noi cung broker.
  if (client.connect("ESP32_NHUNG_2033240244_BAI03")) {
    Serial.println("Ket noi MQTT thanh cong!");

    Serial.print("Broker: ");
    Serial.println(mqtt_server);

    Serial.print("Port: ");
    Serial.println(mqtt_port);
  }
  else {
    Serial.print("Ket noi MQTT that bai! Ma loi: ");
    Serial.println(client.state());
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 3 - GUI DU LIEU MQTT PUBLISH");
  Serial.println("-----------------------------");

  WiFi.mode(WIFI_STA);
  connectWiFi();

  // Cai dat MQTT Broker
  client.setServer(mqtt_server, mqtt_port);

  // Thu ket noi MQTT ngay sau khi WiFi san sang.
  previousReconnect = millis() - reconnectInterval;
}

void loop() {

  // Kiem tra ket noi WiFi
  if (WiFi.status() != WL_CONNECTED) {

    if (millis() - previousReconnect >= reconnectInterval) {
      previousReconnect = millis();

      Serial.println("WiFi bi mat ket noi!");
      WiFi.reconnect();
    }

    return;
  }

  // Kiem tra ket noi MQTT
  if (!client.connected()) {

    if (millis() - previousReconnect >= reconnectInterval) {
      previousReconnect = millis();
      connectMQTT();
    }

    return;
  }

  // Duy tri ket noi MQTT
  client.loop();

  // Gui du lieu moi 5 giay
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    const char* message = "Temperature: 28.5C";

    bool result = client.publish(mqtt_topic, message);

    if (result) {
      Serial.println("-----------------------------");
      Serial.println("GUI DU LIEU MQTT THANH CONG!");

      Serial.print("Topic: ");
      Serial.println(mqtt_topic);

      Serial.print("Message: ");
      Serial.println(message);
    }
    else {
      Serial.println("GUI DU LIEU MQTT THAT BAI!");
    }
  }
}
