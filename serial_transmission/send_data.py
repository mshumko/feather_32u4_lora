# This script will listen to the serial port for strings from Arduino.
import glob
import serial
import time
import argparse

parser = argparse.ArgumentParser(description='Transmitts data to a '
                            'ttyACM# port to an Arduino.')
parser.add_argument('-p', '--port', help='Port to listen to.')
parser.add_argument('-f', '--file', help='File to send', 
                  default='./../data/laftr_example_data.txt')
args = parser.parse_args()

# Figure out which port to use and initialize the driver.
if args.port is None:
    ports = glob.glob('/dev/ttyACM*')
    assert len(ports) == 1, "No unique Arduino port!\n{}".format(ports)
    ser = serial.Serial(ports[0], 9600)
else:
    ser = serial.Serial('/dev/ttyACM{}'.format(args.port), 9600)

start_time = time.time()

# Open test file to send
with open(args.file, 'r') as f:
    for line in f: # Send file line-by-line
        # A Python3 thing. We need to send a byes array.
        ser.write(line.encode()) 
        print('Sent', line.rstrip())
        # Lag between sending lines to allow the radio to 
        # fully recieve the message.
        time.sleep(0.1) 

print('Time to send file {}'.format(time.time() - start_time))
