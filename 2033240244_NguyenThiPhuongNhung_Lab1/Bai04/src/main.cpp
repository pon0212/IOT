#include <Arduino.h>
#include "DHT.h"

// Định nghĩa chân kết nối
#define DHTPIN 15
#define DHTTYPE DHT22
#define LED_PIN 25

// Khởi tạo đối tượng DHT
DHT dht(DHTPIN, DHTTYPE);

// Quản lý thời gian đọc cảm biến
unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000; // Đọc nhiệt độ mỗi 2 giây

// Quản lý trạng thái và nhấp nháy LED
bool isWarning = false;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 300; // Nhấp nháy LED mỗi 300ms
bool ledState = false;

void setup() {
  // Cấu hình chân LED là OUTPUT
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Khởi động Serial Monitor
  Serial.begin(115200);

  // Khởi động cảm biến DHT22
  dht.begin();

  Serial.println("==================================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 04: CANH BAO NHIET DO QUA NGUONG (DHT22 + LED)");
  Serial.println("==================================================");
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Đọc dữ liệu từ DHT22 định kỳ mỗi 2 giây (tránh spam Terminal)
  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;

    float temperature = dht.readTemperature();

    // Xử lý khi cảm biến đọc lỗi
    if (isnan(temperature)) {
      Serial.println("Loi: Khong the doc du lieu tu cam bien DHT22!");
      isWarning = false;
    } else {
      // In nhiệt độ ra Serial Monitor
      Serial.print("Nhiet do: ");
      Serial.print(temperature, 1);
      Serial.println(" C");

      // Kiểm tra ngưỡng nhiệt độ 30°C
      if (temperature > 30.0) {
        isWarning = true;
        Serial.println("CANH BAO: NHIET DO QUA 30 C");
      } else {
        isWarning = false;
      }
    }
  }

  // 2. Điều khiển LED theo trạng thái cảnh báo
  if (isWarning) {
    // Nếu nhiệt độ > 30°C: LED nhấp nháy liên tục
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } else {
    // Nếu nhiệt độ <= 30°C hoặc đọc lỗi: LED tắt
    ledState = false;
    digitalWrite(LED_PIN, LOW);
  }
}
