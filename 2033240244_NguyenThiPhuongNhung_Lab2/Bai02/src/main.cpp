#include <Arduino.h>

const int button = 18;
const int led = 5;

int trangThaiLED = LOW;
int trangThaiNutHienTai = HIGH; 
int giaTriDocTruoc = HIGH;      

unsigned long thoiGianCu = 0;
const unsigned long thoiGianChongRung = 50; 

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  digitalWrite(led, trangThaiLED);

  Serial.begin(115200);

  Serial.println("-------------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI02 - DIEU KHIEN LED BANG NUT NHAN");
  Serial.println("-------------------------------");
}

void loop() {
  int giaTriDoc = digitalRead(button);

  if (giaTriDoc != giaTriDocTruoc) {
    thoiGianCu = millis();       
    giaTriDocTruoc = giaTriDoc;  
  }

  if ((millis() - thoiGianCu) >= thoiGianChongRung) {
    
    if (giaTriDoc != trangThaiNutHienTai) {
      trangThaiNutHienTai = giaTriDoc; 

      if (trangThaiNutHienTai == LOW) {
        trangThaiLED = !trangThaiLED;      
        digitalWrite(led, trangThaiLED);   

        Serial.println("Da nhan nut");
        if (trangThaiLED == HIGH) {
          Serial.println("LED BAT");
        } else {
          Serial.println("LED TAT");
        }
      }
    }
  }
}