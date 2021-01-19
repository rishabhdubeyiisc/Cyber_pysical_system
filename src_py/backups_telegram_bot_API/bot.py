#!/usr/bin/env python3
import telebot
import time
import threading


bot_object = telebot.TeleBot(token=GLOBAL_bot_token)

argv = []

GLOBAL_bot_token ='1539989474:AAH8ut_pJbwALmLwCm44ixQGeoChvaddEPk'

GLOBAL_help_string = '@ is used as a password at present \n @ <command> \n List of available commands : \n 1.)@ : LED : [r|g|b] : [on|off] \n 2.)@ : FAN : [on|off]'


@bot_object.message_handler(func= lambda msg : msg.text is not None and '@' in msg.text)    
def manual_control(message):
    message_splitted = message.text.split(':')
    print(message_splitted)
    message_splitted.pop(0) #remove @ from string
    global argv
    argv = [] 
    for i in range ( len(message_splitted) ):
        string = message_splitted[i].replace(" ","")
        string = string.lower()
        argv.append(string) 
    print(argv)
    bot_object.reply_to(message,"applying manual action")

@bot_object.message_handler(func= lambda msg : msg.text is not None and '[Alerting]' in msg.text)    
def alert_responder(message):
    str_len = len(message.text.splitlines()) - 1 
    temp_string = message.text.splitlines()[ str_len ]
    print(temp_string)
    temp_split = ""
    sensor_temp = "no_data"

    if ': ' in temp_string:
        temp_split = temp_string.split(': ')
        sensor_temp = temp_split [1]

    print(sensor_temp)
    bot_object.reply_to(message,sensor_temp)


# Handle all other messages.
@bot_object.message_handler(func=lambda message: True, content_types=['commands', 'audio', 'photo', 'voice', 'video', 'document', 'text', 'location', 'contact', 'sticker'])
def default_command(message):
    #bot_object.send_message(message.chat.id, help_string)
    bot_object.reply_to(message , GLOBAL_help_string)


def bot_run(bot_object):
    while True:
        try :
            try:
                bot_object.polling(timeout=15,interval=0,none_stop=False)
            except Exception:
                time.sleep(15)
            except RecursionError:
                print("RecursionError")
        except KeyboardInterrupt:
            break
    return 0


threading.Thread(target=bot_run(bot_object)).start()