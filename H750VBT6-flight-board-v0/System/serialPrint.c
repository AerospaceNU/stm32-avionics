#include "serialPrint.h"

// TODO modify this function to use USB OTG
int uartprintf(const char *format, ...)
{
    //declare variable arguments list
    va_list args;

    //get the arguments list
    va_start(args, format);

    //make a buffer of size 256
    char buff[256];
    memset(buff, 0, sizeof(buff));

    //make format string
    vsnprintf(buff, 256, (char*)format, args);

    //end list
    va_end(args);

    //send string out uart
    // TODO put USB OTG print call here
    //return HAL_UART_Transmit(&huart2, (uint8_t*)buff, sizeof(buff), 10000);
    return 0;
}
