#include "minmea.h"
#include "string.h"
#include "stm32h7xx_hal.h"




typedef struct GPSCtrl_s{

    UART_HandleTypeDef* gps_uart;
    DMA_HandleTypeDef* gpa_dma_rx;

    float latitude;
    float longitude;
    float speed;
    float course;
    float altitude;
    float latitude_deviation;
    float longitude_deviation;
    float altitude_deviation;
    float speed_kph;
    float speed_knots;
    int seconds;
    int minutes;
    int hours;
    int day;
    int month;
    int year;
    int num_sats;
    char status;

    char rx_buff[4096];


}GPSCtrl_t;

void parseString(GPSCtrl_t* gps, char line[]);

void usart_process_data(GPSCtrl_t* gps, char* buff, int len);

void gps_init(GPSCtrl_t* gps);
