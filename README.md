# ARM uContoller intefaced with server using Telegram
Files:
Project_Folder:
./ex9-tivaware-shell - final_project/shell : is the project that can be loaded over shell
	/src contain all the files for building shell
		/main.c
		/shell.c
		/shell.h
		/startup_rvmdk.s
		```
		Use Kiel to build project with these files as Src Run Putty or tiva_if file from src folder 
		```
./source_code : Contains all the code relevant to this project
	/py_f
		/class_BOT __________________: 
			Class to create a Telegram bot 
			the token must be changed 
			if someone want to create a new bot 
		/class_DB  __________________: 
			Class to create a DB client 
			and wr over the DB sources
		/class_debugger _____________: 
			Class debuggers is to create log files 
			it creates a folder so named /logs 
			in current directory
		/class_tiva _________________: 
			Class to create a 	
			serial IF and to communicate 
			with tiva board
		/tiva_debug_IF_______________: 
			so was used as a reference
		/tiva_if_continous_temp_read_: 
			This is the main python code 
			that will be executed, 
			interpreter is already set for linux
	/c_f
		/startup_rvmdk.s ____________: 
			startup file with UART_handler 
			and default ADC handler
		/main.c _____________________: 
			contaions all the initialization 
			and command_execute loop
		/shell.h  ___________________: 
			header for shell.c
		/shell.c ____________________: 
			contains all the relevant code 
			to run a shell on ARM

./logs
	/DB_transfer ________________________: 
		Shows logs of data transfer by DB_client
	/bot_talks __________________________: 
		Shows exchanges between BOT and ARM

./screenshots
	Contains some screenshots, But you can go over the video 
    
