import serial
import os
import time

seconds = int(round(time.time()))
seconds -= 18000
command = 'echo ' + str(seconds) + ' > /dev/ttyACM0'
os.system(command)

fs = open('/var/www/html/alarm.txt', 'r') #open the alarm time file
hour = fs.read(2)
minute = fs.read(2)
fs.close()
command = 'echo ' +  str(hour) + str(minute) + ' > /dev/ttyACM0'
os.system(command)

