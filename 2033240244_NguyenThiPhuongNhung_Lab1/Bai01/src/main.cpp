#include <Arduino.h>

const int ledXanh = 25;
const int ledVang = 26;
const int ledDo = 27;

void setup() {
  pinMode(ledXanh, OUTPUT);
  pinMode(ledVang, OUTPUT);
  pinMode(ledDo, OUTPUT);

  Serial.begin(115200);
  Serial.println("==================================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 1 - DEN GIAO THONG");
  Serial.println("==================================================");
}

void loop() {
  // ĐÈN XANH
  digitalWrite(ledXanh, HIGH);
  digitalWrite(ledVang, LOW);
  digitalWrite(ledDo, LOW);

  Serial.println("DEN XANH SANG - 3 GIAY");
  delay(3000);

  // ĐÈN VÀNG
  digitalWrite(ledXanh, LOW);
  digitalWrite(ledVang, HIGH);
  digitalWrite(ledDo, LOW);

  Serial.println("DEN VANG SANG - 1 GIAY");
  delay(1000);

  // ĐÈN ĐỎ
  digitalWrite(ledXanh, LOW);
  digitalWrite(ledVang, LOW);
  digitalWrite(ledDo, HIGH);

  Serial.println("DEN DO SANG - 3 GIAY");
  delay(3000);
}