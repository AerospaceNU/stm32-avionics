#include "GPS.h"

#include "minmea.h"
#include "string.h"


char line[2048];


void parseString(GPSCtrl_t* gps, char line[]) {
	switch(minmea_sentence_id(line, false)) {
		    				case MINMEA_SENTENCE_RMC: {
		    				struct minmea_sentence_rmc frame0;
		    				if (minmea_parse_rmc(&frame0, line)) {
		    					gps->hours = frame0.time.hours;
		    					gps->minutes = frame0.time.minutes;
		    					gps->seconds = frame0.time.seconds;
		    					gps->speed = minmea_tofloat(&frame0.speed);
		    					gps->course = minmea_tofloat(&frame0.course);
		    					if(!isnan(minmea_tofloat(&frame0.latitude)) &&
		    					!isnan(minmea_tofloat(&frame0.longitude))) {

		    						gps->latitude = minmea_tofloat(&frame0.latitude);
		    						gps->longitude = minmea_tofloat(&frame0.longitude);

		    					}
		    				}
		    				break;
		    							  }
		    				case MINMEA_SENTENCE_GGA:{
		    				struct minmea_sentence_gga frame1;
		    				if(minmea_parse_gga(&frame1, line)) {
		    					gps->hours = frame1.time.hours;
		    					gps->minutes = frame1.time.minutes;
		    					gps->seconds = frame1.time.seconds;
		    					if(!isnan(minmea_tofloat(&frame1.latitude)) &&
				    			!isnan(minmea_tofloat(&frame1.longitude)) &&
								!isnan(minmea_tofloat(&frame1.altitude))) {

		    					gps->latitude = minmea_tofloat(&frame1.latitude);
		    					gps->longitude = minmea_tofloat(&frame1.longitude);
		    					gps->altitude = minmea_tofloat(&frame1.altitude);

		    					}
		    				}

		    				break;
		    							 }
		    				case MINMEA_SENTENCE_GLL:{
		    				struct minmea_sentence_gll frame2;
		    				if(minmea_parse_gll(&frame2, line)) {
		    					gps->hours = frame2.time.hours;
		    					gps->minutes = frame2.time.minutes;
		    					gps->seconds = frame2.time.seconds;
		    					gps->status = frame2.status;
		    					if(!isnan(minmea_tofloat(&frame2.latitude)) &&
				    			!isnan(minmea_tofloat(&frame2.longitude))) {

		    					gps->latitude = minmea_tofloat(&frame2.latitude);
		    					gps->longitude = minmea_tofloat(&frame2.longitude);

		    					}
		    				}
		    				break;
		    							 }
		    				case MINMEA_SENTENCE_GSA:{
		    				break;
		    							 }
		    				case MINMEA_SENTENCE_GST:{
		    				struct minmea_sentence_gst frame3;
		    				if(minmea_parse_gst(&frame3, line)) {
		    					gps->hours = frame3.time.hours;
		    					gps->minutes = frame3.time.minutes;
		    					gps->seconds = frame3.time.seconds;
		    					gps->latitude_deviation = minmea_tofloat(&frame3.latitude_error_deviation);
		    					gps->longitude_deviation = minmea_tofloat(&frame3.longitude_error_deviation);
		    					gps->altitude_deviation = minmea_tofloat(&frame3.altitude_error_deviation);
		    				}
		    				break;
		    							 }
		    				case MINMEA_SENTENCE_GSV:{
		    				struct minmea_sentence_gsv frame4;
		    				if(minmea_parse_gsv(&frame4, line)) {
		    					gps->num_sats = frame4.total_sats;
		    						}
		    				break;
		    							 }
		    				case MINMEA_SENTENCE_VTG:{
		    				struct minmea_sentence_vtg frame5;
		    				if(minmea_parse_vtg(&frame5, line)) {
		    					gps->speed_kph = minmea_tofloat(&frame5.speed_kph);
		    					gps->speed_knots = minmea_tofloat(&frame5.speed_knots);
		    					}
		    				break;
		    				}
		    				case MINMEA_SENTENCE_ZDA:{
		    				struct minmea_sentence_zda frame6;
		    				if(minmea_parse_zda(&frame6, line)) {
		    					gps->hours = frame6.time.hours;
		    					gps->minutes = frame6.time.minutes;
		    					gps->seconds = frame6.time.seconds;
		    					gps->day = frame6.date.day;
		    					gps->month = frame6.date.month;
		    					gps->year = frame6.date.year;
		    					}
		    				break;
		    				}
		    				default: {
		    				}
		    	}
}

void usart_process_data(GPSCtrl_t* gps, char* buff, int len) {
	for(int i = 0; i < len; i++) {
		if(buff[i] == '\r') {
			strncat(line, "\n", 1);
			parseString(gps, line);
			strcpy(line,"");
		} else {
			strncat(line,&buff[i],1);
		}
	}
}

void gps_init(GPSCtrl_t* gps) {
	HAL_UART_Receive_DMA(gps->gps_uart, (unsigned char*)gps->rx_buff, 4096);
}
