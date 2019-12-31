import socket
import sys
import datetime

# host
server_ip = '143.89.50.18'
port = 49999

# creat a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# connect the socket to the port where the server is listening
server_address = (server_ip, port)
print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

# client and server communication
try:
    # send data
    message = b'This is the message, it will be repeated.'
    print ('sending {!r}'.format(message))
    sock.sendall(message)

    # wait for response
    amount_received = 0
    amount_expected = len(message)
    while amount_received < amount_expected:
        data = sock.recv(1024)
        amount_received += len(data)
        print('received {!r}'.format(data))

    # exception
finally:
    print(datetime.datetime.now(), ' closing socket')
    sock.close()

