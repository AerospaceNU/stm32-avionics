#include "data_log.h"
#include "S25FLx.h"
#include "serialPrint.h"

// Sends data received from the data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata) function from data_log.c
// to the serial port.
void transmitData(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata) {
	// If there is data left in the log, print the data to serial, clear pdata, and print the rest of the data.
	while (data_log_read(flightNum, maxBytes, pdata) > 0) {
		debugprintf((const char*) pdata); // Print received data to serial
		memset(&pdata[0], 0, sizeof(&pdata)); // Clear pdata so it can be filled with the next set of data values
	}
}

// Reads flight number byte as an interrupt, stores it as a variable,
// and sends that flight's log to serial.
void sendLog(uint32_t maxBytes, uint8_t *pdata) {
	char flightNum; // Will store the flight number byte

	// If flightNum is still NULL, keep trying to receive a flight number
	while (!flightNum) {
		HAL_UART_Receive_IT(&huart2, &flightNum, 1); // Receive a flight number from UART2, and store it in flightNum
	}

	transmitData((uint32_t)flightNum, maxBytes, pdata); // Send the data of the acquired flight number to serial
}
