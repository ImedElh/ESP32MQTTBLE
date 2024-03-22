import os 
import re
Import("env")
#print(env.Dump())
# already set
#env["UPLOAD_PROTOCOL"] = "espota"
# get OTA password.. from e.g. env variables, or from a text file, ..
if os.path.isfile("cred.txt"):
  lines = open("cred.txt","r").read().splitlines()
  print(lines[0])
  pwd = re.findall(r'"([^"]*)"', lines[0])
  print(pwd)
#pw = os.getenv("WIFI_PWD", default=None)
if pwd is None:
    print("!! No ESP OTA password given!! Define the environment variable ESP_OTA_PASSWORD")
    # hard exit if wanted
    #env.Exit(1)
else:
  env.Append(CPPDEFINES=[("WIFI_PWD", "\\\"" + pwd[0] + "\\\"")])
# you can also set the "upload port" = ip / mDNS name here if needed 
#env["UPLOAD_PORT"] = "192.168.1.192"