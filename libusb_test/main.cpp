#if 1
/**
 * @file /example1/main.cpp
 * @author Philippe Lucidarme
 * @date December 2019
 * @brief File containing example of serial port communication
 *
 * This example send a string through the serial device
 * The program expect receiving a string from the device
 * The received string is expected to terminated by a carriage return
 *
 * The following code has been tested with this Arduino Uno sketch
 * that return the uppercase character received on the serial device
 *
 * // Setup, initialize
 * void setup() {
 *      Serial.begin(115200);
 * }
 *
 * // Loop forever
 * void loop() {
 * // If serial data is pending, read and write the character uppercased
 * if (Serial.available())
 *      Serial.write( toupper (Serial.read()) );
 * }
 *
 * @see https://lucidar.me
 */


// Serial library
#include "serialib.h"
#include <stdio.h>


#if defined (_WIN32) || defined(_WIN64)
    //for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
    //also works for COM0 to COM9.
    //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN#communications-resources
    #define SERIAL_PORT "\\\\.\\COM14"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyACM0"
#endif

/*!
 * \brief main  Example of read and write string
  */
int main( /*int argc, char *argv[]*/)
{
    // Serial object
    serialib serial;

    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 115200);


    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n",SERIAL_PORT);

    // // Create the string
    char buffer[200] = "hello\n";

    // // Write the string on the serial device
    // serial.writeString(buffer);
    // printf ("String sent: %s", buffer);

    // Read the string
    while (true) {
        int rc = serial.readString(buffer, '\n', 200, 2000);
        if (rc < 0) break;
        printf("String read: %s\n", buffer);
    }

    // Close the serial device
    serial.closeDevice();

    return 0 ;
}
#endif

#if 0

// File: test.cpp
// Description: Serial (Com port) console program for Windows and Linux
// WebSite: http://cool-emerald.blogspot.sg/2017/05/serial-port-programming-in-c-with.html
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

#include <stdio.h>
#define CE_SERIAL_IMPLEMENTATION
#include "ceSerial.h"
using namespace std;

int main()
{
#ifdef CE_WINDOWS
	ceSerial com("\\\\.\\COM14",9600,8,'N',1); // Windows
#else
	ceSerial com("/dev/ttyACM0",9600,8,'N',1); // Linux
#endif

	printf("Opening port %s.\n",com.GetPort().c_str());
	if (com.Open() == 0) {
		printf("OK.\n");
	}
	else {
		printf("Error.\n");
		return 1;
	}

	bool successFlag;
	// printf("Writing.\n");
	// char s[]="Hello";
	// successFlag=com.Write(s); // write string
	// successFlag=com.WriteChar('!'); // write a character

	// printf("Waiting 3 seconds.\n");
	// ceSerial::Delay(3000); // delay to wait for a character

	printf("Reading.\n");
	for (int i = 0; i < 1000; i++) {
		ceSerial::Delay(5); // delay to wait for a character
        char outArr[2000] = {0};
        com.Read(outArr, sizeof(outArr) - 1);
        printf(outArr); // assume null-terminated
        // char c=com.ReadChar(successFlag); // read a char
        // if(successFlag) printf("%c",c);
    }

	printf("Closing port %s.\n",com.GetPort().c_str());
	com.Close();
	return 0;
}
#endif
