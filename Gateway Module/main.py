import socket
import os
import threading
from multiprocessing import Process, Queue, Value, Lock, Manager
import cv2
import numpy as np
from datetime import datetime
import time


# Personal LIB
import nbiot
import AI_lib








def Image_Display():
    print("Image Display is running")
    while True:
        if exit_thread.value:
            break
        
        try:
            frame = img_queue.get()
            cv2.imshow('Video', frame)
            cv2.waitKey(1)
        except:
            pass
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    print("EXIT DISPLAY")




def UDP_handler(sock, queue, img_queue, lock, exit_thread, current_member, nbiotqueue ):
    
    print("UDP handler is running")
    #cv2.namedWindow("Server Streaming", cv2.WINDOW_NORMAL)
    
    
    
    while True:
        with lock:
            if exit_thread.value:
                break
        
        try:
            client = queue.get()
        except:
            print ("Some wrong get in subprocess")
            continue
        jpeg_data = client[0]
        
        data_len = len(jpeg_data)
        print("Receive len: ", data_len )
        
        
        
        
        if data_len < 1000:
            nbiotqueue.put(jpeg_data)
            continue
        
        
        # Decode the JPEG image
        frame = cv2.imdecode(np.frombuffer(jpeg_data, dtype=np.uint8), cv2.IMREAD_COLOR)
        
        # Face Recognition
        face_names , frame = AI_lib.face_recognition_exe(frame)
        
        
        try:
            img_queue.put(frame)
        except:
            pass
        
        
        for id in face_names:
            if id in current_member:
                continue
            else:
                # dt = datetime.now()
                # dt = dt.strftime("%H:%M:%S")
                current_member.append(id)
                print (current_member)
                nbiotqueue.put(id)        # Publish ID moi 5min len platfrom
    
    print("EXIT UDP HANDLER")
    
    
    
    
    
    
def UDP_handler_nbiot(sock, queue, img_queue, lock, exit_thread, current_member, nbiotqueue ):
    
    print("UDP nbiot handler is running")
    #cv2.namedWindow("Server Streaming", cv2.WINDOW_NORMAL)
    
    
    
    while True:
        with lock:
            if exit_thread.value:
                break
        
        try:
            data = nbiotqueue.get()
        except:
            print ("Some wrong get in subprocess")
            continue
        
        
        
        data_len = len(data)
        print("Receive len: ", data_len )
        
        
        
        
        if data_len > 30:
            nbiot.MQTT_Publish_stats(data)
        else:
            nbiot.MQTT_Publish_ID(data) 
        
    
    print("EXIT UDP HANDLER")
                
        









if __name__ == "__main__":
    
    
    # Define the server IP address and port

    server_ip = "0.0.0.0"
    server_port = 9006              #port 9006 is UDP, port 9008 is TCP

    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Bind the socket to the server IP and port
    sock.bind((server_ip, server_port))


    # nbiot.MQTT_KILL()
    # nbiot.MQTT_Init()
    
    # ------------ The shared variable
    # maximum 100 clients in Queue
    img_queue = Queue(10000)
    queue = Queue(10000)
    nbiotqueue = Queue(10000)
    exit_thread = Value("i",0)
    lock = Lock()
    manager = Manager()
    current_member = manager.list()
    #flag to stop UPD_handler
    
    
    UDP_handler_thread1 = Process(target=UDP_handler,args=(sock, queue, img_queue, lock, exit_thread, current_member, nbiotqueue),)
    UDP_handler_thread1.start()
    UDP_handler_thread2 = Process(target=UDP_handler,args=(sock, queue, img_queue, lock, exit_thread, current_member, nbiotqueue),)
    UDP_handler_thread2.start()
    UDP_handler_thread3 = Process(target=UDP_handler_nbiot,args=(sock, queue, img_queue, lock, exit_thread, current_member, nbiotqueue),)
    UDP_handler_thread3.start()
    Image_Display_thread1 = threading.Thread(target=Image_Display,args=(),)
    Image_Display_thread1.start()


    
    
    
    

    print("UDP server listening on:  ",server_ip,":",server_port)
    while True:
        try:
            # Receive data from a client
            data, addr = sock.recvfrom(250000)  # Maximum receive buffer size is 1024 bytes
            client = [data, addr]
            queue.put(client)
            time.sleep(0.2)
            
        except KeyboardInterrupt:
        # Handle Ctrl+C to gracefully exit the loop
            with lock:
                exit_thread.value = 1
            print("Key Interrupt")
            sock.close()
            break

        except:
            with lock:
                exit_thread.value = 1
            print("Some error found")
            break
            


    UDP_handler_thread1.join()
    UDP_handler_thread2.join()
    UDP_handler_thread3.join()
    Image_Display_thread1.join()

    manager.shutdown()
    cv2.destroyAllWindows()

    nbiot.MQTT_KILL()
    
    print("Process interrupted")
    try:
        sock.close()
        exit(0)
    except SystemExit:
        sock.close()
        os._exit(0)