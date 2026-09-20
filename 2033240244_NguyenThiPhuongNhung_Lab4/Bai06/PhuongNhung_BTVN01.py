
import requests
import random
import time
from datetime import datetime

# URL Webhook.site
url = "https://webhook.site/d98c9e4f-0f67-42d5-9291-f06191cafe77"

print("NGUYEN THI PHUONG NHUNG - 2033240244")
print("BAI 6 - GUI DU LIEU CAM BIEN DINH KY")
print("--------------------------------")

try:
    while True:
        # Tao du lieu cam bien ngau nhien
        data = {
            "device_id": "sensor_01",
            "temperature": round(random.uniform(20, 40), 2),
            "humidity": round(random.uniform(50, 90), 2),
            "timestamp": datetime.now().isoformat(timespec="seconds")
        }

        # Gui du lieu len Webhook.site
        try:
            response = requests.post(url, json=data, timeout=3)

            print("\nThoi gian gui:", data["timestamp"])
            print("Du lieu da gui:", data)
            print("Ma trang thai:", response.status_code)

            if response.ok:
                print("Gui du lieu thanh cong!")
            else:
                print("Server tra ve ma loi:", response.status_code)

        except requests.exceptions.RequestException as e:
            print("LOI GUI DU LIEU:", e)

        # Cho 10 giay truoc lan gui tiep theo
        print("Cho 10 giay de gui lan tiep theo...")
        time.sleep(10)

except KeyboardInterrupt:
    print("\nDa dung chuong trinh an toan.")