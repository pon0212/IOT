#include <Arduino.h>

int led1 = 2;
int led2 = 4;

unsigned long tg1 = 0;
unsigned long tg2 = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Serial.begin(115200);

  Serial.println("-------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI01 - LED DOC LAP");
  Serial.println("-------------------------------");

}

void loop() {

  // LED 1
  if (millis() - tg1 >= 500) {
    tg1 = millis();

    digitalWrite(led1, !digitalRead(led1));

    if (digitalRead(led1) == HIGH) {
      Serial.println("LED 1: BAT");
    } else {
      Serial.println("LED 1: TAT");
    }
  }

  // LED 2
  if (millis() - tg2 >= 1200) {
    tg2 = millis();

    digitalWrite(led2, !digitalRead(led2));

    if (digitalRead(led2) == HIGH) {
      Serial.println("LED 2: BAT");
    } else {
      Serial.println("LED 2: TAT");
    }
  }
}