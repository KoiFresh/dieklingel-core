#!/usr/bin/env python3

from datetime import datetime
import requests

# Write Date to Logfile

today = datetime.now()
log = "last unlock on {} ".format(today.strftime("%d/%m/%Y %H:%M:%S"))
with open("/home/pi/log.txt", "a") as a_file:
  a_file.write(log + "\n")

url = "http://raspberrypi:8083/fhem?"
cmd = "set Drucker on"


payload = {}
headers = {}
csrfToken = ""
uri = url + "cmd=" + cmd + "&fwcsrf=" + csrfToken + "&XHR=1"

payload={}
headers = {}
response = requests.request("GET", uri, headers=headers, data=payload)

if response.status_code == 400:
    csrfToken = response.headers["X-FHEM-csrfToken"]
    uri = url + "cmd=" + cmd + "&fwcsrf=" + csrfToken + "&XHR=1"
    response = requests.request("GET", uri, headers=headers, data=payload)
