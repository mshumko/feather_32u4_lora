# This script will listen to the serial port for strings from Arduino.
import glob
import serial
import argparse

parser = argparse.ArgumentParser(description='Listens to a ttyACM# '
                            'port for transmissions from an Arduino.')
parser.add_argument('-p', '--port', help='Port to listen to.')
args = parser.parse_args()

# Figure out which port to use and initialize the driver.
if args.port is None:
    ports = glob.glob('/dev/ttyACM*')
    assert len(ports) == 1, "No unique Arduino port!\n{}".format(ports)
    ser = serial.Serial(ports[0], 9600)
else:
    ser = serial.Serial('/dev/ttyACM{}'.format(args.port), 9600)

# Open test file to write data to
with open('test_file.txt', 'w') as f:
    # Keep listening while script running
    while True: 
        # Decode line just sent.
        line = ser.readline().decode()
        print(line.rstrip())
        # Write the data to file.
        if 'Recieved' in line: 
            f.write(line)

