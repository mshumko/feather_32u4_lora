# This script will listen to the serial port for strings from Arduino.
import glob
import serial
import time

path = './../data/laftr_example_data.txt'

ports = glob.glob('/dev/ttyACM*')
assert len(ports) == 1, "No unique Arduino port!\n{}".format(ports)

ser = serial.Serial(ports[0], 9600)

start_time = time.time()

with open(path, 'r') as f:
    for line in f:
        ser.write(line.encode())
        print('Sent', line.rstrip())
        print('Recieved', ser.readline().rstrip(), '\n')

print('Time to send file {}'.format(time.time() - start_time))
