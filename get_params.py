import os 
import re
Import("env")
#print(env.Dump())
# already set
#env["UPLOAD_PROTOCOL"] = "espota"
# get passwords.. from e.g. env variables, or from a text file, ..
if os.path.isfile("cred.txt"):
  lines = open("cred.txt","r").read().splitlines()
  x = len(lines)
  for x in lines:
   data = re.findall(r'"([^"]*)"', x)
   s = x.split('=')
   if (s[0] is None) or (s[1] is None) :
    print("!! Wrong data in cred file !!")
    # hard exit if wanted
    #env.Exit(1)
   else:
    env.Append(CPPDEFINES=[(s[0], "\\\"" + data[0] + "\\\"")])
  
