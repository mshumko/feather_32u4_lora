# This script will listen to the serial port for strings from Arduino.
import glob
import serial
import time
import argparse
from functools import partial

parser = argparse.ArgumentParser(description='Transmitts data to a '
                            'ttyACM# port to an Arduino.')
parser.add_argument('-p', '--port', help='Port to listen to.')
parser.add_argument('-f', '--file', help='File to send', 
                  default='./../data/laftr_example_data.txt')
parser.add_argument('-s', '--string', help='String to send to serial.')
parser.add_argument('-t', '--timeout', help='Port listen timeout (seconds).', default=5)
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
    lines_sent = 0
    try:
        with open(args.file, 'r') as f:
            #for line in f: # Send file line-by-line
            for block in iter(partial(f.read, 251), ''):
                # A Python3 thing. We need to send a byes array.
                # print('Sending', line.rstrip().encode())
                # ser.write(line.encode()) 
                # wait_start_time = time.time()
                # while 1:
                #     if ser.in_waiting:
                #         #print(ser.readline().decode())
                #         ack_line = ser.readline().decode()
                #         if 'Message recieved' in ack_line:
                #             #print('Recieved', ack_line)
                #             lines_sent += 1
                #             break

                print('Sending', block.rstrip().encode())
                ser.write(block.encode()) 
                wait_start_time = time.time()
                while 1:
                    if ser.in_waiting:
                        #print(ser.readline().decode())
                        ack_line = ser.readline().decode()
                        if 'Message recieved' in ack_line:
                            print('Recieved', ack_line)
                            lines_sent += 1
                            break

                    # Break if no response heard after a timeout.
                    if time.time() - wait_start_time > args.timeout:
                        print('No response heard.')
                        break


    finally:
        dt = time.time() - start_time
        print('\n\nSent {} lines for {} s ({} lines/s)\n\n'.format(lines_sent, dt, lines_sent/dt))
else:
    #time.sleep(0.5) 
    print("Sending", args.string.encode())
    ser.write(args.string.rstrip().encode())
