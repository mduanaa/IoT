import paho.mqtt.client as mqtt
import datetime
import time
import json

# initiate mosquitto
# "c:\Program Files\mosquitto\mosquitto.exe" -p 49999 -v
host = 'smarteeb.ust.hk'
port = 49999
client_id = 'server'
file_path = './web_server/http/touchpad.json'

###########################################
# on connect
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connection successful: ", rc)
        # subscribe once connected
        print('Subscribe to new topic')
        client.subscribe('CYT3014/#')
        time.sleep(1)
    else:
        print("connection failed: ", rc)

# on_meassage
def on_message(client, userdata, message):
    # print("message qos=",message.qos)
    # print("message retain flag=",message.retain)
    print("message topic = ", message.topic, "received: ", str(message.payload.decode("utf-8")))
    # print(sensor_data['touch'])
    now = datetime.datetime.now()

    # data saving
    playload = float(message.payload.decode("utf-8"))
    if message.topic == "CYT3014/touch":
        sensor_data['time'].append(now.strftime("%Y-%m-%d %H:%M:%S"))
        sensor_data['touch'].append(playload)
    elif message.topic == "CYT3014/hall":
        sensor_data['hall'].append(playload)

    # data concate
    if len(sensor_data['time']) > 1000:
        sensor_data['time'] = sensor_data['time'][600:]
        sensor_data['touch'] = sensor_data['touch'][600:]
        sensor_data['hall'] = sensor_data['hall'][600:]

    # json save
    with open(file_path, 'w') as f:
        json.dump(sensor_data, f)

def on_disconnect(client, userdata, rc=0):
    client.loop_stop()

# log information print
def on_log(client, userdata, level, buf):
    print('log: ', buf)
###########################################

sensor_data = {
    "time": [],
    "touch": [],
    "hall": []
}

# create client
client = mqtt.Client(client_id)
print("creating new client")
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect
# client.on_log = on_log

# client.loop_start()
client.connect(host, port)
client.loop_forever()
# client.disconnect()
# client.loop_stop()
