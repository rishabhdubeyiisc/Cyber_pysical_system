#!/usr/bin/env python3

# Sample python3 code communicating with the shell running on the tiva board.
#
# Note: turn off echo in the shell by removing the following line in the
#       target code:
#    ROM_UARTCharPutNonBlocking(UART0_BASE, ch);     // echo

# Load shell program on the TIVA board.
# Run this code and then reset the TIVA board.

import serial

prompt = b'> '

# Find out the virtual com port.
# In Windows, you should see this under Device Manager / Ports.
# In Linux, you should see the tty port by running "dmesg -y".

# In the following example, it is COM3 on Windows, which translates
# to /dev/ttyS3 in WSL.
s = serial.Serial('/dev/ttyS6', baudrate=115200, timeout=None)

# Skip greetings and prompt
b_in = s.read_until(prompt)
s_in = b_in.decode('ASCII')
print(s_in, end='')

# Read a command from stdin, send it to TIVA and print the response.
while True:
    # Read command from the stdin
    s_in = input()
    s_in = s_in.replace('\n', '\n\r')
    b_in = s_in.encode('ASCII')
    s.write(b_in)
    s.write(b'\r')

    # Read the response
    b_in = s.read_until(prompt)

    # Remove newlines and prompt
    b_in = b_in.replace(b'\n', b'')
    b_in = b_in.replace(prompt, b'')

    # Print the response and the prompt
    print ('response = ', b_in.decode('utf-8'))
    print(prompt.decode('ASCII'), end='')

# end of file
