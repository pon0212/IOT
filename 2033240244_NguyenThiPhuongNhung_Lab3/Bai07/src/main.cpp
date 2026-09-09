#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* ntpServer = "time.google.com";

// Viet Nam UTC+7
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

// 6 tieng
const unsigned long syncInterval = 6UL * 60UL * 60UL * 1000UL;

unsigned long previousSync = 0;

// ----------------------------
// Ket noi WiFi
// ----------------------------
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Dang ket noi WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Ket noi WiFi thanh cong!");
}

// ----------------------------
// In thoi gian hien tai
// ----------------------------
void printTime() {
  struct tm timeinfo;

  if (getLocalTime(&timeinfo, 5000)) {
    Serial.printf(
      "%02d/%02d/%04d - %02d:%02d:%02d\r\n",
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1,
      timeinfo.tm_year + 1900,
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec
    );
  }
  else {
    Serial.println("Khong lay duoc thoi gian!");
  }
}

// ----------------------------
// Dong bo NTP
// ----------------------------
void syncNTP() {

  connectWiFi();

  Serial.println("-----------------------------");
  Serial.println("Bat dau dong bo NTP");

  Serial.print("Thoi gian truoc dong bo: ");
  printTime();

  time_t beforeSync = time(nullptr);

  // Goi lai NTP Server
  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer
  );

  struct tm timeinfo;

  Serial.print("Dang dong bo");

  int count = 0;

  while (!getLocalTime(&timeinfo, 1000) && count < 10) {
    Serial.print(".");
    count++;
  }

  Serial.println();

  time_t afterSync = time(nullptr);

  Serial.print("Thoi gian sau dong bo: ");
  printTime();

  long difference = (long)difftime(afterSync, beforeSync);

  Serial.print("Chenh lech ghi nhan: ");
  Serial.print(difference);
  Serial.println(" giay");

  Serial.println("Dong bo NTP hoan tat!");
  Serial.println("-----------------------------");
}

void setup() {
  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 7 - TU DONG DONG BO NTP DINH KY");
  Serial.println("-----------------------------");

  connectWiFi();

  // Dong bo lan dau
  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer
  );

  Serial.print("Dang lay thoi gian NTP");

  struct tm timeinfo;

  while (!getLocalTime(&timeinfo, 1000)) {
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Dong bo lan dau thanh cong!");

  printTime();

  previousSync = millis();
}

void loop() {

  // Neu mat WiFi thi ket noi lai
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  // Moi 6 tieng dong bo lai NTP
  if (millis() - previousSync >= syncInterval) {

    previousSync = millis();

    syncNTP();
  }

  delay(100);
}