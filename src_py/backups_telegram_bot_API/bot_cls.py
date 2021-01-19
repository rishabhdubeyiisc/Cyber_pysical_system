#!/usr/bin/env python3
import telebot
import time
import threading

class bot_cls:
    def __init__(self , token , help_string , command_argv = [] , alert_argv = [] ):
        self.token = token 
        self.help_string = help_string
        self.bot = telebot.TeleBot(token=token)

        # manual command handler
        @self.bot.message_handler(func= lambda msg : msg.text is not None and '@' in msg.text )    
        def manual_control(message):
            message_splitted = message.text.split(':')
            #print(message_splitted)
            message_splitted.pop(0) #remove @ from string
            #command_argv = []
            command_argv.clear() 
            for i in range ( len(message_splitted) ):
                string = message_splitted[i].replace(" ","").lower()
                #string = string.lower()
                command_argv.append(string) 
            #print(command_argv)
            string_message = self.str_list_to_string(command_argv)
            command_argv.clear()
            command_argv.append(string_message)
            self.bot.reply_to( message , string_message )
        
        #alert handler
        @self.bot.message_handler(func= lambda msg : msg.text is not None and '[Alerting]' in msg.text)    
        def alert_responder(message):
            str_len = len(message.text.splitlines()) - 1 
            temp_string = message.text.splitlines()[ str_len ]
            #print(temp_string)
            temp_split = ""
            sensor_temp = "no_data"

            if ': ' in temp_string:
                temp_split = temp_string.split(': ')
                sensor_temp = temp_split [1]

            #print(sensor_temp)
            alert_argv.clear()
            alert_argv.append(sensor_temp)
            self.bot.reply_to(message,alert_argv[0])

        # Handle all other messages.
        @self.bot.message_handler(func=lambda message: True, content_types=['commands', 'audio', 'photo', 'voice', 'video', 'document', 'text', 'location', 'contact', 'sticker'])
        def default_command(message):
            #bot_object.send_message(message.chat.id, help_string)
            self.bot.reply_to(message , self.help_string )
        
        #run bot 
        #self.bot_run()
        
    def bot_run(self):
        print("BOT_RUN_START")
        while True:
            try :
                try:
                    self.bot.polling(timeout=15,interval=0,none_stop=False)
                except Exception:
                    time.sleep(15)
                except RecursionError:
                    print("RecursionError")
            except KeyboardInterrupt:
                break
        return 0
    
    def str_list_to_string(self , list_string):
        string = ""
        for i in range(len(list_string)):
            string = string + list_string[i] + " "
        #remove last wihitespace
        string = string.rstrip()
        return string 




def print_strings( alert = [] , command = []) :
    print("PRINTER")
    while True :
        try:
            print (alert , command )
            time.sleep(4)
            #alert.clear()
            #command.clear()
        except KeyboardInterrupt:
            exit(0)

GLOBAL_bot_token ='1539989474:AAH8ut_pJbwALmLwCm44ixQGeoChvaddEPk'

GLOBAL_help_string = '@ is used as a password at present \n @ <command> \n List of available commands : \n 1.)@ : LED : [r|g|b] : [on|off] \n 2.)@ : FAN : [on|off]'

command_argv_global = []

alert_argv_global = []

bot_onj = bot_cls(token=GLOBAL_bot_token,help_string=GLOBAL_help_string , command_argv= command_argv_global , alert_argv=alert_argv_global)
msg_reciver = threading.Thread( target=bot_onj.bot_run )

printer = threading.Thread( target=print_strings , args= ( command_argv_global , alert_argv_global))

msg_reciver.start()
printer.start()
