/*
 * rs-fec.h
 *
 *  Created on: Nov 27, 2022
 *      Author: mcmorley
 */

#ifndef RS_FEC_H_
#define RS_FEC_H_

// Driven by CC1120 max packet len
#define RS_MAX_MSG_LEN 64
#define RS_MAX_ECC_LEN 64

#ifdef __cplusplus
extern "C" {
#endif

	void *fec_create(uint8_t msg_length, uint8_t ecc_length);
	void fec_encode(void *prs, const void* src, void* dst);
	void fec_decode(void *prs, const void* src, void* dst);

#ifdef __cplusplus
}
#endif

#endif /* RS_FEC_H_ */
