#!/usr/bin/env python3
from influxdb import InfluxDBClient

class db_client_cls:
    def __init__(self, IFhost = "localhost" , IFport = 8086 , IFDbname = 'CPU'):
        self.IFhost = IFhost
        self.IFport = IFport
        self.IFDbname = IFDbname
        #create a client
        self.client = InfluxDBClient(host=IFhost, port=IFport , database=IFDbname)
        print(self.get_db_list())
        to_run_script= input("press y/Y if DB created : ")
        if (to_run_script.lower() != 'y'):
            exit(-99)
        self.swtich_to_DB()

    def create_DB_by_name(self ):
        self.client.create_database(self.IFDbname)

    def get_db_list(self):
        return self.client.get_list_database()

    def swtich_to_DB(self):
        self.client.switch_database(self.IFDbname)

    def write_to_db ( self , data_json , ERR_str = "", verbose_mode = True):
        if (verbose_mode):
            print(data_json)
        is_data_wr = self.client.write_points(data_json)
        if(not is_data_wr):
            print("ERR : data not written " + ERR_str )
