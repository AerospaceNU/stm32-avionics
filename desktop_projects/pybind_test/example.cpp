#include <array>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <iostream>
#include "ti_fec.h"

#ifdef __cplusplus
extern "C" {
#endif

int main() {
  FecEncoder encoder;
  std::array<uint8_t, 48> in;
  std::fill(in.begin(), in.end(), 0);
  in[0] = 3;
  in[1] = 1;
  in[2] = 2;
  in[3] = 3;

  encoder.Encode(in.data(), in.size());
  for (int i = 0; i < in.size(); i++) {
    printf("%u ", in[i]);
  }
  printf("\n");

  {
    std::array<uint8_t, in.size()> out;
    FecDecoder decoder;
    auto start = std::chrono::high_resolution_clock::now();
    decoder.FecDecode(encoder.OutputArray(), out.data(), out.size());
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Total FEC time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
  }


  for (int i = 0; i < encoder.OutputSize(in.size()); i++) {
    printf("%u ", encoder.OutputArray()[i]);
  }
  printf("\n");
}

bool fec_encode(uint8_t* in, uint8_t* out, size_t inLen) {
  FecEncoder encoder;
  encoder.Encode(in, inLen);
  std::memcpy(out, encoder.OutputArray(), encoder.OutputSize(inLen));
  return true;
}

bool fec_decode(uint8_t* in, uint8_t* out, size_t decodedLen) {
  FecDecoder decoder;
  decoder.FecDecode(in, out, decodedLen);
  return true;
}

#ifdef __cplusplus
}
#endif
