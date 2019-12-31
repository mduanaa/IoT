import socket
import sys

# local_host = '127.0.0.1'
port = 49999

# create TCP/IP
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# bind the socket to the port
server_address = ('localhost', port)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# call listen() to push into server mode and accept () for incoming connection
# listen for incoming connections
sock.listen(1)

while True:
    # wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()

    # recv data
    try:
        print('connection from', client_address)

        while True:
            data = connection.recv(1024)
            print('received {!r}'.format(data))
            if data:
                print('sending data back to the client')
                connection.sendall(data)
            else:
                print('no more data from', client_address)
                break
    finally:
    # clean up the connection
        print(sys.stderr, 'closing socket')
        connection.close()


