#include <Arduino.h>

int led = 2;

unsigned long thoiGianLED = 0;
unsigned long thoiGianSerial = 0;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  Serial.println("-------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI03 - GIAO TIEP SERIAL");
  Serial.println("-------------------------------");
}

void loop() {

  // LED mỗi 1 giây
  if (millis() - thoiGianLED >= 1000) 
  {
    thoiGianLED = millis();

    digitalWrite(led, !digitalRead(led));

    Serial.println("LED ");
  }

  // Serial mỗi 3 giây
  if (millis() - thoiGianSerial >= 3000) 
  {
    thoiGianSerial = millis();

    Serial.print("He thong dang chay - Thoi gian: ");
    Serial.print(millis());
    Serial.println(" ms");
  }
}