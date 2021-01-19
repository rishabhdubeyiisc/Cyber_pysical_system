#!/usr/bin/env python3
import telebot
import time
import threading
import class_debugger

class bot_cls:
    def __init__(self , token , help_string , command_argv = [] , alert_argv = [] ):
        self.token = token
        self.help_string = help_string
        self.bot = telebot.TeleBot(token=token)
        self.chat_id_to_reply = 0
        self.debugger = class_debugger.debugger_class(use_debugger=True,verbose_control=False,create_dir=False,filename="bot_talks.log")

        # manual command handler
        @self.bot.message_handler(func= lambda msg : msg.text is not None and '@' in msg.text )
        def manual_control(message):
            self.debugger.log("bot.message_handler.manual_control",message.text)
            self.chat_id_to_reply = message.chat.id
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
            self.chat_id_to_reply = message.chat.id
            self.message = message
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
            self.debugger.log("bot.alert_responder.alert_responder ",sensor_temp + " ")
            self.bot.reply_to(message,alert_argv[0])

        # Handle all other messages.
        @self.bot.message_handler(func=lambda message: True, content_types=['commands', 'audio', 'photo', 'voice', 'video', 'document', 'text', 'location', 'contact', 'sticker'])
        def default_command(message):
            self.debugger.log("bot.alert_responder.default_command",message.text)
            self.chat_id_to_reply = message.chat.id
            self.bot.reply_to(message , self.help_string )

        #run bot
        #self.bot_run()
    def send_msg(self , string_to_reply = "reply_string") :
        self.debugger.log("bot.send_msg", " " + string_to_reply + " ")
        self.bot.send_message( self.chat_id_to_reply , string_to_reply )
        #self.bot.send_message(message.chat.id, help_string)


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
