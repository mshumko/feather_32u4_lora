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
parser.add_argument('-t', '--timeout', help='Port listen timeout (seconds).', default=20)
parser.add_argument('-r', '--retry', help='Number of seconds before we retry to send a second packet.', default=2)
args = parser.parse_args()

N_RETRIES = 5

# Figure out which port to use and initialize the driver.
if args.port is None:
    ports = glob.glob('/dev/ttyACM*')
    assert len(ports) == 1, "No unique Arduino port!\n{}".format(ports)
    ser = serial.Serial(ports[0], 9600)
else:
    ser = serial.Serial('/dev/ttyACM{}'.format(args.port), 9600)

BLOCK_SIZE = 251 # bytes (max allowed by the Radiohead FM95 radio driver)

if 'zip' in args.file:
    BINARY_MODE = True
    fileReadMode = 'rb'
    iterSentinel = b''
else:
    BINARY_MODE = False
    fileReadMode = 'r'
    iterSentinel = ''

def send_block(block, blocks_sent):
    #print('Sending', block.rstrip())
    ser.write(block) 
    wait_start_time = time.time()
    tries = 0
    while 1:
        if ser.in_waiting:
            ack_line = ser.readline().decode()
            if 'Message recieved' in ack_line:
                #print(ack_line)
                blocks_sent += 1
                break
            
        if time.time() - wait_start_time > (tries+1)*args.retry:
            print('Retrying to send packet')
            ser.write(block)
            tries += 1
            if tries == N_RETRIES:
                print('Tried sending packet {} times without success. Exiting'.format(tries))
                break

        # Break if no response heard after a timeout.
        if time.time() - wait_start_time > args.timeout:
            print('No response heard. Sent\n', block.rstrip(), '\n')
            break
    return blocks_sent

if args.string is None:
    # If user did not supply a string to send, send a test file.
    start_time = time.time()

    # Open test file to send
    blocks_sent = 0
    try:
        with open(args.file, fileReadMode) as f:
            # This block for loop is where the magic happens. 
            # iter(...) returns an iterable object that is
            # f.read(BLOCK_SIZE) in this case. BLOCK_SIZE is 
            # the number of
            # bytes in each block. The loop will go on until
            # the sentinel ('' arg of iter) is found at which
            # point StopIteration exception is raised and the
            # loop halts. 
            for block in iter(partial(f.read, BLOCK_SIZE), iterSentinel):
                if BINARY_MODE:
                    blocks_sent = send_block(block, blocks_sent)
                else:
                    # encode() is a Python3 thing. We need to 
                    # send a byes array.
                    blocks_sent = send_block(block.encode(), blocks_sent)

    finally:
        dt = time.time() - start_time
        print('\n\nSent {} blocks for {} s ({} bytes/s)\n\n'.format(
                    blocks_sent, dt, BLOCK_SIZE*blocks_sent/dt))
else:
    print("Sending", args.string.encode())
    ser.write(args.string.rstrip().encode())
