#ifndef MS5607_H_

#define MS5607_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"
#include "gpio.h"


typedef struct {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* port;
	uint16_t pin;
} SPICtrl_t;


typedef struct {
	double temp;
	double baro;
	double alt;
} AltData_t;

typedef struct MS5607Ctrl_s{
	SPICtrl_t spiconfig;
	AltData_t altData;
} MS5607Ctrl_t;

void MS5607_init(MS5607Ctrl_t *altCtrl);
void MS5607_get_data(MS5607Ctrl_t *altCtrl);

#ifdef __cplusplus
}
#endif

#endif
