import http.server
import socketserver

host = '143.89.50.18'
port = 8097

Handler = http.server.SimpleHTTPRequestHandler
# file_path = './touchpad.json'
#
# # data load
# with open(file_path, 'r') as f:
#         touchpad = json.load(f)

with socketserver.TCPServer((host, port), Handler) as httpd:
    print('serving at port ', port)
    httpd.serve_forever()

