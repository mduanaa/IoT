import paho.mqtt.client as mqtt
import time
from random import randint

host = 'smarteeb.ust.hk'
port = 49999
client_id = 'linux'

###########################################
# on connect
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag = True
        print("connection successful: ", rc)
    else:
        print("connection failed: ", rc)


# on_meassage
def on_message(client, userdata, message):
    print("message topic=",message.topic)
    print("message received " ,str(message.payload.decode("utf-8")))
    # print("message qos=",message.qos)
    # print("message retain flag=",message.retain)

def on_disconnect(client, userdata, rc=0):
    time.sleep(2)
    client.loop_stop()

# log information print
def on_log(client, userdata, level, buf):
    print('log: ', buf)
###########################################


mqtt.Client.connected_flag = False
# create client
client = mqtt.Client(client_id)
print("creating new client")
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect
# client.on_log = on_log

# start a connection & loop
client.loop_start()
client.connect(host, port)
#
while not client.connected_flag:
    time.sleep(1)
# subscribe
client.subscribe('CYT3014/touch')
print('Subscribe to new topic')
time.sleep(2)
# publish
client.publish('CYT3014/led', 'off')
print('publish to new topic')
# stop
time.sleep(2)

# while True:
#     client.publish('CYT3014/touch', randint(0, 100))
#     print('publish to new topic')
#     time.sleep(10)
client.disconnect()
# client.loop_stop()
