#include "sx126x_driver-master/SX126x.hpp"
#include "main.h"
#include "radio_packet_types.h"
#include "packet_encoder.h"
#include "radio_manager.h"

#pragma GCC optimize("Ofast")

void LED_on() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); }
void LED_off() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }

SX126x radio;

SX126x::ModulationParams_t params;

uint32_t ComputeRfFreq(double frequencyMhz) {
    return (uint32_t)(frequencyMhz * 1048576L); //2^25/(32e6)
}
void delay_cycles(uint32_t cycles) {
    //34 s for 100*30000 cycles
    //about 11 us per cycle
    // We want 125 - 40 us
    // Or about 125 - 40
    // Try 8 cycles
    for (size_t i = 0; i < cycles; i++) {
        asm("NOP");
    }
}

#define CENTER_FREQ 433

// x 2 to convert frequency in hz to bitrate (where the bits are a preamble of 101010101...)
static const double MIDDLE_C = 261.64;
#define FREQ(NOTE, OCT) lround(MIDDLE_C * pow(2, (double) (OCT * 12 + NOTE) / 12.0) * 2)

extern "C" HAL_StatusTypeDef SUBGHZSPI_Transmit(SUBGHZ_HandleTypeDef *hsubghz, uint8_t Data);
extern "C" SUBGHZ_HandleTypeDef hsubghz;

inline uint32_t play(uint8_t note, int octave, unsigned long durationMs) {
	auto end = HAL_GetTick() + durationMs;
	auto noteHz = FREQ(note, octave-4);

	static const auto deviation = 0.003;

	// sample at 4khz, or 1ms
	// sin(2 pi f) does one cycle in 1/f, or 4000/f
	size_t bound = 4000 / noteHz;
	uint32_t lut[bound];
	for (size_t i = 0; i < bound; i++) {
		auto freq = CENTER_FREQ
				+ deviation * sin(2.0 * 3.141592 * (float) i / (float) bound);
		lut[i] = ComputeRfFreq(freq);
	}

	// Sample at 1khz? maybe?
	size_t i = 0;
	radio.SetTxContinuousWave();
	while (HAL_GetTick() < end) {
		uint32_t rfFreq = lut[i % bound];
		uint8_t txbuf[5] = { 0x86, (rfFreq & 0xFF000000) >> 24, (rfFreq
				& 0x00FF0000) >> 16, (rfFreq & 0x0000FF00) >> 8, rfFreq
				& 0x000000FF };

		/* NSS = 0 */
		LL_PWR_SelectSUBGHZSPI_NSS();

		for (uint16_t i = 0U; i < 5; i++) {
			(void) SUBGHZSPI_Transmit(&hsubghz, txbuf[i]);
		}

		/* NSS = 1 */
		LL_PWR_UnselectSUBGHZSPI_NSS();

		i++;

		delay_cycles(50);
	}
	radio.SetStandby(SX126x::STDBY_XOSC);

	return i;
}

inline void rest(unsigned long durationMs) {
	radio.SetStandby(SX126x::STDBY_XOSC);
	HAL_Delay(durationMs);
}

extern "C" void entrypoint(void) {
	LED_on();
	HAL_Delay(10);
  radio.Init();
  HAL_Delay(10);
  radio.SetStandby(SX126x::STDBY_RC);

  // ramp time copied from elvin code
  radio.SetTxParams(-9, SX126x::RADIO_RAMP_40_US);
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
		//radio.SetTxInfinitePreamble();

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
