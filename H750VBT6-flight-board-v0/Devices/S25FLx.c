/*
 * S25FLx.c
 *
 *  Created on: Jun 28, 2020
 *      Author: ben helfrich
 *
 *      adapted from https://github.com/BleepLabs/S25FLx/blob/master/S25FLx.cpp
 */


#include "stm32h7xx_hal.h"
#include "S25FLx.h"

uint8_t chip_info[3];

unsigned long prev;

//A great little tool for printing a byte as binary without it chopping off the leading zeros.
//from http://forum.arduino.cc/index.php/topic,46320.0.html

extern SPI_HandleTypeDef hspi1;

void flash_init(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, SET);
}

uint8_t flash_SPI_transceive(uint8_t *txBuf, uint8_t *rxBuf){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, RESET);
	HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, 2, HAL_MAX_DELAY);
	while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, SET);
	return 0;
}

uint8_t flash_SPI_transmit(uint8_t *txBuf, uint8_t *rxBuf){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, RESET);
	HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, 0x01, HAL_MAX_DELAY);
	while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, SET);
	return 0;
}

uint8_t flash_SPI_transceive_ncs(uint8_t *txBuf, uint8_t *rxBuf){
	HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, 0x01, 50);
	while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);

	return 0;
}

void csl(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, RESET);
}

void csh(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, SET);
}

//read and return the status register.
uint8_t stat(){                            //check status register
	uint8_t rxBuf[] = {0x00, 0x00};
	uint8_t txBuf[] = {RDSR, 0x00};

	flash_SPI_transceive(txBuf, rxBuf);

	return rxBuf[1];
}

// use between each communication to make sure S25FLxx is ready to go.
void waitforit(){
	uint8_t s=stat();
	while ((s & 0x01)==0x01){    //check if WIP bit is 1

		/*if ((millis()-prev)>1000){
			prev=millis();
		}*/
		HAL_Delay(50);

		s=stat();
	}

}


// Must be done to allow erasing or writing
void write_enable(){
	uint8_t rxBuf;
	uint8_t txBuf = WREN;

	flash_SPI_transmit(&txBuf, &rxBuf);

	waitforit();

}


// Erase an entire 4k sector the location is in.
// For example "erase_4k(300);" will erase everything from 0-3999.
//
// All erase commands take time. No other actions can be preformed
// while the chip is errasing except for reading the register
// NOT SUPPORTED
void erase_4k(unsigned long loc){

	waitforit();
	write_enable();

	uint8_t rxBuf;
	uint8_t txBuf = 0x20;

	csl();
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf= (loc>>16);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc>>8);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc & 0xFF);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	csh();

	waitforit();
}

// Errase an entire 64_k sector the location is in.
// For example erase4k(530000) will erase everything from 524543 to 589823.
// NOT SUPPORTED
void erase_64k(unsigned long loc){

	waitforit();
	write_enable();

	uint8_t rxBuf;
	uint8_t txBuf = 0x20;
	csl();
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf= (loc>>16);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc>>8);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc & 0xFF);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	csh();
	waitforit();
}

// Erase an entire 256_k sector the location is in.
void erase_sector(unsigned long loc){

	waitforit();
	write_enable();

	uint8_t rxBuf;
	uint8_t txBuf = 0xD8;
	csl();
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf= (loc>>16);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc>>8);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc & 0xFF);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	csh();
	waitforit();
}

//errases all the memory. Can take several seconds.
void erase_all(){
	uint8_t rxBuf;
	uint8_t txBuf = CE;

	waitforit();
	write_enable();

	flash_SPI_transmit(&txBuf, &rxBuf);

	waitforit();

}




// Read data from the flash chip. There is no limit "length". The entire memory can be read with one command.
//read_S25(starting location, array, number of bytes);
void read(unsigned long loc, uint8_t* array, unsigned long length){

	uint8_t rxBuf;
	uint8_t txBuf = READ;

	csl();
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc>>16);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc>>8);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = (loc & 0xff);
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);


	for (int i=0; i<length+1;i++){
		txBuf = 0x00;
		flash_SPI_transceive_ncs(&txBuf, &rxBuf);  //send the data
		array[i] = rxBuf;

	}
	csh();
}

// Programs up to 256 bytes of data to flash chip. Data must be erased first. You cannot overwrite.
// Only one continuous page (256 Bytes) can be programmed at once so there's some
// sorcery going on here to make it not wrap around.
// It's most efficient to only program one page so if you're going for speed make sure your
// location %=0 (for example location=256, length=255.) or your length is less that the bytes remain
// in the page (location =120 , length= 135)


//write_S25(starting location, array, number of bytes);
void write(unsigned long loc, uint8_t* array, unsigned long length){

	uint8_t rxBuf;
	uint8_t txBuf;



	if (length>255){
		unsigned long reps=length>>8;
		unsigned long length1;
		unsigned long array_count;
		unsigned long first_length;
		unsigned remainer0=length-(256*reps);
		unsigned long locb=loc;

		for (int i=0; i<(reps+2);i++){

			if (i==0){

				length1=256-(locb & 0xff);
				first_length=length1;
				if (length1==0){i++;}
				array_count=0;
			}

			if (i>0 && i<(reps+1)){
				locb= first_length+loc+(256*(i-1));;

				array_count=first_length+(256*(i-1));
				length1=255;

			}
			if (i==(reps+1)){
				locb+=(256);
				array_count+=256;
				length1=remainer0;
				if (remainer0==0){break;}

			}



			write_enable();
			waitforit();

			csl();
			txBuf=PP;
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (locb>>16);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (locb>>8);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (locb & 0xFF);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);

			for (unsigned long i=array_count; i<(length1+array_count+1) ; i++){
				txBuf = array[i];
				flash_SPI_transceive_ncs(&txBuf, &rxBuf);  //send the data
			}

			csh();
			waitforit();



		}
	}

	if (length<=255){
		if (((loc & 0xff)!=0) | ((loc & 0xff)<length)){
			uint8_t remainer = loc & 0xff;
			uint8_t length1 =256-remainer;
			//uint8_t length2 = length-length1;
			unsigned long page1_loc = loc;
			unsigned long page2_loc = loc+length1;

			write_enable();
			waitforit();


			csl();
			txBuf=PP;
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (page1_loc>>16);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (page1_loc>>8);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (page1_loc & 0xFF);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);

			for (int i=0; i<length+1;i++){
				txBuf = array[i];
				flash_SPI_transceive_ncs(&txBuf, &rxBuf);

			}
			csh();

			waitforit();
			write_enable();

			waitforit();


			csl();
			txBuf=PP;
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (page2_loc>>16);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (page2_loc>>8);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (page2_loc & 0xFF);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);

			for (int i=length1; i<length+1;i++){
				txBuf = array[i];
				flash_SPI_transceive_ncs(&txBuf, &rxBuf);

			}
			csh();

			waitforit();

		}



		else{


			write_enable(); // Must be done before writing can commence. Erase clears it.
			waitforit();

			csl();
			txBuf=PP;
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (loc>>16);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (loc>>8);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			txBuf = (loc & 0xFF);
			flash_SPI_transceive_ncs(&txBuf, &rxBuf);

			/*digitalWriteFast(cs,LOW);
			SPI.transfer(PP);
			SPI.transfer(loc>>16);
			SPI.transfer(loc>>8);
			SPI.transfer(loc & 0xff);*/

			for (int i=0; i<length+1;i++){
				txBuf = array[i];
				flash_SPI_transceive_ncs(&txBuf, &rxBuf);
			}

			csh();
			waitforit();
		}
	}
}


//Used in conjuture with the write protect pin to protect blocks.
//For example on the S25FL216K sending "write_reg(B00001000);" will protect 2 blocks, 30 and 31.
//See the datasheet for more. http://www.mouser.com/ds/2/380/S25FL216K_00-6756.pdf
void write_reg(uint8_t w){

	uint8_t rxBuf;
	uint8_t txBuf;

	csl();
	txBuf=WRSR;
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = w;
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	csh();

	/*digitalWriteFast(cs,LOW);
	SPI.transfer(WRSR);
	SPI.transfer(w);
	digitalWriteFast(cs,HIGH);*/
}

void read_info(){

	uint8_t txBuff[] = {0x9f, 0x00, 0x00, 0x00};
	uint8_t rxBuff[4];

	csl();
	//txBuf=0x9F;
	/*flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	txBuf = 0;
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	chip_info[0] = rxBuf; //mfg id
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	chip_info[1] = rxBuf; //mem type
	flash_SPI_transceive_ncs(&txBuf, &rxBuf);
	chip_info[2] = rxBuf; //capacity*/
	HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, 4, 500);
	csh();



	/*digitalWriteFast(cs,LOW);
	SPI.transfer(0x9F);
	//  SPI.transfer(0);
	byte m=  SPI.transfer(0);
	byte t = SPI.transfer(0);
	byte c = SPI.transfer(0);
	digitalWriteFast(cs,HIGH);*/


	/*while (c==0){
		Serial.println("Cannot read S25FL. Check wiring");
	}

	Serial.print("Manufacturer ID: ");
	Serial.print(m);
	Serial.print("     Memory type: ");
	Serial.print(t);
	Serial.print("     Capacity: ");
	Serial.println(c);
	Serial.println();
	waitforit();*/

}
