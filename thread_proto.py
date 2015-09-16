# you must unblock bluetooth device before running this program.

# $ rfkill unblock bluetooth

#import mraa
import threading
import time
import datetime
import sys
from bluetooth import *


#bluetooth server 
def bl_server(uuid):
    server_sock=BluetoothSocket( RFCOMM )
    server_sock.bind(("",PORT_ANY))
    server_sock.listen(1)

    port = server_sock.getsockname()[1]
    
    
    advertise_service( server_sock, "SampleServer",
                       service_id = uuid,
                       service_classes = [ uuid, SERIAL_PORT_CLASS ],
                       profiles = [ SERIAL_PORT_PROFILE ], 
#                   protocols = [ OBEX_UUID ] 
    )
    print("bl server starts")               
    print("Waiting for connection on RFCOMM channel %d" % port)
    
    client_sock, client_info = server_sock.accept()
    print("Accepted connection from ", client_info)
    
    try:
        while True:
            data = client_sock.recv(1024)
            if len(data) == 0: break
            print("received [%s]" % data)
    except IOError:
        pass

        print("disconnected")

        
        client_sock.close()
        server_sock.close()
        print("all done")

#bluetooth client
def bl_client(uuid):
    if sys.version < '3':
        input = raw_input
        
        addr = None
    
        if len(sys.argv) < 2:
            print("no device specified.  Searching all nearby bluetooth devices for")
            print("the SampleServer service")
        else:
            addr = sys.argv[1]
            print("Searching for SampleServer on %s" % addr)
            
        # search for the SampleServer service

  
        service_matches = find_service( uuid = uuid, address = addr )
        
        if len(service_matches) == 0:
            print("couldn't find the SampleServer service")
            sys.exit(0)
            
        first_match = service_matches[0]
        port = first_match["port"]
        name = first_match["name"]
        host = first_match["host"]
            
        print("connecting to \"%s\" on %s" % (name, host))
            
        # Create the client socket
        sock=BluetoothSocket( RFCOMM )
        sock.connect((host, port))

        print("connected.  type stuff")

        #pin Input and Output setting.
        #pin = mraa.Gpio(14)
        #pin.dir(mraa.DIR_IN)
        #pin.isr(mraa.EDGE_BOTH, detectVoltage, sock)


        while True:
            data = input()
            if len(data) == 0: break
            sock.send(data)

        sock.close()

#def detectVoltage(args):
 # args.send(x.read())


                
# main  
if __name__ == '__main__':
   
    uuid = "00000000-0000-0000-0000-000000000001"
    #bluetooth server starts.
    bl_server = threading.Thread(target=bl_server, name="bl_server",args=(uuid,))
    #bluetooth client starts.
    bl_server.start()

    bl_client(uuid)
    
