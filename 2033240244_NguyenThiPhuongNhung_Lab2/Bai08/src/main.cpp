#include <Arduino.h>

#define LED_PWM 5
#define LED_POWER 2
#define BUTTON 18

#define PWM_CHANNEL 0

int brightness[] = {0, 64, 128, 191, 255};
int level = 0;


bool powerState = LOW;


bool lastButton = HIGH;

unsigned long buttonTime = 0;
unsigned long ledTime = 0;

void setup() {
  pinMode(LED_POWER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);


  ledcSetup(PWM_CHANNEL, 5000, 8);
  ledcAttachPin(LED_PWM, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 0);

  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244"); 
  Serial.println("BAI 8 - DIEU KHIEN DO SANG");
  Serial.println("-----------------------------");
  Serial.println("Do sang LED: 0%");
}

void loop() {
  unsigned long now = millis();

  // ===== NUT NHAN =====
  bool button = digitalRead(BUTTON);

  // Phat hien luc vua nhan nut
  if (button == LOW && lastButton == HIGH) {

    // Chong rung 50ms
    if (now - buttonTime >= 50) {
      buttonTime = now;

      level++;

      if (level > 4) {
        level = 0;
      }

      ledcWrite(PWM_CHANNEL, brightness[level]);

      Serial.print("Do sang LED: ");
      Serial.print(level * 25);
      Serial.println("%");
    }
  }

  lastButton = button;


  // ===== LED BAO NGUON =====
  if (now - ledTime >= 750) {
    ledTime = now;

    powerState = !powerState;
    digitalWrite(LED_POWER, powerState);
  }
}