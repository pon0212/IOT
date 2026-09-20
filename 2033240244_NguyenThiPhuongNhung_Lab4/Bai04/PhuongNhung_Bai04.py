
import requests

# URL Webhook.site
url = "https://webhook.site/d98c9e4f-0f67-42d5-9291-f06191cafe77"

# Du lieu cam bien
data = {
    "device_id": "sensor_01",
    "temperature": 28.5,
    "humidity": 75
}

# Header chua API Key
headers = {
    "Content-Type": "application/json",
    "X-API-Key": "my_secret_key_123"
}

# Gui HTTP POST kem API Key
response = requests.post(url, json=data, headers=headers)

# Hien thi ket qua
print("NGUYEN THI PHUONG NHUNG - 2033240244")
print("BAI 4 - GUI HTTP POST KEM API KEY")
print("--------------------------------")

print("Du lieu da gui:", data)
print("API Key da gui:", headers["X-API-Key"])
print("Ma trang thai:", response.status_code)
print("Noi dung phan hoi:", response.text)