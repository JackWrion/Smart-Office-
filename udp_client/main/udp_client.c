/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "addr_from_stdin.h"



#include "wifi.h"
#include "camera.h"
#include "aes.h"



//#define HOST_IP_ADDR "115.78.92.253"
#define HOST_IP_ADDR "192.168.56.176"
#define PORT 9006

// Global key and iv
uint8_t key[16] = {29, 63, 232, 131, 224, 66, 36, 115, 92, 2, 103, 197, 50, 173, 179, 235};
uint8_t iv[16] = {190, 158, 23, 152, 28, 65, 18, 99, 124, 75, 84, 174, 159, 31, 32, 241};


static const char *TAG = "UDP_client: ";


static void udp_client_task(void *pvParameters)
{
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;


        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

        while (1) {



//            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
//            socklen_t socklen = sizeof(source_addr);
//            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
//
//            // Error occurred during receiving
//            if (len < 0) {
//                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
//                break;
//            }
//            // Data received
//            else {
//                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
//                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
//                ESP_LOGI(TAG, "%s", rx_buffer);
//                if (strncmp(rx_buffer, "OK: ", 4) == 0) {
//                    ESP_LOGI(TAG, "Received expected message, reconnecting");
//                    break;
//                }
//            }



            ESP_LOGI(TAG_CAMERA, "Taking picture...");
            camera_fb_t *pic = esp_camera_fb_get();
            ESP_LOGI(TAG_CAMERA, "Picture taken! Its size was: %zu bytes", pic->len);

            // Encrypt the image
            uint8_t *img_enc = aes_cbc_encrypt(pic->buf, pic->len, key, iv);
            int img_enc_len = length_after_pad(pic->len);

            // Send the encrypted image instead of the original image
            int err = sendto(sock, img_enc, img_enc_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
			if (err < 0) {
				ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
				break;
			}
            ESP_LOGI(TAG, "Message sent %d", img_enc_len);


            esp_camera_fb_return(pic);
            vTaskDelay(300 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}




void app_main(void)
{

	wifi_connect("Vjppro", "1111.1111");
	while (!WIFI_FLAG){
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	while (init_camera() != ESP_OK){
		vTaskDelay(pdMS_TO_TICKS(5000));
	}


    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
}
