## ARM uContoller intefaced with garfana server using Telegram
### Files:
#### ./src_c
	main.c_________________________: Contains initializations of modules PWN, ADC, UART. 
	shell.c ,shell.h_______________: Used to interface controller with PC.
	startup_rvmdk.S________________: Startup file containing all the interrupt handlers.
#### ./src_py
	logs___________________________: Contains logs of bot talks and communication with server.
	class_BOT.py___________________: Telegram Bot you have to put up a token to this.
	class_DB.py____________________: Creates a influx data base client to write to data source.
	class_debugger.py______________: Creates a debugger to log files.
	class_tiva.py__________________: Used to create a serial interface with UART.
	tiva_debug_IF.py_______________: Simple rd/wr to UART over serial port.
	tiva_if_continous_temp_read.py_: Takes desired values form sensor nodes and send to server.

#### ./Keil_ARM_project
	Contains project
