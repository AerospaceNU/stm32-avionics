#include "cc1200_cpp.h"

//
// Test suite for the CC1200 radio driver.
// Provides a menu to configure radio options, and
// attempts to send and receive a signal using two radios connected
// to the processor.
//

#include "CC1200.h"

#include "board_config.h"
#include "board_config_common.h"
#include <inttypes.h>

CC1200 txRadio(
		radioTi433Hspi[0],
		radioTi433RstGpioPort[0],
		radioTi433RstPin[0],
		radioTi433CsGpioPort[0],
		radioTi433CsPin[0],
		radioTi433MisoGpioPort[0],
		radioTi433MisoPin[0]
		);

void checkExistance()
{
	printf("Checking TX radio.....\n");
	bool txSuccess = txRadio.begin();
	printf("TX radio initialized: %s\n", txSuccess ? "true" : "false");

//	printf("Checking RX radio.....\n");
//	bool rxSuccess = rxRadio.begin();
//	printf("RX radio initialized: %s\n", rxSuccess ? "true" : "false");
}

void checkSignalTransmit()
{
	printf("Initializing CC1200s.....\n");
	txRadio.begin();
//	rxRadio.begin();

	printf("Configuring RF settings.....\n");

	int config=-1;
	//MENU. ADD AN OPTION FOR EACH TEST.
	printf("Select a config: \n");
	// This is similar to the SmartRF "100ksps ARIB Standard" configuration, except it doesn't use zero-if
	printf("1.  915MHz 100ksps 2-GFSK DEV=50kHz CHF=208kHz\n");
	// This should be identical to the SmartRF "500ksps 4-GFSK Max Throughput ARIB Standard" configuration
	printf("2.  915MHz 500ksps 4-GFSK DEV=400kHz CHF=1666kHz Zero-IF\n");
	printf("3.  915MHz 38.4ksps 2-GFSK DEV=20kHz CHF=833kHz\n");
	printf("4.  915MHz 100ksps 2-GFSK DEV=50kHz CHF=208kHz Fixed Length\n");

	// TODO: zero shot that scanf actually works currently lol
	scanf("%d", &config);
	printf("Running test with config %d:\n\n", config);

	CC1200::PacketMode mode = CC1200::PacketMode::VARIABLE_LENGTH;
	CC1200::Band band = CC1200::Band::BAND_410_480MHz;
	float frequency = 433e6;
	const float txPower = 0;
	float fskDeviation;
	float symbolRate;
	float rxFilterBW;
	CC1200::ModFormat modFormat;
	CC1200::IFCfg ifCfg = CC1200::IFCfg::POSITIVE_DIV_8;
	bool imageCompEnabled = true;
	bool dcOffsetCorrEnabled = true;
	uint8_t agcRefLevel;

	uint8_t dcFiltSettingCfg = 1; // default chip setting
	uint8_t dcFiltCutoffCfg = 4; // default chip setting
    uint8_t agcSettleWaitCfg = 2;

	CC1200::SyncMode syncMode = CC1200::SyncMode::SYNC_32_BITS;
	uint32_t syncWord = 0x930B51DE; // default sync word
	uint8_t syncThreshold = 8; // TI seems to recommend this threshold for most configs above 100ksps

	uint8_t preableLengthCfg = 5; // default chip setting
	uint8_t preambleFormatCfg = 0; // default chip setting

	if(config == 1)
	{
		fskDeviation = 49896;
		symbolRate = 100000;
		rxFilterBW = 208300;
		modFormat = CC1200::ModFormat::GFSK_2;
		agcRefLevel = 0x2A;
	}
	else if(config == 2)
	{
		// SmartRF "500ksps 4-GFSK ARIB standard" config
		fskDeviation = 399169;
		symbolRate = 500000;
		rxFilterBW = 1666700;
		ifCfg = CC1200::IFCfg::ZERO;
		imageCompEnabled = false;
		dcOffsetCorrEnabled = true;
		modFormat = CC1200::ModFormat::GFSK_4;
		agcRefLevel = 0x2F;
	}
	else if(config == 3)
	{
		// This is basically our (AeroNU's) present config
		fskDeviation = 19989;
		symbolRate = 38400;
		rxFilterBW = 104200;
		modFormat = CC1200::ModFormat::GFSK_2;
		agcRefLevel = 0x27;
        agcSettleWaitCfg = 1;
	}
	else if(config == 4)
	{
		fskDeviation = 49896;
		symbolRate = 100000;
		rxFilterBW = 208300;
		modFormat = CC1200::ModFormat::GFSK_2;
		agcRefLevel = 0x2A;
		mode = CC1200::PacketMode::FIXED_LENGTH;
	}
	else
	{
		printf("Invalid config number!");
		return;
	}

	CC1200& radio = txRadio;
	{

		radio.configureFIFOMode();
		radio.setPacketMode(mode);
		radio.setModulationFormat(modFormat);
		radio.setFSKDeviation(fskDeviation);
		radio.setSymbolRate(symbolRate);
		radio.setOutputPower(txPower);
		radio.setRadioFrequency(band, frequency);
		radio.setIFCfg(ifCfg, imageCompEnabled);
		radio.configureDCFilter(dcOffsetCorrEnabled, dcFiltSettingCfg, dcFiltCutoffCfg);
		radio.setRXFilterBandwidth(rxFilterBW);
		radio.configureSyncWord(syncWord, syncMode, syncThreshold);
		radio.configurePreamble(preableLengthCfg, preambleFormatCfg);

		// AGC configuration (straight from SmartRF)
		radio.setAGCReferenceLevel(agcRefLevel);
		radio.setAGCSyncBehavior(CC1200::SyncBehavior::FREEZE_NONE);
        radio.setAGCSettleWait(agcSettleWaitCfg);
		if(ifCfg == CC1200::IFCfg::ZERO)
		{
			radio.setAGCGainTable(CC1200::GainTable::ZERO_IF, 11, 0);
		}
		else
		{
			// enable all AGC steps for NORMAL mode
			radio.setAGCGainTable(CC1200::GainTable::NORMAL, 17, 0);
		}
		radio.setAGCHysteresis(0b10);
		radio.setAGCSlewRate(0);
	}

	// configure on-transmit actions
	txRadio.setOnTransmitState(CC1200::State::TX);
//	rxRadio.setOnReceiveState(CC1200::State::RX, CC1200::State::RX);

	printf("Starting transmission.....\n");

	txRadio.startTX();
//	rxRadio.startRX();

	const char message[] = "Hello world!";
//	char receiveBuffer[sizeof(message)];

	if(mode == CC1200::PacketMode::FIXED_LENGTH)
	{
		txRadio.setPacketLength(sizeof(message) - 1);
//		rxRadio.setPacketLength(sizeof(message) - 1);
	}

	while(true)
	{
		HAL_Delay(1000);

		printf("\n---------------------------------\n");

		printf("<<SENDING: %s\n", message);
		txRadio.enqueuePacket(message, sizeof(message) - 1);


		// update TX radio
		printf("TX radio: state = 0x%" PRIx8 ", TX FIFO len = %zu, FS lock = 0x%u\n",
				  static_cast<uint8_t>(txRadio.getState()), txRadio.getTXFIFOLen(), txRadio.readRegister(CC1200::ExtRegister::FSCAL_CTRL) & 1);

		// wait a while for the packet to come in.
//		ThisThread::sleep_for(10ms);
		HAL_Delay(10);

		// wait the time we expect the message to take, with a safety factor of 2
		auto timeoutTimeMicros = (static_cast<int64_t>((1/symbolRate) * 1e-6f * sizeof(message) * 2));
		HAL_Delay(timeoutTimeMicros / 1000);


//		Timer packetReceiveTimeout;
//		packetReceiveTimeout.start();
//		bool hasPacket;
//		do
//		{
//			hasPacket = rxRadio.hasReceivedPacket();
//		}
//		while(packetReceiveTimeout.elapsed_time() < timeoutTime && !hasPacket);
//
//		printf("RX radio: state = 0x%" PRIx8 ", RX FIFO len = %zu\n",
//				  static_cast<uint8_t>(rxRadio.getState()), rxRadio.getRXFIFOLen());
//		if(hasPacket)
//		{
//			rxRadio.receivePacket(receiveBuffer, sizeof(message) - 1);
//			receiveBuffer[sizeof(message)-1] = '\0';
//			printf(">>RECEIVED: %s\n", receiveBuffer);
//		}
//		else
//		{
//			printf(">>No packet received!\n");
//		}
	}
}


int cc1200_main()
{
	printf("\nCC1200 Radio Test Suite:\n");

	while(1){
		int test=-1;
		//MENU. ADD AN OPTION FOR EACH TEST.
		printf("Select a test: \n");
		printf("1.  Exit Test Suite\n");
		printf("2.  Check Existence\n");
		printf("3.  Check Transmitting Signal\n");

		scanf("%d", &test);
		printf("Running test %d:\n\n", test);
		//SWITCH. ADD A CASE FOR EACH TEST.
		switch(test) {
			case 1:         printf("Exiting test suite.\n");    return 0;
			case 2:         checkExistance();              break;
			case 3:         checkSignalTransmit();              break;
			default:        printf("Invalid test number. Please run again.\n"); continue;
		}
		printf("done.\r\n");
	}

	return 0;

}

