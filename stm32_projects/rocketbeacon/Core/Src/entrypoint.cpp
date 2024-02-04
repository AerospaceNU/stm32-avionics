#include "sx126x_driver-master/SX126x.hpp"
#include "main.h"
#include "radio_packet_types.h"
#include "packet_encoder.h"
#include "radio_manager.h"

void LED_on() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); }
void LED_off() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }

SX126x radio;

SX126x::ModulationParams_t params;

// x 2 to convert frequency in hz to bitrate (where the bits are a preamble of 101010101...)
static const double MIDDLE_C = 261.64;
#define FREQ(NOTE, OCT) lround(MIDDLE_C * pow(2, (double) (OCT * 12 + NOTE) / 12.0) * 2)

inline void play(uint8_t note, int octave, unsigned long durationMs) {
	// offset by 4 since we play from middle c
	params.Params.Gfsk.BitRate = FREQ(note, octave - 2);
	radio.SetModulationParams(params);
	HAL_Delay(durationMs);
}

inline void rest(unsigned long durationMs) {
	radio.SetStandby(SX126x::STDBY_XOSC);
	HAL_Delay(durationMs);
	radio.SetTxInfinitePreamble();
}

extern "C" void entrypoint(void) {
	LED_on();
	HAL_Delay(10);
  radio.Init();
  HAL_Delay(10);
  radio.SetStandby(SX126x::STDBY_RC);

  // ramp time copied from elvin code
  radio.SetTxParams(10, SX126x::RADIO_RAMP_40_US);
  radio.SetPacketType(SX126x::PACKET_TYPE_GFSK);


  params.PacketType = SX126x::PACKET_TYPE_GFSK;
  params.Params.Gfsk.Bandwidth = SX126x::RX_BW_156200;
  params.Params.Gfsk.BitRate = 38400;
  params.Params.Gfsk.Fdev = 3000;
  // todo random copy from elvin
  params.Params.Gfsk.ModulationShaping = SX126x::MOD_SHAPING_G_BT_07;

  radio.SetModulationParams(params);

  const uint32_t FREQ_915 = 433000000;
  radio.SetRfFrequency(FREQ_915);
  
  FSKPacketRadioEncoder packetEncoder;

  const uint8_t PACKET_LEN = packetEncoder.EncodedLength();  // todo

  SX126x::PacketParams_t pparams;
  pparams.PacketType = SX126x::PACKET_TYPE_GFSK;
  pparams.Params.Gfsk.HeaderType = SX126x::RADIO_PACKET_FIXED_LENGTH;
  pparams.Params.Gfsk.CrcLength = SX126x::RADIO_CRC_OFF;  // TODO
  pparams.Params.Gfsk.PayloadLength = PACKET_LEN;
  pparams.Params.Gfsk.PreambleMinDetect =
      SX126x::RADIO_PREAMBLE_DETECTOR_08_BITS;  // TODO idk
  pparams.Params.Gfsk.PreambleLength = 0;
  pparams.Params.Gfsk.SyncWordLength = 0;
  pparams.Params.Gfsk.AddrComp = SX126x::RADIO_ADDRESSCOMP_FILT_OFF;
  pparams.Params.Gfsk.DcFree = SX126x::RADIO_DC_FREE_OFF;
  radio.SetPacketParams(pparams);

  // Copied from default cc1200 settings
  uint8_t sync[8] = {0x93, 0x0b, 0x51, 0xde};
  radio.SetSyncWord(sync);

  radio.SetBufferBaseAddresses(0, 0);

  LED_off();



#define C 0
#define Cs 1
#define D 2
#define Ds 3
#define E 4
#define F 5
#define Fs 6
#define G 7
#define Gs 8
#define A 9
#define As 10
#define B 11

  // 2 seconds per measure, 0.5 seconds per full note
#define FULL_NOTE 400
#define HALF_NOTE (FULL_NOTE/2)


  //static int i;
	while (1) {
		LED_on();
		radio.SetTxInfinitePreamble();

		play(D, 4, 250);
		play(D, 4, 250);
		play(D, 5, 250);
		rest(250);
		play(A, 4, 250);
		rest(500);
		play(Gs, 4, 250);
		rest(.25);
		play(G, 4, 250);
		rest(.25);
		play(F, 4, 500);
		play(D, 4, 250);
		play(F, 4, 250);
		play(G, 4, 250);

		radio.SetStandby(SX126x::STDBY_XOSC);
		LED_off();
		HAL_Delay(1000);
	}
}
