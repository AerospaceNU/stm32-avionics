#include "sx126x_driver-master/SX126x.hpp"
#include "main.h"
#include "radio_packet_types.h"
#include "packet_encoder.h"
#include "radio_manager.h"
#include "audio_samples.h"
#include "stm32wlxx_hal_subghz.h"

#pragma GCC optimize("O3")

void LED_on() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); }
void LED_off() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }

SX126x radio;

extern SUBGHZ_HandleTypeDef hsubghz;
extern "C" HAL_StatusTypeDef SUBGHZSPI_Transmit(SUBGHZ_HandleTypeDef *hsubghz, uint8_t Data);


uint32_t freq_mapping[256];
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

extern "C" void entrypoint(void) {
	LED_on();
	HAL_Delay(10);
  radio.Init();
  HAL_Delay(10);
  radio.SetStandby(SX126x::STDBY_RC);

  // ramp time copied from elvin code
  radio.SetTxParams(-9, SX126x::RADIO_RAMP_40_US);
  radio.SetPacketType(SX126x::PACKET_TYPE_GFSK);


  const uint32_t FREQ_915 = 433000000;
  radio.SetRfFrequency(FREQ_915);
  
  FSKPacketRadioEncoder packetEncoder;

  const uint8_t PACKET_LEN = 64;
  const uint32_t BITRATE = 10000;

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

  SX126x::ModulationParams_t params;
  params.PacketType = SX126x::PACKET_TYPE_GFSK;
  params.Params.Gfsk.Bandwidth = SX126x::RX_BW_156200;
  params.Params.Gfsk.BitRate = BITRATE;
  params.Params.Gfsk.Fdev = 3000;
  // todo random copy from elvin
  params.Params.Gfsk.ModulationShaping = SX126x::MOD_SHAPING_G_BT_05;

  radio.SetModulationParams(params);

  // Copied from default cc1200 settings
  uint8_t sync[8] = {0x93, 0x0b, 0x51, 0xde};
  radio.SetSyncWord(sync);

  radio.SetBufferBaseAddresses(0, 0);

  double audio_deviation = 0.0025; // 2.5 KHz for 12.5 khz channel spacing
  for (uint16_t i = 0; i < 256; i++) {
    freq_mapping[i] = ComputeRfFreq(433.00 + audio_deviation * ((double) i / 255.0L  - 0.5L));
  }


  LED_off();



	while (1) {
        const uint8_t *audio_sample_data;
        uint32_t num_audio_samples = 0;
        audio_sample_data = audio_four;
        num_audio_samples = sizeof(audio_four);

		LED_on();

		radio.SetTxContinuousWave();
		HAL_Delay(10);

        for (uint32_t sample_num = 0; sample_num < num_audio_samples; sample_num++) {
            //SetRfFreq(freq_mapping[audio_zero[sample_num]]);

            // Default not fast enough
            //SetRfFreq(freq_mapping[audio_sample_data[sample_num]]);
            uint32_t rfFreq = freq_mapping[audio_sample_data[sample_num]];
            uint8_t txbuf[5] = {0x86, (rfFreq & 0xFF000000) >> 24, (rfFreq & 0x00FF0000) >> 16, (rfFreq & 0x0000FF00) >> 8, rfFreq & 0x000000FF};

            /* NSS = 0 */
            LL_PWR_SelectSUBGHZSPI_NSS();

            for (uint16_t i = 0U; i < 5; i++)
            {
              (void)SUBGHZSPI_Transmit(&hsubghz, txbuf[i]);
            }

            /* NSS = 1 */
            LL_PWR_UnselectSUBGHZSPI_NSS();


            delay_cycles(45); // Trial and error.
        }

		radio.SetStandby(SX126x::STDBY_XOSC);
		LED_off();
		HAL_Delay(1000);
	}
}
