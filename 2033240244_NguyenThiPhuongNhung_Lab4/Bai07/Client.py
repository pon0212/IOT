
import requests

# Dia chi Local Server
url = "http://127.0.0.1:5000/api/sensor"

# Du lieu cam bien
data = {
    "device_id": "sensor_01",
    "temperature": 28.5,
    "humidity": 75
}

print("NGUYEN THI PHUONG NHUNG - 2033240244")
print("BAI 7 - GUI DU LIEU DEN LOCAL SERVER")
print("--------------------------------")

# Gui du lieu den Flask Server
try:
    response = requests.post(url, json=data, timeout=5)

    print("Du lieu da gui:", data)
    print("Ma trang thai:", response.status_code)
    print("Noi dung phan hoi:", response.json())

except requests.exceptions.RequestException as e:
    print("LOI KET NOI:", e)