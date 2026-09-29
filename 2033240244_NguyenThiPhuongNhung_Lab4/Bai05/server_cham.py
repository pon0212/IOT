
from http.server import BaseHTTPRequestHandler, HTTPServer
import time

class ServerCham(BaseHTTPRequestHandler):
    def do_POST(self):
        length = int(self.headers.get("Content-Length", 0))
        self.rfile.read(length)

        print("Da nhan du lieu. Dang gia lap phan hoi cham...")
        time.sleep(5)

        self.send_response(200)
        self.end_headers()

        try:
            self.wfile.write(b"Data received")
        except BrokenPipeError:
            print("Client da ngat ket noi do timeout.")

server = HTTPServer(("127.0.0.1", 5001), ServerCham)

print("Server dang chay tai http://127.0.0.1:5001")
server.serve_forever()
