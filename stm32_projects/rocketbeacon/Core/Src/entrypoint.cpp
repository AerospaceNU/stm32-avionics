#include "sx126x_driver-master/SX126x.hpp"
#include "main.h"
#include "radio_packet_types.h"
#include "packet_encoder.h"
#include "radio_manager.h"
#include "gps.h"
#include "usart.h"
#include "scheduler.h"

void LED_on() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); }
void LED_off() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }

SX126x radio;
GpsCtrl_s gps;

RadioDecodedPacket_s packet;
FSKPacketRadioEncoder packetEncoder;

const uint8_t PACKET_LEN = packetEncoder.EncodedLength();

extern "C" void entrypoint(void) {
	LED_on();
	HAL_Delay(10);
  radio.Init();
  HAL_Delay(10);
  radio.SetStandby(SX126x::STDBY_RC);

  // ramp time copied from elvin code
  radio.SetTxParams(10, SX126x::RADIO_RAMP_40_US);
  radio.SetPacketType(SX126x::PACKET_TYPE_GFSK);

  SX126x::ModulationParams_t params;
  params.PacketType = SX126x::PACKET_TYPE_GFSK;
  params.Params.Gfsk.Bandwidth = SX126x::RX_BW_156200;
  params.Params.Gfsk.BitRate = 38400;
  params.Params.Gfsk.Fdev = 20000;
  // todo random copy from elvin
  params.Params.Gfsk.ModulationShaping = SX126x::MOD_SHAPING_G_BT_05;
  radio.SetModulationParams(params);

  radio.SetRfFrequency(RADIO_FREQ);

  SX126x::PacketParams_t pparams;
  pparams.PacketType = SX126x::PACKET_TYPE_GFSK;
  pparams.Params.Gfsk.HeaderType = SX126x::RADIO_PACKET_FIXED_LENGTH;
  pparams.Params.Gfsk.CrcLength = SX126x::RADIO_CRC_OFF;
  pparams.Params.Gfsk.PayloadLength = PACKET_LEN;
  pparams.Params.Gfsk.PreambleMinDetect =
      SX126x::RADIO_PREAMBLE_DETECTOR_08_BITS;  // TODO idk
  pparams.Params.Gfsk.PreambleLength = 3;
  pparams.Params.Gfsk.SyncWordLength = 4;
  pparams.Params.Gfsk.AddrComp = SX126x::RADIO_ADDRESSCOMP_FILT_OFF;
  pparams.Params.Gfsk.DcFree = SX126x::RADIO_DC_FREE_OFF;
  radio.SetPacketParams(pparams);

  // Copied from default cc1200 settings
  uint8_t sync[8] = {0x93, 0x0b, 0x51, 0xde};
  radio.SetSyncWord(sync);

  // And start our GPS doing its DMA thing
  gps_init(&gps, &huart1, GPS_TYPE_UBLOX);

  LED_off();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1) {
    // update GPS
	gps_newData(&gps);

    memcpy(packet.callsign, "KM6GNL\0\0", 8);
    packet.timestampMs = HAL_GetTick();
    packet.softwareVersion = 0;
    packet.board_serial_num = 0;
    packet.packetType = TELEMETRY_ID_POSITION;

    packet.payload.positionData.lat = gps.data.generalData.latitude;
    packet.payload.positionData.lon = gps.data.generalData.longitude;
    packet.payload.positionData.gps_alt = gps.data.generalData.altitude;
    packet.payload.positionData.gpsTime = gps.data.timeData.timestamp;
    packet.payload.positionData.sats = gps.data.generalData.satsTracked;

    packet.payload.positionData.state = Scheduler::StateId_e::PreFlight;

    packet.packetCRC = calculateRadioPacketCRC(packet);

    // Fill the TX buffer, starting at 0, to contain a packet
    // explicitly set the FIFO back to the start (page 48 of SX1261/2 datasheet)
    radio.SetBufferBaseAddresses(0, 0);


    static RadioOTAPayload_s output;
    if (0 == packetEncoder.Encode(packet, output)) {

      radio.WriteBuffer(0, (uint8_t*)&output.payload, output.payloadLen);

      // And put us into TX mode, which transmits starting at the TX base addr up to PACKET_LEN many bytes
      LED_on();
      radio.SetTx(0xffffff);
      HAL_Delay(radio.GetTimeOnAir() / 1000);
      LED_off();
    } else {
      // lol wut even 
      assert(0);
    }

    HAL_Delay(1000);
  }
}
