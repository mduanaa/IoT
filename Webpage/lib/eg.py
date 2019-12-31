import cgi
import time
import json

file_path = '../touchpad.json'

# loop plot
def get_data():
    with open(file_path, 'r') as f:
        touchpad = json.load(f)
        # print(touchpad)
    return touchpad

print(get_data())
