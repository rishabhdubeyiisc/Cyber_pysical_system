#!/usr/bin/env python3

# Sample python3 code communicating with the shell running on the tiva board.
#
# Note: turn off echo in the shell by removing the following line in the
#       target code:
#    ROM_UARTCharPutNonBlocking(UART0_BASE, ch);     // echo

# Load shell program on the TIVA board.
# Run this code and then reset the TIVA board.

import serial
import datetime
from influxdb import InfluxDBClient

IFhost = "127.0.0.1"
IFport = 3000
IFDbname = 'CPS'

client = InfluxDBClient(host=IFhost, port=IFport , database=IFDbname)

prompt = b'> '

# Find out the virtual com port.
# In Windows, you should see this under Device Manager / Ports.
# In Linux, you should see the tty port by running "dmesg -y".

# In the following example, it is COM3 on Windows, which translates
# to /dev/ttyS3 in WSL.
usb = serial.Serial('/dev/ttyS6', baudrate=115200, timeout=None)

# Skip greetings and prompt
serial_Din = usb.read_until(prompt)
serial_Din_ascii = serial_Din.decode('ASCII')
print(serial_Din_ascii, end='')

# Read a command from stdin, send it to TIVA and print the response.
stabilze = 3
i = 0 
sample_no = 0 
while True:
    # Read command from the stdin
    serial_Din_ascii = "readTemp c"
    serial_Din_ascii = serial_Din_ascii.replace('\n', '\n\r')
    serial_din = serial_Din_ascii.encode('ASCII')
    usb.write(serial_din)
    usb.write(b'\r')

    # Read the response
    serial_din = usb.read_until(prompt)

    # Remove newlines and prompt
    serial_din = serial_din.replace(b'\n', b'')
    serial_din = serial_din.replace(prompt, b'')

    # Print the response and the prompt
    if ( i > stabilze):
        sample_no = sample_no + 1
        time = datetime.datetime.now()
        print (time , sample_no , 'temprature = ', serial_din.decode('utf-8') , " *C ")
        print(prompt.decode('ASCII'), end='')
        continue
    i = i + 1

# end of file
