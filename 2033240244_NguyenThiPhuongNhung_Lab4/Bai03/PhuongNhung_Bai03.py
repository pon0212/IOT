import requests

# URL Webhook.site
url = "https://webhook.site/d98c9e4f-0f67-42d5-9291-f06191cafe77"

# Du lieu cam bien
data = {
    "device_id": "sensor_01",
    "temperature": 28.5,
    "humidity": 75
}

# Gui du lieu len Webhook.site
response = requests.post(url, json=data)

# Hien thi ket qua
print("NGUYEN THI PHUONG NHUNG - 2033240244")
print("BAI 3 - GUI HTTP POST BANG PYTHON")
print("--------------------------------")

print("Du lieu da gui:", data)
print("Ma trang thai:", response.status_code)
print("Noi dung phan hoi:", response.text)