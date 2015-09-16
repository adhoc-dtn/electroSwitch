# you must unblock bluetooth device before running this program.

# $ rfkill unblock bluetooth

import mraa
import time
import datetime
import sys
import commands
from bluetooth import *

from datetime import datetime

#bluetooth client
uuid = "00000000-0000-0000-0000-000000000001"
addr = "5C:51:88:CC:84:3C"
#addr = "C4:21:C8:C9:BF:55"

# Create the client socket
sock=BluetoothSocket( RFCOMM )
#sock_retry=BluetoothSocket( RFCOMM )

#pin for open or close detection
pin = mraa.Gpio(14)
pin.dir(mraa.DIR_IN)
pin.mode(mraa.MODE_PULLUP)

#pin for bluetooth conection
blc = mraa.Gpio(0)
blc.dir(mraa.DIR_IN)
blc.mode(mraa.MODE_PULLUP)   

time_now = 1
spacer = 30
time_old = 0-spacer

counter=0
def detectVoltage(args):
    global time_now
    global time_old
    time_now = datetime.now().second

    if(pin.read()==1):
	string = "open"
	print("now " + str(time_now))
 	print("old " + str(time_old))

        if ( time_now >= time_old):
            time_bet_last = time_now-time_old
	else :
            time_bet_last = time_now+60-time_old

	if(sock is None) :
        	print("Socket is None")
    	else :
		if(time_bet_last >= 30):
 			print(string)
        		time_old = time_now

			try :
            			sock.send(string)
				
        		except :
				print ("send error")
                                
#connecting bluetooth session between edison and android
def detectBLCON(args):
    global uuid
    global addr
    global sock
    sock.close()
    
    string = "conecting..."
    print(string)
    service_matches = find_service( uuid = uuid, address = addr )
    print("service is found.")
    if len(service_matches) == 0:
        print("couldn't find the SampleServer service")
        sys.exit(0)
            
    first_match = service_matches[0]
    print("first match")
    port = first_match["port"]
    name = first_match["name"]
    host = first_match["host"]
    print("trying to connect")
    try :
	sock=BluetoothSocket( RFCOMM )
    	sock.connect((host, port))
    	print("connecting to \"%s\" on %s" % (name, host))
    except :
	print("die...")

#detect open or closing
pin.isr(mraa.EDGE_BOTH, detectVoltage, detectVoltage)
print("setting detectVoltage")
#connecting bluetooth
blc.isr(mraa.EDGE_BOTH, detectBLCON, detectBLCON)
print("setting detectBLCON")

service_matches = find_service( uuid = uuid, address = addr )

if len(service_matches) == 0:
    print("couldn't find the SampleServer service")
    sys.exit(0)
            
first_match = service_matches[0]
port = first_match["port"]
name = first_match["name"]
host = first_match["host"]

sock.connect((host, port))
print("connecting to \"%s\" on %s" % (name, host))

#sleep test
raw_input()
sock.close()


