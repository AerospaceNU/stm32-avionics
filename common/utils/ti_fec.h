#pragma once

#include <stddef.h>
#include <stdint.h>

#ifndef MaxMessageLen
#define MaxMessageLen 128
#endif

class FecEncoder {
 public:
  FecEncoder() = default;

  // Encode a message. CRC is assumed to have already been added.
  uint8_t* Encode(uint8_t* input, size_t inLen);

 private:
  uint8_t input[MaxMessageLen + 2];  // input buffer + Trellis Terminator
  uint8_t fec[4 * (MaxMessageLen / 2 + 1)];
  uint8_t interleaved[4 * (MaxMessageLen / 2 + 1)];
};

class FecDecoder {
 public:
  FecDecoder() { Reset(); }

  ~FecDecoder() = default;

  void Reset();

  /**
   * Decode an input buffer and store the result in the outputBuffer.
   * The input buffer should be of size ((payload length+crc)/2+1)*4, and the
   * input of size (payload length+crc)
   * @param pInputMessage where to find the FEC encoded message
   * @param pOutputBuffer where to store the decoded message
   * @param decodedMessageLen How long we expect the decoded message to be.
   */
  void FecDecode(uint8_t* pInputMessage, uint8_t* pOutputBuffer,
                 size_t decodedMessageLen);

 private:
  /**************************************************************************************************************
   * @fn fecDecode
   *
   * @brief De-interleaves and decodes a given input buffer. The input must be
   * exactly 4 bytes long. Up to 4? bytes may be stored in the output array.
   *
   * @param pOutputArray - Pointer to where to put decoded data (NULL when
   * initializing at start of packet).
   * @param pInoutArray - Pointer to received data to decode
   * @param nRemBytes remaining (decoded) bytes in the output message. If the
   * encoded message is 32 bytes long, and we've decoded 10, should be 22.
   *
   *
   * @return Number of bytes of decoded data stored at pOutputArray
   */
  unsigned short FecDecode4(unsigned char* pOutputArray,
                            unsigned char* pInputArray,
                            unsigned short nRemBytes);

  // ==== Private varibles to keep track of state, reset with Reset() between
  // messages ====

  // Two sets of buffers (last, current) for each destination state for holding:
  unsigned char nCost[2][8] = {0};  // Accumulated path cost
  unsigned long aPath[2][8] = {0};  // Encoder input data (32b window)
  // Indices of (last, current) buffer for each iteration
  unsigned char iLastBuf = 0;
  unsigned char iCurrBuf = 0;
  // Number of bits in each path buffer
  unsigned char nPathBits = 0;
};

uint16_t calculateCRC(uint8_t crcData, uint16_t crcReg);

#include "ti_fec_encode.hpp"
