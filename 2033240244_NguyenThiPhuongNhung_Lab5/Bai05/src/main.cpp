
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// 1. THONG TIN WIFI

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// 2. THONG TIN MQTT BROKER

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// Topic gui du lieu cam bien
const char* topic_data =
  "nhung2033240244/esp32/data";

// Topic nhan lenh dieu khien
const char* topic_cmd =
  "nhung2033240244/esp32/cmd";

// 3. KHAI BAO CHAN ESP32

const int LED_PIN = 2;
const int SENSOR_PIN = 34;

// 4. KHOI TAO MQTT CLIENT

WiFiClient espClient;
PubSubClient client(espClient);

// 5. KHAI BAO THOI GIAN

unsigned long previousReconnect = 0;
const unsigned long reconnectInterval = 5000;

unsigned long previousPublish = 0;

// Mac dinh gui du lieu moi 5 giay
unsigned long publishInterval = 5000;

// =====================================
// HAM KET NOI WIFI
// =====================================

void connectWiFi() {

  Serial.println("Dang ket noi WiFi...");

  WiFi.begin(ssid, password);
}

// =====================================
// HAM CALLBACK - NHAN LENH MQTT
// =====================================

void callback(char* topic, byte* payload,
              unsigned int length) {

  String message = "";

  // Chuyen payload thanh chuoi
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  message.trim();

  Serial.println("-----------------------------");
  Serial.println("NHAN LENH MQTT THANH CONG!");

  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  Serial.println(message);

  // Chi xu ly lenh tu topic_cmd
  if (String(topic) != topic_cmd) {
    return;
  }

  // LENH BAT LED

  if (message == "ON") {

    digitalWrite(LED_PIN, HIGH);

    Serial.println("LED DA BAT!");
  }

  // LENH TAT LED

  else if (message == "OFF") {

    digitalWrite(LED_PIN, LOW);

    Serial.println("LED DA TAT!");
  }

  // LENH THAY DOI CHU KY GUI DU LIEU

  else if (message.startsWith("INTERVAL:")) {

    String value = message.substring(9);

    unsigned long newInterval = value.toInt();

    // Chi cho phep tu 1 den 60 giay
    if (newInterval >= 1000 &&
        newInterval <= 60000) {

      publishInterval = newInterval;

      // Bat dau tinh chu ky moi
      previousPublish = millis();

      Serial.print("Chu ky gui moi: ");
      Serial.print(publishInterval);
      Serial.println(" ms");

    } else {

      Serial.println("Chu ky khong hop le!");
    }
  }

  else {

    Serial.println("LENH KHONG HOP LE!");
  }
}

// =====================================
// HAM KET NOI MQTT
// =====================================

void connectMQTT() {

  Serial.println("Dang ket noi MQTT Broker...");

  // Client ID rieng cho Bai 5
  if (client.connect("ESP32_NHUNG_2033240244_BAI05")) {

    Serial.println("Ket noi MQTT thanh cong!");

    Serial.print("Broker: ");
    Serial.println(mqtt_server);

    Serial.print("Port: ");
    Serial.println(mqtt_port);

    // Dang ky nhan lenh dieu khien
    if (client.subscribe(topic_cmd)) {

      Serial.println("SUBSCRIBE THANH CONG!");

      Serial.print("Topic: ");
      Serial.println(topic_cmd);

    } else {

      Serial.println("SUBSCRIBE THAT BAI!");
    }

  } else {

    Serial.print("Ket noi MQTT that bai! Ma loi: ");
    Serial.println(client.state());
  }
}

// =====================================
// HAM GUI DU LIEU CAM BIEN
// =====================================

void publishSensorData() {

  // Doc gia tri ADC tu chan GPIO 34
  int sensorValue = analogRead(SENSOR_PIN);

  // Dong goi du lieu JSON
  String payload = "{";
  payload += "\"sensor\":\"light\",";
  payload += "\"adc\":";
  payload += String(sensorValue);
  payload += "}";

  // Gui du lieu len MQTT Broker
  bool success = client.publish(
    topic_data,
    payload.c_str()
  );

  if (success) {

    Serial.println("-----------------------------");
    Serial.println("GUI DU LIEU CAM BIEN THANH CONG!");

    Serial.print("Topic: ");
    Serial.println(topic_data);

    Serial.print("Payload: ");
    Serial.println(payload);

  } else {

    Serial.println("GUI DU LIEU THAT BAI!");
  }
}

// =====================================
// HAM SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 5 - MQTT PUBLISH & SUBSCRIBE");
  Serial.println("-----------------------------");

  // Cau hinh chan LED
  pinMode(LED_PIN, OUTPUT);

  // Ban dau LED tat
  digitalWrite(LED_PIN, LOW);

  // Cau hinh chan cam bien ADC
  pinMode(SENSOR_PIN, INPUT);

  // Cau hinh ADC 12 bit
  analogReadResolution(12);

  // Cau hinh WiFi
  WiFi.mode(WIFI_STA);

  connectWiFi();

  // Cai dat MQTT Broker
  client.setServer(mqtt_server, mqtt_port);

  // Dang ky ham Callback
  client.setCallback(callback);

  // Cho phep thu ket noi MQTT ngay khi WiFi san sang
  previousReconnect = millis() - reconnectInterval;
}

// =====================================
// HAM LOOP
// =====================================

void loop() {

  // 1. KIEM TRA KET NOI WIFI

  if (WiFi.status() != WL_CONNECTED) {

    if (millis() - previousReconnect >= reconnectInterval) {

      previousReconnect = millis();

      Serial.println("WiFi chua ket noi!");

      WiFi.reconnect();
    }

    return;
  }

  // 2. KIEM TRA KET NOI MQTT

  if (!client.connected()) {

    if (millis() - previousReconnect >= reconnectInterval) {

      previousReconnect = millis();

      connectMQTT();
    }

    return;
  }

  // 3. DUY TRI KET NOI MQTT

  client.loop();

  // 4. GUI DU LIEU CAM BIEN THEO CHU KY

  if (millis() - previousPublish >= publishInterval) {

    previousPublish = millis();

    publishSensorData();
  }
}