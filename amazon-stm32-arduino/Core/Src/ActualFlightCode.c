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

void FlightMain() {
	// Do init things. These should be changed!
	CC1120Ctrl_t radio;
	radio.radhspi = 0;
	radio.CS_port = 0;
	radio.CS_pin = 0;
	radio.RST_port = 0;
	radio.RST_pin = 0;
	radio.MISO_port = 0;
	radio.MISO_pin = 0;
	radio.GP0_port = 0;
	radio.GP0_pin = 0;
	radio.GP2_port = 0;
	radio.GP2_pin = 0;
	radio.GP3_port = 0;
	radio.GP3_pin = 0;
	radio.payloadSize = 48; // To match groundstation
	radio.initialized = 0;

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
		HAL_Delay(1000);
	}
}
