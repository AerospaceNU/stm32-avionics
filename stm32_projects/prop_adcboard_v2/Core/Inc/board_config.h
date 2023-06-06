/*
 * board_config.h
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#ifndef INC_BOARD_CONFIG_H_
#define INC_BOARD_CONFIG_H_

#define HAL_HEADER "stm32f4xx.h"
#define USE_STM32_HAL_HEADER
#define USE_ARM_MATH
#define USE_STM_HARDWARE_MANAGER

#define NUM_USB_STD 1


#define NUM_TC_READER 4

#define ADC1_FIRST_DUCER_ID 8
#define ADC1_LAST_DUCER_ID 14
#define ADC1_NUM_DUCER (1 + ADC1_LAST_DUCER_ID - ADC1_FIRST_DUCER_ID)

#define ADC2_FIRST_DUCER_ID 0
#define ADC2_LAST_DUCER_ID 7
#define ADC2_NUM_DUCER (1 + ADC2_LAST_DUCER_ID - ADC2_FIRST_DUCER_ID)


#endif /* INC_BOARD_CONFIG_H_ */
