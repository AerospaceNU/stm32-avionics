#include "SX126x.hpp"

SX126x radio;

void entrypoint(void) {

  radio.Init();
  radio.SetStandby(SX126x::STDBY_XOSC);

  // ramp time copied from elvin code
  radio.SetTxParams(22, SX126x::RADIO_RAMP_40_US);
  radio.SetPacketType(SX126x::PACKET_TYPE_GFSK);

  SX126x::ModulationParams_t params;
  params.PacketType = SX126x::PACKET_TYPE_GFSK;
  params.Params.Gfsk.Bandwidth = SX126x::RX_BW_156200;
  params.Params.Gfsk.BitRate = 38400;
  params.Params.Gfsk.Fdev = 20000;
  // todo random copy from elvin
  params.Params.Gfsk.ModulationShaping = SX126x::MOD_SHAPING_G_BT_05;

  radio.SetModulationParams(params);

  const uint32_t FREQ_915 = 915000000;
  radio.SetRfFrequency(FREQ_915);

  RadioPacket_s packet;
  const uint8_t PACKET_LEN = sizeof(packet);  // todo

  SX126x::PacketParams_t pparams;
  pparams.PacketType = SX126x::PACKET_TYPE_GFSK;
  pparams.Params.Gfsk.HeaderType = SX126x::RADIO_PACKET_FIXED_LENGTH;
  pparams.Params.Gfsk.CrcLength = SX126x::RADIO_CRC_OFF;  // TODO
  pparams.Params.Gfsk.PayloadLength = PACKET_LEN;
  pparams.Params.Gfsk.PreambleMinDetect =
      SX126x::RADIO_PREAMBLE_DETECTOR_08_BITS;  // TODO idk
  pparams.Params.Gfsk.PreambleLength = 3;
  pparams.Params.Gfsk.SyncWordLength = 4;
  radio.SetPacketParams(pparams);

  // Copied from default cc1200 settings
  uint8_t sync[8] = {0x93, 0x0b, 0x51, 0xde};
  radio.SetSyncWord(sync);

  LED_off();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1) {
    // Fill the TX buffer, starting at 0, to contain a packet
    // explicitly set the FIFO back to the start (page 48 of SX1261/2 datasheet)
    radio.SetBufferBaseAddresses(0, 0);

    memcpy(packet.callsign, "KM6GNL\0\0", 8);
    packet.packetType = TELEMETRY_ID_STRING;
    snprintf((char*)packet.payload.cliString.string,
             sizeof(packet.payload.cliString.string), "Hello at time %lu!\n",
             HAL_GetTick());

    radio.WriteBuffer(0, (uint8_t*)&packet, sizeof(packet));
    // And put us into TX mode
    radio.SetTx(radio.GetTimeOnAir());
  }
}

