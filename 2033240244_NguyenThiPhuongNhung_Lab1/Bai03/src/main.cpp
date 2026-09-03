
#include <Arduino.h>

const int buttonPin = 14;
const int ledPin = 25;

bool ledState = false;

int buttonState;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Serial.println("==================================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 3 - CONG TAC THONG MINH");
  Serial.println("==================================================");
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Phát hiện nút vừa thay đổi trạng thái
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Chờ tín hiệu ổn định 50ms (chống dội nút)
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Khi phát hiện sự kiện nhấn nút (LOW vì dùng INPUT_PULLUP)
      if (buttonState == LOW) {
        // Đảo trạng thái LED
        ledState = !ledState;
        digitalWrite(ledPin, ledState ? HIGH : LOW);

        if (ledState) {
          Serial.println("DEN SANG");
        } else {
          Serial.println("DEN TAT");
        }
      }
    }
  }

  lastButtonState = reading;
}