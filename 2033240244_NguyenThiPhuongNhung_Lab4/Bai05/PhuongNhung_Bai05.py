
import requests

# URL Webhook.site
url = "https://webhook.site/d98c9e4f-0f67-42d5-9291-f06191cafe77"

# Du lieu cam bien
data = {
    "device_id": "sensor_01",
    "temperature": 28.5,
    "humidity": 75
}

# Thong tin sinh vien
print("NGUYEN THI PHUONG NHUNG - 2033240244")
print("BAI 5 - XU LY NGOAI LE VA TIMEOUT")
print("--------------------------------")

# Gui HTTP POST va xu ly loi
try:
    response = requests.post(url, json=data, timeout=3)

    print("Du lieu da gui:", data)
    print("Ma trang thai:", response.status_code)
    print("Noi dung phan hoi:", response.text)

except requests.exceptions.Timeout:
    print("LOI: Server phan hoi qua cham!")

except requests.exceptions.ConnectionError:
    print("LOI: Khong the ket noi den server!")

except requests.exceptions.RequestException as e:
    print("LOI KHAC:", e)