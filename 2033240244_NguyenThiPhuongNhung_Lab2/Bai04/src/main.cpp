#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT22

#define BUTTON 18
#define LED 5

DHT dht(DHTPIN, DHTTYPE);

bool ledState = false;

int buttonState = HIGH;
int lastButtonState = HIGH;

unsigned long buttonTime = 0;
unsigned long dhtTime = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
  dht.begin();

  Serial.println("-------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI04 - DHT22 + NUT NHAN");
  Serial.println("-------------------------------");
}

void loop() {

  unsigned long now = millis();

  int reading = digitalRead(BUTTON);
  if (reading != lastButtonState) {
    buttonTime = now;
  }

  if (now - buttonTime >= 50) {

    if (reading != buttonState) {

      buttonState = reading;
      if (buttonState == LOW) {

        ledState = !ledState;

        digitalWrite(LED, ledState);

        Serial.println("DA NHAN NUT");

        if (ledState) {
          Serial.println("LED BAT");
        } else {
          Serial.println("LED TAT");
        }
      }
    }
  }

  lastButtonState = reading;

  if (now - dhtTime >= 2000) {

    dhtTime = now;

    float nhietDo = dht.readTemperature();
    float doAm = dht.readHumidity();

    if (isnan(nhietDo) || isnan(doAm)) {
      Serial.println("LOI DOC DHT22");
    }
    else {
      Serial.print("Nhiet do: ");
      Serial.print(nhietDo);
      Serial.println(" C");

      Serial.print("Do am: ");
      Serial.print(doAm);
      Serial.println(" %");
    }
  }
}