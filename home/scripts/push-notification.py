import os, json, requests
from datetime import datetime

SIGN = os.getenv("SIGN")

devices = requests.get(f"http://localhost:8081/devices?signs={SIGN}").json()
tokens = list(map(lambda device: device['token'], devices))

if not tokens:
    print("no tokens: early exit!")
    exit()

now = datetime.now()

payload = {
  'tokens': tokens,
  'id': 'random123',
  'title': 'Jemand steht vor deiner Tür!',
  'body': now.strftime("%d.%m.%Y %H:%M:%S")
}

requests.post("https://fcm-worker.dieklingel.workers.dev/fcm/send", json = payload)
