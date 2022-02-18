/*
 * ActualFlightCode.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: matth
 */

#include "CC1120.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"
#include "spi.h"
#include "gpio.h"
#include "stm32f401xc.h"
#include "radioconfig/smartrf_CC1120_cfg_434_1_2kbps.h"

void FlightMain() {
	// Do init things. These should be changed!
	CC1120Ctrl_t radio;
	radio.radhspi = &hspi1;
	radio.CS_port = RAD_CS_GPIO_Port;
	radio.CS_pin = RAD_CS_Pin;
	radio.RST_port = RAD_RST_GPIO_Port;
	radio.RST_pin = RAD_RST_Pin;
	radio.MISO_port = GPIOA;
	radio.MISO_pin = GPIO_PIN_6;
	radio.settingsPtr = cc1120_433_1_2kbps_cfg;
	radio.settingsSize = sizeof(cc1120_433_1_2kbps_cfg);
//	radio.GP0_port = 0;
//	radio.GP0_pin = 0;
//	radio.GP2_port = 0;
//	radio.GP2_pin = 0;
	radio.GP3_port = RAD_GP3_GPIO_Port;
	radio.GP3_pin = RAD_GP3_Pin;
	radio.payloadSize = 48; // To match groundstation
	radio.initialized = 0;
	cc1120_init(&radio);

	while (1) {
		// Test the GPS
		uint8_t pdata[255];
		uint16_t rxlen = 255; // This tells us how many bytes we _actually_ got
		// This waits until the GPS stops sending us data, or 2 seconds
		HAL_UARTEx_ReceiveToIdle(&huart1, pdata, rxlen, &rxlen, 2000);
		printf("Got string: "); printf((char*)pdata);

		// We could try transmitting something
		char* str = "Hello, world!\n";
		memcpy(radio.packetToTX, str, strlen(str));

		// the state function needs to be called for the radio to actually do things
		cc1120State(&radio);

		// Wait a bit
		HAL_Delay(100);
	}
}
