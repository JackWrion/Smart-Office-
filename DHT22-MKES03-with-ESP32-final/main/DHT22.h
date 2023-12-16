/*
 * DHT22.h
 *
 *  Created on: 16 Nov 2023
 *      Author: DLCH
 */

/*
	DHT22 temperature sensor driver
*/

#ifndef DHT22_H_
#define DHT22_H_

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2


// == function prototypes =======================================

void 	setDHTgpio(int gpio);
void 	errorHandler(int response);
int 	readDHT();
float 	getHumidity();
float 	getTemperature();
int 	getSignalLevel( int usTimeOut, bool state );
void 	DHT_reader_task(void *pvParameter);

#endif
