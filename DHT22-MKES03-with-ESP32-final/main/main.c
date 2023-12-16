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
//#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
//#include "addr_from_stdin.h"



#include "wifi.h"
#include "DHT22.h"
#include "MKES03.h"


//#define HOST_IP_ADDR "115.78.92.253"
#define HOST_IP_ADDR "192.168.82.176"
#define PORT 9006



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


            /*TODO : PROCESS SENSOR*/
            readDHT();
            float humi = getHumidity();
            float temp = getTemperature();
            float light = readSensorValue(NULL);

            /*TODO : CREATE DATA*/

            // data = {"temp":"26", "humi":"65", "light":"60"}
            char data[100] = {};
            sprintf(data, "{\"temp\":\"%.2f\", \"humi\":\"%.2f\", \"light\":\"%.2f\"}",temp,humi,light);

            int err = sendto(sock,    data     ,  strlen(data)       , 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));



			if (err < 0) {
				ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
				break;
			}
            ESP_LOGI(TAG, "Message sent");
            vTaskDelay(12000 / portTICK_PERIOD_MS);
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




    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
}
