
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <DHT.h>

// Cau hinh cam bien DHT22
#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// WiFi cua Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// URL Webhook.site
const char* serverURL =
    "https://webhook.site/d98c9e4f-0f67-42d5-9291-f06191cafe77";

// Thoi gian gui du lieu
unsigned long previousMillis = 0;
const unsigned long interval = 10000;

// Ham ket noi WiFi
void connectWiFi() {

    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    Serial.println("Dang ket noi WiFi...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Ket noi WiFi thanh cong!");

    Serial.print("Dia chi IP: ");
    Serial.println(WiFi.localIP());
}

// Ham doc cam bien va gui du lieu
void sendSensorData() {

    // Kiem tra ket noi WiFi
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    // Doc du lieu DHT22
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Kiem tra loi cam bien
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("LOI: Khong doc duoc DHT22!");
        return;
    }

    // Hien thi du lieu cam bien
    Serial.println();
    Serial.println("-----------------------------");
    Serial.println("DU LIEU CAM BIEN DHT22");

    Serial.print("Nhiet do: ");
    Serial.print(temperature);
    Serial.println(" *C");

    Serial.print("Do am: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Tao du lieu JSON
    String jsonData = "{";
    jsonData += "\"device_id\":\"ESP32_01\",";
    jsonData += "\"temperature\":" + String(temperature, 2) + ",";
    jsonData += "\"humidity\":" + String(humidity, 2);
    jsonData += "}";

    Serial.print("JSON: ");
    Serial.println(jsonData);

    // Tao ket noi HTTPS
    WiFiClientSecure client;

    // Bo qua kiem tra chung chi HTTPS
    // Chi su dung de thu nghiem tren Wokwi
    client.setInsecure();

    HTTPClient http;

    // Ket noi den Webhook.site
    if (!http.begin(client, serverURL)) {
        Serial.println("LOI: Khong khoi tao duoc HTTP!");
        return;
    }

    // Thiet lap Header JSON
    http.addHeader("Content-Type", "application/json");

    // Gioi han thoi gian cho phan hoi
    http.setTimeout(10000);

    Serial.println("Dang gui du lieu len Webhook.site...");

    // Gui HTTP POST
    int httpCode = http.POST(jsonData);

    // Kiem tra ket qua
    if (httpCode > 0) {

        Serial.print("Ma trang thai HTTP: ");
        Serial.println(httpCode);

        if (httpCode == 200) {
            Serial.println("GUI DU LIEU THANH CONG!");
        } else {
            Serial.println("Server tra ve ma trang thai khac 200.");
        }

        Serial.print("Phan hoi tu server: ");
        Serial.println(http.getString());

    } else {

        Serial.print("LOI GUI DU LIEU: ");
        Serial.println(http.errorToString(httpCode));
    }

    // Dong ket noi HTTP
    http.end();
}

// Ham khoi tao
void setup() {

    Serial.begin(115200);

    Serial.println();
    Serial.println("-----------------------------");
    Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
    Serial.println("BAI 8 - ESP32 GUI DU LIEU CAM BIEN");
    Serial.println("-----------------------------");

    // Khoi tao cam bien
    dht.begin();

    // Ket noi WiFi
    connectWiFi();

    Serial.println("He thong da san sang!");
}

// Ham chay lien tuc
void loop() {

    unsigned long currentMillis = millis();

    // Gui du lieu lan dau va lap lai moi 10 giay
    if (previousMillis == 0 ||
        currentMillis - previousMillis >= interval) {

        previousMillis = currentMillis;

        sendSensorData();
    }
}