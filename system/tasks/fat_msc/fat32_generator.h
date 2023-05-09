/*
 * fat32_generator.h
 *
 *  Created on: May 8, 2023
 *      Author: mmorley@na.jnj.com
 */

#ifndef TASKS_FAT_MSC_FAT32_GENERATOR_H_
#define TASKS_FAT_MSC_FAT32_GENERATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

void mapFlashToClusters();
void retrieveSector(uint32_t mscAddress, uint32_t lengthBytes,
		uint8_t *pSector);

#ifdef __cplusplus
}
#endif

#endif /* TASKS_FAT_MSC_FAT32_GENERATOR_H_ */
