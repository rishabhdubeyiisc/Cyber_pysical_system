import serial

################   UART

# Find out the virtual com port.

class tiva_serial_if ():
    def __init__ (self):
        self.serial_port = '/dev/ttyACM0'
        self.usb = serial.Serial( self.serial_port, baudrate=115200, timeout=None)
        # Skip greetings and prompt
        print("Press reset on TIVA")
        self.prompt = b'> '
        self.serial_Din = self.usb.read_until(self.prompt)
        self.serial_Din_ascii = self.serial_Din.decode('ASCII')
        print(self.serial_Din_ascii, end='')

    def send_to_shell(self, shell_command = "readTemp c" ):
        self.serial_Din_ascii = shell_command
        self.serial_Din_ascii = self.serial_Din_ascii.replace('\n', '\n\r')
        self.serial_din = self.serial_Din_ascii.encode('ASCII')
        self.usb.write(self.serial_din)
        self.usb.write(b'\r')

    def read_from_shell( self):
        # Read the response
        self.serial_din = self.usb.read_until(self.prompt)
        # Remove newlines and prompt
        self.serial_din = self.serial_din.replace(b'\n', b'')
        self.serial_din = self.serial_din.replace(self.prompt, b'')
        return self.serial_din.decode('utf-8')
