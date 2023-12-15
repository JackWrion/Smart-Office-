import socket
import os
import queue
import threading
import cv2
import numpy as np

# Define the server IP address and port

server_ip = "0.0.0.0"
server_port = 9006              #port 9006 is UDP, port 9008 is TCP

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# Bind the socket to the server IP and port
sock.bind((server_ip, server_port))




# maximum 10 clients in Queue
queue = queue.Queue(1000)



#flag to stop UPD_handler
exit_thread = False



def UDP_handler(sock):

    print("UDP handler is running")

    while not exit_thread:
        try:
            client = queue.get(False)
            data = client[0]
            

            #------ DATA PROCESSING --------#
            with open("image.bin", 'wb') as binary_file:
                # Write the binary data to the file
                binary_file.write(data)
            
            
            data = cv2.imdecode(np.frombuffer(data, dtype=np.uint8), cv2.IMREAD_COLOR)

            data_len = len(data)
            cv2.imshow('Video', data)
            cv2.waitKey(1)
            
            
            
            
            #----- DATA RESPONDING -------#
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