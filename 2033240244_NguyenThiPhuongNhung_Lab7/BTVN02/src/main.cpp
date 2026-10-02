#define BLYNK_TEMPLATE_ID "TMPL6JeR8ESDy"
#define BLYNK_TEMPLATE_NAME "Smart Environment"
#define BLYNK_AUTH_TOKEN "bzkPXO7k-wNooo0R4Qf4sIXdYvQtb_ar"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// WiFi mặc định của Wokwi
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

// Dùng để tránh ghi Event liên tục
bool highTempLogged = false;

// Giá trị điều khiển
int ledState = 0;
int brightness = 0;


// =========================
// KHI BLYNK KẾT NỐI
// =========================
BLYNK_CONNECTED() {
  Serial.println("Da ket noi Blynk!");

  // V5 = trạng thái kết nối
  Blynk.virtualWrite(V5, 1);
}


// =========================
// NHẬN SWITCH TỪ BLYNK - V3
// =========================
BLYNK_WRITE(V3) {
  ledState = param.asInt();

  Serial.print("LED Switch: ");
  Serial.println(ledState ? "ON" : "OFF");
}


// =========================
// NHẬN SLIDER TỪ BLYNK - V4
// =========================
BLYNK_WRITE(V4) {
  brightness = param.asInt();

  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.println("%");
}


// =========================
// GỬI DỮ LIỆU CẢM BIẾN
// =========================
void sendSensorData() {

  // ==========================================
  // TEST BTVN 2:
  // Để 38.0 để kích hoạt cảnh báo > 35°C
  // ==========================================
  float temperature = 38.0;

  // Độ ẩm giả lập 40.0 - 80.9 %
  float humidity = random(400, 810) / 10.0;


  // Gửi dữ liệu lên Blynk
  Blynk.virtualWrite(V0, temperature); // Temperature
  Blynk.virtualWrite(V1, humidity);    // Humidity

  if (Blynk.connected()) {
    Blynk.virtualWrite(V5, 1);         // Connection Status
  }


  // ==========================================
  // GHI EVENT KHI NHIỆT ĐỘ > 35°C
  // ==========================================
  if (temperature > 35 && !highTempLogged) {

    Blynk.logEvent(
      "high_temperature",
      String("Nhiet do vuot nguong: ")
      + String(temperature, 1)
      + " C"
    );

    highTempLogged = true;

    Serial.println(">>> HIGH TEMPERATURE EVENT LOGGED!");
  }


  // Cho phép cảnh báo lại khi nhiệt độ
  // đã giảm xuống <= 35°C
  if (temperature <= 35) {
    highTempLogged = false;
  }


  // ==========================================
  // HIỂN THỊ SERIAL MONITOR
  // ==========================================
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | Connection: ");

  if (Blynk.connected()) {
    Serial.println("ONLINE");
  } else {
    Serial.println("OFFLINE");
  }
}


// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("Dang ket noi WiFi va Blynk...");

  randomSeed(micros());

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  // Gửi dữ liệu mỗi 2 giây
  timer.setInterval(2000L, sendSensorData);
}


// =========================
// LOOP
// =========================
void loop() {

  Blynk.run();
  timer.run();
}