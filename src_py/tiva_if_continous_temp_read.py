#!/usr/bin/env python3

# Sample python3 code communicating with the shell running on the tiva board.
#
# Note: turn off echo in the shell by removing the following line in the
#       target code:
#    ROM_UARTCharPutNonBlocking(UART0_BASE, ch);     // echo

# Load shell program on the TIVA board.
# Run this code and then reset the TIVA board.

# In Windows, you should see this under Device Manager / Ports.
# In Linux, you should see the tty port by running "dmesg -y".

# In the following example, it is COM3 on Windows, which translates
# to /dev/ttyS3 in WSL.

import datetime
import pytz
import psutil
import time
import threading
#my modules
import class_DB
import class_tiva
import class_BOT
from collections import deque
import class_debugger

def create_me_json (measurement = 'usage' , tag_name = 'Sensor' , tag_field = 'cpu_usage0' , filed_name = 'cpu_usage' , field_value = 0):
    time_stamp = datetime.datetime.now(pytz.utc)
    data_json = [
                    {
                        'measurement' : measurement ,
                        'tags' : { tag_name : tag_field },
                        'time' : time_stamp ,
                        'fields' : { filed_name : float(field_value)}
                    }
                ]

    string =            '{ ' + 'measurement ' + measurement +                                           ' }'
    string = string +   '{ ' + 'tags ' + ' @ ' + tag_name + " " + tag_field + ' @ ' +                   ' }'
    string = string +   '{ ' + 'time ' + str(time_stamp) +                                              ' }'
    string = string +   '{ ' + 'fields ' + '@ ' + filed_name + " " + str(float(field_value)) + ' @ ' +  ' }'

    return data_json , string

#TIVA commands that can be executed

def comm_process():
    print("in communication to ARM")
    last_command = ""
    last_alert = ""
    while True:
        try:
            temprature_C = float(0)
            #keep on reading the commands from bot
            #serial data in out
            #tiva_shell_if.send_to_shell(shell_command="readTemp c")
            #serial_din = tiva_shell_if.read_from_shell ()
            #temprature_C = serial_din
            #serial data in out done
            if (command_argv_global[0]):
                #print( command_argv_global[0] )
                if (command_argv_global[0] != "readtemp c"):
                    #run manual commands
                    if (last_command != command_argv_global[0] ):
                        last_command = command_argv_global[0]
                        print("running command : " + command_argv_global[0] )
                        shell_command = ""
                        shell_command = command_argv_global[0]
                        if ( "fanctrl" in command_argv_global[0] ):
                            fan_cmd = command_argv_global[0].replace("fanctrl","fanCtrl")
                            shell_command = fan_cmd
                        tiva_shell_if.send_to_shell(shell_command = shell_command )
                        try :
                            serial_din = tiva_shell_if.read_from_shell ()
                            print("serial_din " + serial_din)
                            bot_obj.send_msg(serial_din)
                        except:
                            pass
            try:
                if (alert_feedback_global[0]):
                    if (last_alert != alert_feedback_global[0]):
                        #temp > 60
                        last_alert = alert_feedback_global[0]
                        #do a feedback control
                        print("Executing F/b control based on alert")
                        duty_calculated = PID_calculated_value(alert_feedback_global[0]) / 10 # devide by 10 is to get to desired scale
                        shell_command = "fanCtrl " + str(duty_calculated)
                        #default command
                        tiva_shell_if.send_to_shell( shell_command = shell_command)
                        try:
                            serial_din = tiva_shell_if.read_from_shell ()
                            print("serial_din " + serial_din)
                            bot_obj.send_msg(serial_din + " taking action over alert")
                        except :
                            pass
            except:
                pass


            #keep sending temprature to server
            tiva_shell_if.send_to_shell(shell_command="readTemp c")
            serial_din = tiva_shell_if.read_from_shell ()
            temprature_C = serial_din

            sensor_data_json , log_string_temp = create_me_json (measurement = 'temperature' ,
                                               tag_name    = 'Sensor'  , tag_field = 'thermal_zone0' ,
                                               filed_name  = 'cputemp' , field_value = temprature_C)

            db_client.write_to_db( data_json = sensor_data_json , ERR_str= "SENSOR",verbose_mode=Verbose_GLOBAL)
            debugger.log("comm_process",log_string_temp)
            cpu_usage=psutil.cpu_percent()
            cpu_data_json , log_string_usage = create_me_json (measurement = 'usage' ,
                                            tag_name    = 'Sensor'    , tag_field = 'cpu_usage0' ,
                                            filed_name  = 'cpu_usage' , field_value = cpu_usage)

            db_client.write_to_db(data_json = cpu_data_json , ERR_str = "CPU" ,verbose_mode=Verbose_GLOBAL)
            debugger.log("comm_process",log_string_usage)

            #sleep
            time.sleep(1)
        except KeyboardInterrupt:
            print("KeyboardInterrupt")
            exit(99)

def PID_calculated_value(alert_temprature):
    #this will change
    duty_cycle = 50
    return duty_cycle

def print_strings( alert = [] , command = []) :
    print("PRINTER")
    while True :
        try:
            print (alert , command )
            time.sleep(4)
        except KeyboardInterrupt:
            exit(0)

Verbose_GLOBAL = False

GLOBAL_bot_token =""

"""
static cmdtable_t CmdTable[] =
{
	{"help"			, "help: this command       \n", doHelp	    },
	{"led"			, "led [r|b] on-off         \n", doLed      },
	{"readTemp"	, "readTemp c/f                 \n", readTemp   },
	{"fanCtrl"	, "fanCtrl [duty {0 - 10}]      \n", fanCtrl	}
};
"""

GLOBAL_help_string =                       '  @ is used as a password at present     \n '
GLOBAL_help_string = GLOBAL_help_string +   ' @ <command>                            \n '
GLOBAL_help_string = GLOBAL_help_string +   ' List of available commands :           \n '
GLOBAL_help_string = GLOBAL_help_string +   ' 1.)@ : LED : [r|b] : [on|off]          \n '
GLOBAL_help_string = GLOBAL_help_string +   ' 2.)@ : FANCTRL : [0,10] => duty cycle  \n '
GLOBAL_help_string = GLOBAL_help_string +   ' 3.)@ : READTEMP : c/f                  \n '
GLOBAL_help_string = GLOBAL_help_string +   ' 4.)help'


command_argv_global = ["readtemp c"]

alert_feedback_global = []

if __name__ == "__main__":
    if (GLOBAL_bot_token == ""):
        print("ERR : Bot Token not present")
        exit(99)
    print("creating_debugger")
    debugger = class_debugger.debugger_class(use_debugger=True,verbose_control=False,create_dir=True,filename="DB_transfer.log")

    print("creating db_client")
    db_client = class_DB.db_client_cls(IFhost="localhost",IFport=8086,IFDbname='CPU')

    print("initializing tiva IF")
    tiva_shell_if = class_tiva.tiva_serial_if()

    print("creating BOT")
    bot_obj = class_BOT.bot_cls(token=GLOBAL_bot_token,help_string=GLOBAL_help_string , command_argv= command_argv_global , alert_argv=alert_feedback_global)

    #creating threads
    try:
        msg_reciver = threading.Thread( target=bot_obj.bot_run )
    except :
        print("Error in threads msg_reciver")

    try:
        printer = threading.Thread( target=print_strings , args= ( command_argv_global , alert_feedback_global))
    except :
        print("Error in threads printer")

    try:
        comm_thread = threading.Thread(target=comm_process )
    except :
        print("Error in threads comm_thread")
    #starting threads
    msg_reciver.start()
    if (Verbose_GLOBAL ):
        printer.start()
    comm_thread.start()
