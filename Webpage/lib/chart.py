#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import cgi
import json
# import random
import configparser
# import MySQLdb
# from datetime import *

def getQuerystring(name):
    try:
        return form[name].value
    except:
        print(json.JSONEncoder().encode({}))
        exit()

# def getConnection():
#     username = config['mysql']['username']
#     password = config['mysql']['password']
#     database = config['mysql']['database']
#     return MySQLdb.connect(user=username, passwd=password, db=database)

# def getSensors():
#     response = []
#     dbconn = getConnection()
#     cursor = dbconn.cursor()
#     cursor.execute("select code from keda.gas_terminal where link_status=1 and run_status=1;")
#     for (sensor,) in cursor:
#         response.append(sensor)
#     dbconn.close()
#     return response

def getSensorData(sensor, dtype, stamp):
    response = {'x': [], 'y': [], 'timestamp': None, 'title': None}
    # Database data.
    dbconn = getConnection()
    cursor = dbconn.cursor()
    try:
        dconf = eval(config["sensor"][dtype])
        stamp = datetime.fromtimestamp(int(stamp)/1000).strftime(config["mysql"]["datetime"])
        query = dconf[0].format(sensor, dconf[1], stamp)
        cursor.execute(query)
    except:
        return response
    # Fill data.
    for (report_time, data) in cursor:
        try:
            response['x'].append(report_time.strftime(config["mysql"]["datetime"]))
            response['y'].append(float(data))
            response['timestamp'] = int(report_time.timestamp())*1000
            response['title'] = dconf[2]
        except:
            pass
    # Close and return.
    dbconn.close()
    return response

# Read config.
config = configparser.ConfigParser(interpolation=configparser.ExtendedInterpolation())
config.read("../conf/config-site.ini")

# HTTP handling.
form = cgi.FieldStorage()
print("Content-type: text/html\n")
action = getQuerystring("action")
if action == "active_sensors":
    print(json.JSONEncoder().encode(getSensors()))
elif action == "latest_data":
    sensor = getQuerystring("sensor")
    dtype = getQuerystring("type")
    stamp = getQuerystring("timestamp")
    print(json.JSONEncoder().encode(getSensorData(sensor, dtype, stamp)))