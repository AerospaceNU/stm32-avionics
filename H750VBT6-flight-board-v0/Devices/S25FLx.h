/*
 * S25FLx.h
 *
 *  Created on: Jun 28, 2020
 *      Author: bph71
 */

#ifndef INC_S25FLX_H_
#define INC_S25FLX_H_

#ifdef __cplusplus
extern "C"{
#endif

#define WREN        0x06    /* Write Enable */
#define WRDI        0x04    /* Write Disable */
#define RDSR        0x05    /* Read Status Register */
#define WRSR        0x01    /* Write Status Register */
#define READ        0x03    /* Read Data Bytes  */
#define FAST_READ   0x0b    /* Read Data Bytes at Higher Speed //Not used as as the 328 isn't fast enough  */
#define PP          0x02    /* Page Program  */
#define SE          0x20    /* Sector Erase (4k)  */
#define BE          0x20    /* Block Erase (64k)  */
#define CE          0xc7    /* Erase entire chip  */
#define DP          0xb9    /* Deep Power-down  */
#define RES         0xab    /* Release Power-down, return Device ID */
#define RDID        0x9F      /* Read Manufacture ID, memory type ID, capacity ID */

void flash_init();
void read_info();
void write(unsigned long loc, uint8_t* array, unsigned long length);
void read(unsigned long loc, uint8_t* array, unsigned long length);
void erase_256k(unsigned long loc);
void erase_all();
void waitforit();

#ifdef __cplusplus
}
#endif

#endif /* INC_S25FLX_H_ */
