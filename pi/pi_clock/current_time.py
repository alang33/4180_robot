import serial
import os
import time

seconds = int(round(time.time()))
seconds -= 18000
command = 'echo ' + str(seconds) + ' > /dev/ttyACM0'
os.system(command)