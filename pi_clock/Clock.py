import serial
import os
import time
seconds = int(round(time.time()) )
#seconds = 123123123
seconds -= 18000
print(seconds)
command = 'echo '+ str(seconds)+' > /dev/ttyACM0'
os.system(command)
#ser = serial.Serial('/dev/ttyACM0')
#print(ser.name)
#ser.write(12345678)
#ser.close()