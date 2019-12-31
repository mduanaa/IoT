from flask import Flask, request, send_from_directory

app = Flask(__name__, static_folder='./', static_url_path='')

host = '143.89.50.18'
# host = '127.0.0.1'
port = 8097

@app.route('/')
def root():
    return send_from_directory('./', 'index.html')

if __name__ == '__main__':
    app.run(host=host, port=port)


