#!/usr/bin/env python3

from datetime import datetime

today = datetime.now()
log = "last unlock on {} ".format(today.strftime("%d/%m/%Y %H:%M:%S"))

with open("/home/pi/log.txt", "a") as a_file:
  a_file.write(log + "\n")

import os
beep = lambda x: os.system("echo -n '\a';sleep 0.2;" * x)
beep(3)
