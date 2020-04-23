
#ifndef SAPI_H
#define SAPI_H
#include "stdbool.h"

typedef bool bool_t;

typedef enum{
	CH1
} adcMap_t; 

typedef enum{
	GPIO0,
	GPIO1,
	GPIO2,
	GPIO3
}gpioMap_t;


bool_t gpioWrite( gpioMap_t pin, bool_t value );

uint16_t adcRead( adcMap_t analogInput );

float mpu9250GetGyroX_rads(void);
float mpu9250GetGyroY_rads(void);
float mpu9250GetGyroZ_rads(void);


#endif
