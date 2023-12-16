
import serial
import time

PORT = 'COM3'
VERBOSE = 0




def NB_send(port, message, wait_time=2):
    
    ser = serial.Serial(PORT, 115200, timeout=2)
    
    if not VERBOSE:
        return
        
    
    
    # Open a serial connection
    port = PORT
    

    try:
        # Send the message
        if "SMCONN" in message:
            print(message)
            ser.write(message.encode())

            while(1):
                time.sleep(wait_time)
                response = ser.readline().decode().strip()
                if "OK" in response:
                    break
                elif "ERROR" in response:
                    print("ERROR")
                    ser.write(message.encode())
                else:
                    state = "AT+SMSTATE?\r\n"
                    ser.write(state.encode())
                    print(state)
                    response = ser.readline().decode().strip()
                    print(response)
                    if "2" in response:
                        print("OK")
                        break
        else:
            print(message)
            while(1):
                ser.write(message.encode())
                time.sleep(0.5)
                response = ser.readline().decode()
                if "OK" in response:
                    print("OK")
                    break
    finally:
        # Close the serial connection
        ser.close()
        pass




def MQTT_Init():
    
    
    if not VERBOSE:
        return
    
    serial_port = PORT
    wait_time = 1

    #check username = token
    setup = ['ATE0\r\n',"AT+CNACT=0,1\r\n", 'AT+SMCONF="URL","demo.thingsboard.io"\r\n','AT+SMCONF="USERNAME","FkJxbd2ybU9O1sTGUTjf"\r\n','AT+SMCONF="TOPIC","v1/devices/me/telemetry"\r\n','AT+SMCONF="CLIENTID","THI"\r\n']
    connect = 'AT+SMCONN\r\n'
    
    #setup
    for i in range(len(setup)):
        NB_send(serial_port, setup[i], wait_time)
    #connect
    NB_send(serial_port, connect, 15)
    pass




def MQTT_Publish_stats (data):
    ser = serial.Serial(PORT, 115200, timeout=2)
    
    if not VERBOSE:
        return
    
    #data = data.decode('latin-1')
    
    
    
    pub = 'AT+SMPUB="v1/devices/me/telemetry",'  +str(len(data))+  ',0,1\r\n'
    
    try:
        print(pub)
        ser.write(pub.encode())
        time.sleep(1)
        
        print(data)
        ser.write(data)
    # Read the response
        # while(1):
        #     response = ser.readline().decode().strip()
        #     if "OK" in response:
        #         print("OK")
        #         break
    finally:
        # Close the serial connection
        ser.close()
        pass     




def MQTT_Publish_ID(id):
    ser = serial.Serial(PORT, 115200, timeout=2)
    
    if not VERBOSE:
        return
    
    #ser = serial.Serial(PORT, 115200, timeout=2)
    
    data = '{"name":"' + str(id)  +'"}'
    pub = 'AT+SMPUB="v1/devices/me/telemetry",'  +str(len(data))+  ',0,1\r\n'
    
    try:
        print(pub)
        ser.write(pub.encode())
        time.sleep(1)
        
        print(data)
        ser.write(data.encode())
        time.sleep(5)
    # Read the response
        # while(1):
        #     response = ser.readline().decode().strip()
        #     if "OK" in response:
        #         print("OK")
        #         break
    finally:
        ser.close()
        pass        
        
        
        
def MQTT_KILL():
    
    ser = serial.Serial(PORT, 115200, timeout=2)
    try:
        #ser = serial.Serial(PORT, 115200, timeout=2)
        data = "AT+CNACT=0,0"
        ser.write(data.encode())
        time.sleep(2)
        data = "AT+SMDISC"
        ser.write(data.encode())
        ser.close()
    except:
        pass
        
    
