# This script will listen to the serial port for strings from Arduino.
import glob
import serial
import argparse

parser = argparse.ArgumentParser(description='Listens to a ttyACM# '
                            'port for transmissions from an Arduino.')
parser.add_argument('-p', '--port', help='Port to listen to.')
parser.add_argument('-b', '--binary_mode', default=False,
        help='Wether the incoming data in binary format e.g. zip files.')
args = parser.parse_args()

BAUD_RATE = 9600

# Figure out which port to use and initialize the driver.
if args.port is None:
    ports = glob.glob('/dev/ttyACM*')
    assert len(ports) == 1, "No unique Arduino port!\n{}".format(ports)
    ser = serial.Serial(ports[0], BAUD_RATE)
else:
    ser = serial.Serial('/dev/ttyACM{}'.format(args.port), BAUD_RATE)

if args.binary_mode:
    openFileMode = 'wb'
else:
    openFileMode = 'w'

# Open test file to write data to
with open('test_file.txt', openFileMode) as f:
    # Keep listening while script running
    while True: 
        # Decode line just sent.
        if args.binary_mode:
            line = ser.readline()
        else:
            line = ser.readline().decode()
        print('Recieved:\n', line)
        f.write(line)
