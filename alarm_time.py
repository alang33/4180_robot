import serial
import os

fs = open("/var/www/html/alarm.txt", "r") 
hour = fs.read(2)
minute = fs.read(2)
fs.close()
command = "echo " + hour + minute + " > /dev/ttyACM0"
os.system(command)
