#include "minmea.h"
#include "string.h"
#include "stm32h7xx_hal.h"
#include "hardware_manager.h"

#define GPS_RX_BUF_SIZE			4096
#define GPS_RX_BUF_HALF			2048

typedef struct GPSCtrl_t {

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

    char rx_buff[GPS_RX_BUF_SIZE];
} GPSCtrl_t;

void parseString(GPSCtrl_t* gps);

void usart_process_half_data(void* gps);
void usart_process_data(void* gps);

void gps_init(GPSCtrl_t* gps);
