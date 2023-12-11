/*
 * wifi.c
 *
 *  Created on: 14 thg 11, 2023
 *      Author: JACKWR
 */

#ifndef MAIN_WIFI_C_
#define MAIN_WIFI_C_

#include "wifi.h"


int WIFI_FLAG = 0;





void event_wifi_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    	ESP_LOGI("WIFI","STA START");
    	WIFI_FLAG = 0;
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    	ESP_LOGI("WIFI","STA DISCONNECT");
    	WIFI_FLAG = 0;
    	vTaskDelay(pdMS_TO_TICKS(5000));
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        WIFI_FLAG = 1;
    	ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("Scan", "ESP32 got IP:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}



/* Initialize Wi-Fi as sta and set scan method */
void wifi_connect(const char* userSSID, const char* userPWD)
{

	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
	    ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_wifi_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_wifi_handler, NULL, NULL));

    // Initialize default station as network interface instance (esp-netif)
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    // Initialize and start WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .scan_method = WIFI_FAST_SCAN,
            .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
            .threshold = {
            		.rssi = -127,
            		.authmode = WIFI_AUTH_OPEN,
            },
        },
    };
    memcpy(wifi_config.sta.ssid, userSSID, sizeof(wifi_config.sta.ssid)-1);
    memcpy(wifi_config.sta.password, userPWD, sizeof(wifi_config.sta.password)-1);


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}








#endif /* MAIN_WIFI_C_ */
