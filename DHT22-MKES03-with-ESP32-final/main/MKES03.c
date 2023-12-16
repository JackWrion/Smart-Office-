/*
 * MKES03.c
 *
 *  Created on: 27 Nov 2023
 *      Author: DLCH
 */

#include "MKES03.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <driver/gpio.h>

#define INPUT_PIN_32	ADC1_CHANNEL_4

#define RED	GPIO_NUM_25
#define YELLOW GPIO_NUM_19
#define GREEN GPIO_NUM_26


int converter[] = {4000,3000,2000,1000};

static esp_adc_cal_characteristics_t adc1_chars;

//turn a led on
void set_led(gpio_num_t pin){
	gpio_set_level(RED,0);
	gpio_set_level(YELLOW,0);
	gpio_set_level(GREEN,0);
	switch(pin){
	case RED:
		gpio_set_level(RED,1);
		printf("Thieu anh sang\n");
		break;
	case YELLOW:
		gpio_set_level(YELLOW,1);
		printf("Anh sang mo\n");
		break;
	case GREEN:
		gpio_set_level(GREEN,1);
		printf("Anh sang day du\n");
		break;
	default:
		break;
	}

}

//read and print raw value [4095,0]
//return value through pvParameter. 5 level of brightness [1,5]. 5 is the highest
float readSensorValue(void* pvParameter){
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(INPUT_PIN_32, ADC_ATTEN_DB_11);

	//LED PIN, set direction
	gpio_set_direction(RED,GPIO_MODE_OUTPUT);
	gpio_set_direction(YELLOW,GPIO_MODE_OUTPUT);
	gpio_set_direction(GREEN,GPIO_MODE_OUTPUT);
	//

//	int *return_value = (int*)pvParameter;
//	while(1){
		int reading =  adc1_get_raw(INPUT_PIN_32);
//		printf("%d\n",reading);
		if(reading > converter[0]){
//			*return_value = 1;
			set_led(RED);
		}
		else if(reading <= converter[0] && reading > converter[1]){
//			*return_value = 2;
			set_led(YELLOW);
		}
		else if(reading <= converter[1] && reading > converter[2]){
//			*return_value = 3;
			set_led(GREEN);
		}
		else if(reading <= converter[2] && reading > converter[3]){
//			*return_value = 4;
			set_led(GREEN);
		}
		else {
//			*return_value = 5;
			set_led(GREEN);
		}
		float result = 100.0 - reading/4095.0*100.0;
		return result;
//		printf("%d\n",*return_value);
//		vTaskDelay(pdMS_TO_TICKS(1000));
//	}
//	vTaskDelete(NULL);
}
