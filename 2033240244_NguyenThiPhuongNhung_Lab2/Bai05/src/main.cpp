#include <Arduino.h>

#define LED1 2
#define LED2 5
#define BUTTON 18

bool led1State = LOW;
bool led2State = LOW;

bool buttonState = HIGH;
bool lastButtonState = HIGH;

unsigned long ledTime = 0;
unsigned long buttonTime = 0;
unsigned long loopTime = 0;

unsigned long loopCount = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("-------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI05 - 3 TAC VU");
  Serial.println("-------------------------------");
}

void loop() {
  unsigned long now = millis();

  loopCount++;

  // ===== TAC VU 1: LED1 =====
  if (now - ledTime >= 300) {
    ledTime = now;

    led1State = !led1State;
    digitalWrite(LED1, led1State);

    Serial.print("LED1: ");

    if (led1State == HIGH) {
      Serial.println("BAT");
    } else {
      Serial.println("TAT");
    }
  }

  // ===== TAC VU 2: NUT NHAN + LED2 =====
  int reading = digitalRead(BUTTON);

  if (reading != lastButtonState) {
    buttonTime = now;
  }

  if (now - buttonTime >= 50) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        led2State = !led2State;
        digitalWrite(LED2, led2State);

        Serial.println("DA NHAN NUT");

        Serial.print("LED2: ");

        if (led2State == HIGH) {
          Serial.println("BAT");
        } else {
          Serial.println("TAT");
        }
      }
    }
  }

  lastButtonState = reading;

  // ===== TAC VU 3: DEM LOOP =====
  if (now - loopTime >= 1000) {
    loopTime = now;

    Serial.println("----------------------------");

    Serial.print("Thoi gian: ");
    Serial.print(now / 1000);
    Serial.println(" giay");

    Serial.print("So vong loop: ");
    Serial.println(loopCount);

    Serial.print("Trang thai LED1: ");
    if (led1State == HIGH) {
      Serial.println("BAT");
    } else {
      Serial.println("TAT");
    }

    Serial.print("Trang thai LED2: ");
    if (led2State == HIGH) {
      Serial.println("BAT");
    } else {
      Serial.println("TAT");
    }

    Serial.println("----------------------------");

    loopCount = 0;
  }
}