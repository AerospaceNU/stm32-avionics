#include "GPS.h"

#include "minmea.h"
#include "string.h"
#include "hal_callbacks.h"
#include "stdbool.h"

void parseString(GPSCtrl_t *gps, char line[]) {
	switch (minmea_sentence_id(line, false)) {
	case MINMEA_SENTENCE_RMC: {
		struct minmea_sentence_rmc frame0;
		if (minmea_parse_rmc(&frame0, line)) {
			gps->hours = frame0.time.hours;
			gps->minutes = frame0.time.minutes;
			gps->seconds = frame0.time.seconds;
			gps->speed = minmea_tofloat(&frame0.speed);
			gps->course = minmea_tofloat(&frame0.course);
			if (!isnan(minmea_tofloat(&frame0.latitude)) &&
			!isnan(minmea_tofloat(&frame0.longitude))) {

				gps->latitude = minmea_tofloat(&frame0.latitude);
				gps->longitude = minmea_tofloat(&frame0.longitude);

			}
		}
		break;
	}
	case MINMEA_SENTENCE_GGA: {
		struct minmea_sentence_gga frame1;
		if (minmea_parse_gga(&frame1, line)) {
			gps->hours = frame1.time.hours;
			gps->minutes = frame1.time.minutes;
			gps->seconds = frame1.time.seconds;
			if (!isnan(minmea_tofloat(&frame1.latitude)) &&
			!isnan(minmea_tofloat(&frame1.longitude)) &&
			!isnan(minmea_tofloat(&frame1.altitude))) {

				gps->latitude = minmea_tofloat(&frame1.latitude);
				gps->longitude = minmea_tofloat(&frame1.longitude);
				gps->altitude = minmea_tofloat(&frame1.altitude);

			}
		}

		break;
	}
	case MINMEA_SENTENCE_GLL: {
		struct minmea_sentence_gll frame2;
		if (minmea_parse_gll(&frame2, line)) {
			gps->hours = frame2.time.hours;
			gps->minutes = frame2.time.minutes;
			gps->seconds = frame2.time.seconds;
			gps->status = frame2.status;
			if (!isnan(minmea_tofloat(&frame2.latitude)) &&
			!isnan(minmea_tofloat(&frame2.longitude))) {

				gps->latitude = minmea_tofloat(&frame2.latitude);
				gps->longitude = minmea_tofloat(&frame2.longitude);

			}
		}
		break;
	}
	case MINMEA_SENTENCE_GSA: {
		break;
	}
	case MINMEA_SENTENCE_GST: {
		struct minmea_sentence_gst frame3;
		if (minmea_parse_gst(&frame3, line)) {
			gps->hours = frame3.time.hours;
			gps->minutes = frame3.time.minutes;
			gps->seconds = frame3.time.seconds;
			gps->latitude_deviation = minmea_tofloat(
					&frame3.latitude_error_deviation);
			gps->longitude_deviation = minmea_tofloat(
					&frame3.longitude_error_deviation);
			gps->altitude_deviation = minmea_tofloat(
					&frame3.altitude_error_deviation);
		}
		break;
	}
	case MINMEA_SENTENCE_GSV: {
		struct minmea_sentence_gsv frame4;
		if (minmea_parse_gsv(&frame4, line)) {
			gps->num_sats = frame4.total_sats;
		}
		break;
	}
	case MINMEA_SENTENCE_VTG: {
		struct minmea_sentence_vtg frame5;
		if (minmea_parse_vtg(&frame5, line)) {
			gps->speed_kph = minmea_tofloat(&frame5.speed_kph);
			gps->speed_knots = minmea_tofloat(&frame5.speed_knots);
		}
		break;
	}
	case MINMEA_SENTENCE_ZDA: {
		struct minmea_sentence_zda frame6;
		if (minmea_parse_zda(&frame6, line)) {
			gps->hours = frame6.time.hours;
			gps->minutes = frame6.time.minutes;
			gps->seconds = frame6.time.seconds;
			gps->microseconds = frame6.time.microseconds;
			gps->day = frame6.date.day;
			gps->month = frame6.date.month;
			gps->year = frame6.date.year;

			struct timespec ts;
			memset(&ts, 0, sizeof(ts));

			const struct minmea_date date = {gps->day, gps->month, gps->year};
			const struct minmea_time time = {gps->hours, gps->minutes, gps->seconds, gps->microseconds};

			if (!minmea_gettime(&ts, &date, &time)) {
				gps->timestamp = ts.tv_sec;
			}

		}
		break;
	}
	default: {
	}
	}
}

void gps_process_data(GPSCtrl_t *gps) {
	char *buff = &gps->rx_buff[0];
	//if half is true, increment buff pointer by 2048
	if (gps->half)
		buff += GPS_RX_BUF_HALF;

	int len = GPS_RX_BUF_HALF;

	for (int i = 0; i < len; i++) {
		if (buff[i] == '\n') {
			gps->line[gps->place] = '\n';
			parseString(gps, gps->line);
			memset(gps->line, '\0', len);
			gps->place = 0;
		} else {
			gps->line[gps->place] = buff[i];
			gps->place += 1;
		}
	}
	gps->data_available = false;
}

void gps_RxHalfCpltCallback(void *gps) {
	GPSCtrl_t *pgps = (GPSCtrl_t*) gps;
	pgps->data_available = true;
	pgps->half = false;
}

void gps_RxCpltCallback(void *gps) {
	GPSCtrl_t *pgps = (GPSCtrl_t*) gps;
	pgps->data_available = true;
	pgps->half = true;
}


//TODO: Call gps_new_data to check if new data is available and update sensorData struct
bool gps_new_data(GPSCtrl_t *gps) {
	if(gps->data_available){
		gps_process_data(gps);
		return true;
	}
	return false;
}

void gps_init(GPSCtrl_t *gps) {
	HAL_UART_Receive_DMA(gps->gps_uart, (uint8_t*) gps->rx_buff, 4096);

	register_HAL_UART_RxHalfCpltCallback(gps->gps_uart, gps_RxHalfCpltCallback,
			gps);
	register_HAL_UART_RxCpltCallback(gps->gps_uart, gps_RxCpltCallback, gps);

	uint8_t nmea[16] = {0xB5,
						0x62,
						0x06,
						0x01,
						0x08,
						0x00,
						0xF0,
						0x08,
						0x01,
						0x01,
						0x01,
						0x01,
						0x01,
						0x00,
						0x0B,
						0x6B};
	HAL_UART_Transmit(gps->gps_uart, (uint8_t*)nmea, sizeof(nmea), 1000);
/* other messages?
		uint8_t nmea2[] = {0xB5, 0x62, 0x06, 0x01, 0x02, 0x00, 0xF0, 0x08, 0x01, 0x19};

		HAL_UART_Transmit(gps->gps_uart, (uint8_t*)nmea2, sizeof(nmea2), 100);

		uint8_t nmea3[10] = {0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38};

		HAL_UART_Transmit(gps->gps_uart, (uint8_t*)nmea3, sizeof(nmea3), 100);

		uint8_t nmea4[10] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x08, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x0B, 0x6B};

		HAL_UART_Transmit(gps->gps_uart, (uint8_t*)nmea4, sizeof(nmea4), 100);

		uint8_t nmea5[10] = {0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x01, 0x0F, 0x38};

		HAL_UART_Transmit(gps->gps_uart, (uint8_t*)nmea5, sizeof(nmea5), 100);
*/
}
