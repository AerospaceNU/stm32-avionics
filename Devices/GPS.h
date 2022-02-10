#include "minmea.h"
#include "string.h"

#include "board_config.h"

#include "stdbool.h"

#define GPS_RX_BUF_SIZE			4096
#define GPS_RX_BUF_HALF			2048

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
    uint64_t timestamp;
    int seconds;
    int microseconds;
    int minutes;
    int hours;
    int day;
    int month;
    int year;
    int num_sats;
    char status;

    char rx_buff[4096];
    char line[2048];

    int place;

    bool data_available;
    bool half;


}GPSCtrl_t;

void parseString(GPSCtrl_t* gps, char line[]);

void gps_process_data(GPSCtrl_t* gps);

void gps_init(GPSCtrl_t* gps);

bool gps_new_data(GPSCtrl_t *gps);

