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
parser.add_argument('-s', '--string', help='String to send to serial.')
args = parser.parse_args()

# Figure out which port to use and initialize the driver.
if args.port is None:
    ports = glob.glob('/dev/ttyACM*')
    assert len(ports) == 1, "No unique Arduino port!\n{}".format(ports)
    ser = serial.Serial(ports[0], 9600)
else:
    ser = serial.Serial('/dev/ttyACM{}'.format(args.port), 9600)

if args.string is None:
    # If user did not supply a string to send, send a test file.
    start_time = time.time()

    # Open test file to send
    line_n = 0
    try:
        with open(args.file, 'r') as f:
            for line in f: # Send file line-by-line
                # A Python3 thing. We need to send a byes array.
                print('Sending', line.rstrip().encode())
                ser.write(line.encode()) 
                line_n += 1
                # Lag between sending lines to allow the radio to 
                # fully recieve the message.
                time.sleep(0.3) 
    finally:
        print('Sent {} lines for {} s'.format(line_n, time.time() - start_time))
else:
    #time.sleep(0.5) 
    print("Sending", args.string.encode())
    ser.write(args.string.rstrip().encode())
