# Python script to launch multiple rat tracker dataloggers 
import subprocess, os, sys, threading
## import time

 
for num in range(0,3):
  cmd = "python datalogger.py " + str(num)
  subprocess.Popen(cmd,text=True)
 
print("finished all devices!")
##while True:
##  time.sleep(1)
