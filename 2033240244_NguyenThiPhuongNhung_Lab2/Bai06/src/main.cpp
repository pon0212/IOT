#include <Arduino.h> 
 
#define RED_LED 25 
#define YELLOW_LED 26 
#define GREEN_LED 27 
 
int state = 0; 
unsigned long previousTime = 0; 
 
void setup() { 
  pinMode(RED_LED, OUTPUT); 
  pinMode(YELLOW_LED, OUTPUT); 
  pinMode(GREEN_LED, OUTPUT); 
 
  Serial.begin(115200); 
   
  Serial.println("---------------------------------------"); 
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244"); 
  Serial.println("BAI 6 - DEN GIAO THONG"); 
  Serial.println("---------------------------------------"); 

  Serial.println("DEN DO - 5 GIAY");
} 
 
void loop() { 
  unsigned long now = millis(); 
 
  switch (state) { 
 
    case 0: // DEN DO 
      digitalWrite(RED_LED, HIGH); 
      digitalWrite(YELLOW_LED, LOW); 
      digitalWrite(GREEN_LED, LOW); 
 
      if (now - previousTime >= 5000) { 
        previousTime = now; 
        state = 1; 
 
        Serial.println("DEN XANH - 4 GIAY"); 
      } 
      break; 
 
    case 1: // DEN XANH 
      digitalWrite(RED_LED, LOW); 
      digitalWrite(YELLOW_LED, LOW); 
      digitalWrite(GREEN_LED, HIGH); 
 
      if (now - previousTime >= 4000) { 
        previousTime = now; 
        state = 2; 
 
        Serial.println("DEN VANG - 2 GIAY"); 
      } 
      break; 
 
    case 2: // DEN VANG 
      digitalWrite(RED_LED, LOW); 
      digitalWrite(YELLOW_LED, HIGH); 
      digitalWrite(GREEN_LED, LOW); 
 
      if (now - previousTime >= 2000) { 
        previousTime = now; 
        state = 0; 
 
        Serial.println("DEN DO - 5 GIAY"); 
      } 
      break; 
  } 
}