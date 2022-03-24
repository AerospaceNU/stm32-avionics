#ifndef MS5607_H_

#define MS5607_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "spi.h"
#include "gpio.h"
#include "board_config.h"

/**
 * Different OSR resolutions for MS5607 ADCs, mapped to their conversion times in ms
 */
typedef enum {
	OSR_4096 = 10,
	OSR_2048 = 5,
	OSR_1024 = 3,
	OSR_512 = 2,
	OSR_256 = 1
} MS5607_OSR;

typedef struct {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* port;
	uint16_t pin;
} SPICtrl_t;


typedef struct {
	double temp;
	double baro;
} AltData_t;

typedef struct MS5607Ctrl_s{
	SPICtrl_t spiconfig;
	AltData_t altData;
} MS5607Ctrl_t;

bool MS5607_init(MS5607Ctrl_t *altCtrl);
void MS5607_get_data(MS5607Ctrl_t *altCtrl);

#ifdef __cplusplus
}
#endif

#endif
