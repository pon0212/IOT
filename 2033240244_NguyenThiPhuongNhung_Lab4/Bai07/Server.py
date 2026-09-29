
from flask import Flask, request, jsonify

# Tao ung dung Flask
app = Flask(__name__)

# Thong tin sinh vien
print("NGUYEN THI PHUONG NHUNG - 2033240244")
print("BAI 7 - TAO LOCAL TEST SERVER BANG FLASK")
print("--------------------------------")

# Tao API nhan du lieu cam bien
@app.route("/api/sensor", methods=["POST"])
def nhan_du_lieu():

    # Doc du lieu JSON tu Client
    data = request.get_json()

    # Hien thi du lieu nhan duoc
    print("--------------------------------")
    print("SERVER DA NHAN DU LIEU:")
    print(data)

    # Tra ket qua ve Client
    return jsonify({
        "status": "success",
        "message": "Data received successfully"
    }), 200


# Khoi dong Server
if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000)