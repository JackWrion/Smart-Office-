# GATEWAY
GateWay folder is used for Device, which drives PYTHON enviroment, such as RASHBERRY PI, JETSONNANO, and Your PC
This gateway works as a UDP server.

main.py is the main file, which includes:
  + `AI_lib`: recognize the face from ESP32-Camera
  + `NBIoT`: communicate with nbiot via UART, in order to send data to platform Thingboard.
  + `AES`: which is used for decrypting the image data from ESP32.

# UDP_CLIENT
udp_client folder include soure code which you can flash to ESP32-CAMERA.

Briefly, It takes a photo and send to gateway via WIFI local, using UDP.
In order to encrypt the data sending, follow the folder    ``.udp_client/main/udp_client.c``  , on line 102.
```
camera_fb_t *pic = esp_camera_fb_get();
```
the data of picture is stored in  `pic->buf`    with   `pic->len`   indicate the length of picture in byte.
Then the data will send to Gateway using LOCAL WIFI. You should define the IP of the GATEWAY on line 37 at `udp_client.c`; and also in this file, please connect to WIFI LOCAL by func `wifi_connect("SSID","PWD")` in line 141.

# DHT22-MKES03-with-ESP32
this is the source code for ESP32 SENSOR CONTROLLER.
Please, go into `DHT22.c` and `MKES03.c` for the GPIO_PIN SENSORs configuration. You easily find the #define of GPIO in some first line.
Then, go into `main.c` and define GATEWAY IP, connect to WIFI as mentioned above, the same way as `UDP_CLIENT`. 
