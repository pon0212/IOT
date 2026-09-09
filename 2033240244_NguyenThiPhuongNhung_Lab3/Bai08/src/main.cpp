#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// WiFi Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// NTP
const char* ntpServer = "time.google.com";

const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

// Moc thoi gian gan nhat
time_t lastEpoch = 0;
unsigned long lastMillis = 0;

bool daDongBo = false;
bool wifiTruocDo = false;


// -----------------------------
// Ket noi WiFi
// -----------------------------
void connectWiFi() {

  Serial.print("Dang ket noi WiFi");

  WiFi.begin(ssid, password);

  int count = 0;

  while (
    WiFi.status() != WL_CONNECTED &&
    count < 20
  ) {

    delay(500);
    Serial.print(".");
    count++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("Ket noi WiFi thanh cong!");

  } else {

    Serial.println("Khong ket noi duoc WiFi!");
  }
}


// -----------------------------
// Dong bo NTP
// -----------------------------
bool syncNTP() {

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  Serial.println("Dang dong bo NTP...");

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer
  );

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo, 5000)) {

    Serial.println("Dong bo NTP that bai!");

    return false;
  }

  lastEpoch = time(nullptr);
  lastMillis = millis();

  daDongBo = true;

  Serial.println("Dong bo NTP thanh cong!");

  return true;
}


// -----------------------------
// Lay gio khi mat mang
// -----------------------------
time_t getCurrentTime() {

  if (!daDongBo) {
    return 0;
  }

  unsigned long elapsed =
    (millis() - lastMillis) / 1000;

  return lastEpoch + elapsed;
}


void setup() {

  Serial.begin(115200);

  Serial.println("-----------------------------");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 8 - DONG HO NTP THONG MINH");
  Serial.println("-----------------------------");

  // OLED
  Wire.begin(21, 22);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {

    Serial.println("Khong tim thay OLED!");

    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Smart Clock");
  display.println("Dang ket noi WiFi...");
  display.display();

  // WiFi
  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {

    wifiTruocDo = true;

    syncNTP();
  }
}


void loop() {

  bool wifiHienTai =
    WiFi.status() == WL_CONNECTED;


  // WiFi bi mat
  if (!wifiHienTai && wifiTruocDo) {

    Serial.println("-----------------------------");
    Serial.println("MAT KET NOI WIFI!");
    Serial.println("Dong ho van chay bang millis()");
    Serial.println("-----------------------------");
  }


  // WiFi ket noi lai
  if (wifiHienTai && !wifiTruocDo) {

    Serial.println("-----------------------------");
    Serial.println("WIFI DA KET NOI LAI!");
    Serial.println("Dang dong bo lai NTP...");
    Serial.println("-----------------------------");

    syncNTP();
  }


  wifiTruocDo = wifiHienTai;


  // Neu mat WiFi thi thu ket noi lai
  if (!wifiHienTai) {

    WiFi.reconnect();
  }


  // Lay thoi gian
  time_t currentTime = getCurrentTime();


  // OLED
  display.clearDisplay();
  display.setCursor(0, 0);


  if (wifiHienTai) {

    display.println("WiFi: OK");

  } else {

    display.println("WiFi: MAT MANG!");
  }


  if (currentTime > 0) {

    struct tm timeinfo;

    localtime_r(
      &currentTime,
      &timeinfo
    );

    char ngay[20];
    char gio[20];

    sprintf(
      ngay,
      "%02d/%02d/%04d",
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1,
      timeinfo.tm_year + 1900
    );

    sprintf(
      gio,
      "%02d:%02d:%02d",
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec
    );


    display.println();
    display.print("Date: ");
    display.println(ngay);

    display.print("Time: ");
    display.println(gio);


    Serial.printf(
      "Ngay: %s | Gio: %s | WiFi: %s\r\n",
      ngay,
      gio,
      wifiHienTai ? "OK" : "MAT MANG"
    );

  } else {

    display.println();
    display.println("Chua co thoi gian");

    Serial.println("Chua dong bo duoc NTP!");
  }


  display.display();

  delay(1000);
}