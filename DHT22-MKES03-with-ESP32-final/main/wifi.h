/*
 * wifi.h
 *
 *  Created on: 14 thg 11, 2023
 *      Author: JACKWR
 */


/*
 *  User guide.
 *
 *  1. Add file "wifi.c" and "wifi.h" in the same folder of "main.c"
 *  2. Config CMakeLists.txt:
 *
 *  	idf_component_register(SRCS "main.c" "wifi.c"
                    INCLUDE_DIRS ".")
 *
 *	3. Simple to use:
 *		- Call function   wifi_connect("your ssid","your password")
 *		- In term of check whether Connected or Not, please check "WIFI_FLAG"
 *
 *
 *
*/


#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_



#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <string.h>


/* USER DEFINE */
#define DEFAULT_SSID 	"Jackwrion"
#define DEFAULT_PWD 	"16122002"

extern int WIFI_FLAG;

void wifi_connect(const char userSSID[], const char userPWD[]);



#endif /* MAIN_WIFI_H_ */
