import socket
import os
import time
from datetime import datetime, timedelta
import queue
import threading
# Define the server IP address and port

server_ip = "0.0.0.0"
server_port = 9006              #port 9006 is UDP, port 9008 is TCP

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# Bind the socket to the server IP and port
sock.bind((server_ip, server_port))




# maximum 10 clients in Queue
queue = queue.Queue(100)



#flag to stop UPD_handler
exit_thread = False



def UDP_handler(sock):

    print("UDP handler is running")

    while not exit_thread:
        try:
            client = queue.get(False)
            data = client[0]
            addr = client[1]

            dt = datetime.now() + timedelta(minutes=0)
            dt = dt.strftime("%H:%M:%S")


            #------ DATA PROCESSING --------#
            data = data.decode('latin-1')
            #print(dt,"   Received: ",data, "from ", addr, "\n\n\n")
            data_len = len(data)
            data_temp = data[:int(data_len/2)+1]
            
            
            
            
            #----- DATA RESPONDING -------#
            response = "UDP_" + data_temp + "***\r\n"
            sock.sendto(response.encode("latin-1"), addr)
            response =  "{} : UDP Server response to {}\r\n".format(dt,addr)
            sock.sendto(response.encode("latin-1"), addr)
        except:
            pass




UDP_handler_thread = threading.Thread(target=UDP_handler,args=(sock,),)
UDP_handler_thread.start()



print("UDP server listening on:  ",server_ip,":",server_port)


while True:
    try:
        # Receive data from a client
        data, addr = sock.recvfrom(50000)  # Maximum receive buffer size is 1024 bytes
        client = [data, addr]
        queue.put(client)
        
    except KeyboardInterrupt:
    # Handle Ctrl+C to gracefully exit the loop
        sock.close()
        break

    except:
        pass


exit_thread = True
UDP_handler_thread.join()

print("Process interrupted")
try:
    sock.close()
    exit(0)
except SystemExit:
    sock.close()
    os._exit(0)