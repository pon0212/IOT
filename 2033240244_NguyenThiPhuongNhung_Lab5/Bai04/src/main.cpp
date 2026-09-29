
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Thong tin WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Thong tin MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Topic nhan lenh dieu khien LED
const char* mqtt_topic =
  "nhung2033240244/esp32/control/led";

// Chan LED
const int LED_PIN = 2;

// Tao doi tuong MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Thoi gian thu ket noi lai
unsigned long previousReconnect = 0;
const unsigned long reconnectInterval = 5000;

// Ham ket noi WiFi
void connectWiFi() {

  Serial.println("Dang ket noi WiFi...");

  WiFi.begin(ssid, password);
}

// Ham xu ly tin nhan MQTT
void callback(char* topic, byte* payload,
              unsigned int length) {

  String message = "";

  // Chuyen du lieu nhan duoc thanh chuoi
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  message.trim();

  Serial.println("-----------------------------");
  Serial.println("NHAN DU LIEU MQTT THANH CONG!");

  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  Serial.println(message);

  // Kiem tra topic dieu khien LED
  if (String(topic) == mqtt_topic) {

    // Nhan lenh ON
    if (message == "ON") {

      digitalWrite(LED_PIN, HIGH);

      Serial.println("LED DA BAT!");
    }

    // Nhan lenh OFF
    else if (message == "OFF") {

      digitalWrite(LED_PIN, LOW);

      Serial.println("LED DA TAT!");
    }

    else {

      Serial.println("LENH KHONG HOP LE!");
    }
  }
}

// Ham ket noi MQTT
void connectMQTT() {

  Serial.println("Dang ket noi MQTT Broker...");

  // Client ID rieng cho Bai 4
  if (client.connect("ESP32_NHUNG_2033240244_BAI04")) {

    Serial.println("Ket noi MQTT thanh cong!");

    Serial.print("Broker: ");
    Serial.println(mqtt_server);

    Serial.print("Port: ");
    Serial.println(mqtt_port);

    // Dang ky nhan lenh dieu khien LED
    if (client.subscribe(mqtt_topic)) {

      Serial.println("SUBSCRIBE THANH CONG!");

      Serial.print("Topic: ");
      Serial.println(mqtt_topic);
    }

    else {

      Serial.println("SUBSCRIBE THAT BAI!");
    }
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
  Serial.println("BAI 4 - MQTT SUBSCRIBE & CALLBACK");
  Serial.println("-----------------------------");

  // Cau hinh chan LED
  pinMode(LED_PIN, OUTPUT);

  // Ban dau LED tat
  digitalWrite(LED_PIN, LOW);

  // Cau hinh WiFi
  WiFi.mode(WIFI_STA);

  // Ket noi WiFi
  connectWiFi();

  // Cai dat MQTT Broker
  client.setServer(mqtt_server, mqtt_port);

  // Dang ky ham Callback
  client.setCallback(callback);

  // Thu ket noi MQTT ngay sau khi WiFi san sang
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
}