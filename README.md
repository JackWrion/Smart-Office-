# GATEWAY
GateWay folder is used for Device, which drives PYTHON enviroment, such as RASHBERRY PI, JETSONNANO, and Your PC

go to `.gateway/main.py` on line 73, the received frame will be simply decode. You can implement the decrypt methods in there.


# UDP_CLIENT
udp_client folder include soure code which you can flash to ESP32-CAMERA.

Briefly, It takes a photo and send to gateway via WIFI local, using UDP.
In order to encrypt the data sending, follow the folder    ``.udp_client/main/udp_client.c``  , on line 102.
```
camera_fb_t *pic = esp_camera_fb_get();
```

the data of picture is stored in  `pic->buf`    with   `pic->len`   indicate the length of picture in byte.