#include <Arduino.h>

const int buttonPin = 14;
const int ledPin = 25;

int trangThaiCu = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  
  Serial.println("==================================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 2 - CONG TAC NHAN PHAT XUYEN TANG");
  Serial.println("==================================================");
}

void loop() {
  int trangThaiHienTai = digitalRead(buttonPin);

  // Vừa nhấn nút
  if (trangThaiCu == HIGH && trangThaiHienTai == LOW) {
    digitalWrite(ledPin, HIGH);
    Serial.println("DEN SANG");
  }

  // Vừa nhả nút
  if (trangThaiCu == LOW && trangThaiHienTai == HIGH) {
    digitalWrite(ledPin, LOW);
    Serial.println("DEN TAT");
  }

  // Lưu trạng thái hiện tại
  trangThaiCu = trangThaiHienTai;

  delay(50);
}