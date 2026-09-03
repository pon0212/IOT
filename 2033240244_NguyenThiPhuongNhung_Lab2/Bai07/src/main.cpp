#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT22
#define BUZZER 15

DHT dht(DHTPIN, DHTTYPE);

unsigned long dhtTime = 0;
unsigned long buzzerTime = 0;

bool warning = false;
bool buzzerState = false;

void setup() {
  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);
  dht.begin();

  Serial.println("------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244"); 
  Serial.println("BAI 7 - CANH BAO NHIET DO");
  Serial.println("------------------------------");
}

void loop() {
  unsigned long now = millis();

  if (now - dhtTime >= 2000) {
    dhtTime = now;

    float nhietDo = dht.readTemperature();

    if (isnan(nhietDo)) {
      Serial.println("LOI DOC DHT22");
    }
    else {
      Serial.print("Nhiet do: ");
      Serial.print(nhietDo);
      Serial.println(" C");

      if (nhietDo > 30) {
        warning = true;
        Serial.println("CANH BAO: NHIET DO CAO!");
      }

   
      else {
        if (warning == true) {
          noTone(BUZZER);
        }

        warning = false;
        buzzerState = false;

        Serial.println("Nhiet do binh thuong");
      }
    }
  }

  if (warning == true) {

    if (now - buzzerTime >= 200) {
      buzzerTime = now;

      buzzerState = !buzzerState;

      if (buzzerState == true) {
        tone(BUZZER, 1000);   // Keu
      }
      else {
        noTone(BUZZER);       // Tat
      }
    }
  }
}