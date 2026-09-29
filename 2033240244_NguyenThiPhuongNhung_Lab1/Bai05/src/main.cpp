#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Khởi tạo LCD 16x2 với địa chỉ I2C 0x27, 16 cột và 2 dòng
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Quản lý thời gian cập nhật màn hình LCD
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 200; // Cập nhật mỗi 200ms giúp hiển thị mượt mà

void setup() {
  // Khởi động Serial Monitor
  Serial.begin(115200);

  // Khởi tạo màn hình LCD và bật đèn nền
  lcd.init();
  lcd.backlight();

  // In dòng 1 cố định
  lcd.setCursor(0, 0);
  lcd.print("Thoi gian chay:");

  Serial.println("==================================================");
  Serial.println("NGUYEN THI PHUONG NHUNG - 2033240244");
  Serial.println("BAI 05: BO DEM THOI GIAN (UPTIME) TREN LCD 16X2");
  Serial.println("==================================================");
}

void loop() {
  unsigned long currentMillis = millis();

  // Cập nhật LCD định kỳ không dùng delay() chặn chương trình
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    // Tính thời gian đã chạy tính theo giây từ hàm millis()
    unsigned long seconds = currentMillis / 1000;

    // Hiển thị ở dòng 2 (cột 0, dòng 1)
    lcd.setCursor(0, 1);
    lcd.print(seconds);
    lcd.print(" giay   "); // Thêm khoảng trắng đệm phía sau để xóa sạch ký tự thừa cũ khi số chữ số thay đổi

    // In thông tin ra Serial Monitor
    Serial.print("Thoi gian chay: ");
    Serial.print(seconds);
    Serial.println(" giay");
  }
}
