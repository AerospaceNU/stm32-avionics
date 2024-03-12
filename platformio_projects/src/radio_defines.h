const double FREQ_STEP = 0.95367431640625;
const int XTAL_FREQ = 32000000;
const double FREQ_ERR = 0.47683715820312;

#include <stdint.h>

typedef enum {
  MOD_SHAPING_OFF = 0x00,
  MOD_SHAPING_G_BT_03 = 0x08,
  MOD_SHAPING_G_BT_05 = 0x09,
  MOD_SHAPING_G_BT_07 = 0x0A,
  MOD_SHAPING_G_BT_1 = 0x0B,
} RadioModShapings_t;

typedef enum {
  RX_BW_4800 = 0x1F,
  RX_BW_5800 = 0x17,
  RX_BW_7300 = 0x0F,
  RX_BW_9700 = 0x1E,
  RX_BW_11700 = 0x16,
  RX_BW_14600 = 0x0E,
  RX_BW_19500 = 0x1D,
  RX_BW_23400 = 0x15,
  RX_BW_29300 = 0x0D,
  RX_BW_39000 = 0x1C,
  RX_BW_46900 = 0x14,
  RX_BW_58600 = 0x0C,
  RX_BW_78200 = 0x1B,
  RX_BW_93800 = 0x13,
  RX_BW_117300 = 0x0B,
  RX_BW_156200 = 0x1A,
  RX_BW_187200 = 0x12,
  RX_BW_234300 = 0x0A,
  RX_BW_312000 = 0x19,
  RX_BW_373600 = 0x11,
  RX_BW_467000 = 0x09,
} RadioRxBandwidth_t;

typedef struct {
  uint32_t BitRate;
  uint32_t Fdev;
  RadioModShapings_t ModulationShaping;
  uint8_t Bandwidth;
} ModulationParams_t;

typedef enum RadioCommands_e {
  RADIO_GET_STATUS = 0xC0,
  RADIO_WRITE_REGISTER = 0x0D,
  RADIO_READ_REGISTER = 0x1D,
  RADIO_WRITE_BUFFER = 0x0E,
  RADIO_READ_BUFFER = 0x1E,
  RADIO_SET_SLEEP = 0x84,
  RADIO_SET_STANDBY = 0x80,
  RADIO_SET_FS = 0xC1,
  RADIO_SET_TX = 0x83,
  RADIO_SET_RX = 0x82,
  RADIO_SET_RXDUTYCYCLE = 0x94,
  RADIO_SET_CAD = 0xC5,
  RADIO_SET_TXCONTINUOUSWAVE = 0xD1,
  RADIO_SET_TXCONTINUOUSPREAMBLE = 0xD2,
  RADIO_SET_PACKETTYPE = 0x8A,
  RADIO_GET_PACKETTYPE = 0x11,
  RADIO_SET_RFFREQUENCY = 0x86,
  RADIO_SET_TXPARAMS = 0x8E,
  RADIO_SET_PACONFIG = 0x95,
  RADIO_SET_CADPARAMS = 0x88,
  RADIO_SET_BUFFERBASEADDRESS = 0x8F,
  RADIO_SET_MODULATIONPARAMS = 0x8B,
  RADIO_SET_PACKETPARAMS = 0x8C,
  RADIO_GET_RXBUFFERSTATUS = 0x13,
  RADIO_GET_PACKETSTATUS = 0x14,
  RADIO_GET_RSSIINST = 0x15,
  RADIO_GET_STATS = 0x10,
  RADIO_RESET_STATS = 0x00,
  RADIO_CFG_DIOIRQ = 0x08,
  RADIO_GET_IRQSTATUS = 0x12,
  RADIO_CLR_IRQSTATUS = 0x02,
  RADIO_CALIBRATE = 0x89,
  RADIO_CALIBRATEIMAGE = 0x98,
  RADIO_SET_REGULATORMODE = 0x96,
  RADIO_GET_ERROR = 0x17,
  RADIO_SET_TCXOMODE = 0x97,
  RADIO_SET_TXFALLBACKMODE = 0x93,
  RADIO_SET_RFSWITCHMODE = 0x9D,
  RADIO_SET_STOPRXTIMERONPREAMBLE = 0x9F,
  RADIO_SET_LORASYMBTIMEOUT = 0xA0,
} RadioCommands_t;

typedef enum {
  RADIO_PREAMBLE_DETECTOR_OFF = 0x00,      //!< Preamble detection length off
  RADIO_PREAMBLE_DETECTOR_08_BITS = 0x04,  //!< Preamble detection length 8 bits
  RADIO_PREAMBLE_DETECTOR_16_BITS = 0x05,  //!< Preamble detection length 16 bits
  RADIO_PREAMBLE_DETECTOR_24_BITS = 0x06,  //!< Preamble detection length 24 bits
  RADIO_PREAMBLE_DETECTOR_32_BITS = 0x07,  //!< Preamble detection length 32 bit
} RadioPreambleDetection_t;

/*!
 * \brief Represents the possible combinations of SyncWord correlators activated
 */
typedef enum {
  RADIO_ADDRESSCOMP_FILT_OFF = 0x00,  //!< No correlator turned on, i.e. do not search for SyncWord
  RADIO_ADDRESSCOMP_FILT_NODE = 0x01,
  RADIO_ADDRESSCOMP_FILT_NODE_BROAD = 0x02,
} RadioAddressComp_t;

/*!
 *  \brief Radio packet length mode
 */
typedef enum {
  RADIO_PACKET_FIXED_LENGTH = 0x00,     //!< The packet is known on both sides, no header included in the packet
  RADIO_PACKET_VARIABLE_LENGTH = 0x01,  //!< The packet is on variable size, header included
} RadioPacketLengthModes_t;

/*!
 * \brief Represents the CRC length
 */
typedef enum {
  RADIO_CRC_OFF = 0x01,  //!< No CRC in use
  RADIO_CRC_1_BYTES = 0x00,
  RADIO_CRC_2_BYTES = 0x02,
  RADIO_CRC_1_BYTES_INV = 0x04,
  RADIO_CRC_2_BYTES_INV = 0x06,
  RADIO_CRC_2_BYTES_IBM = 0xF1,
  RADIO_CRC_2_BYTES_CCIT = 0xF2,
} RadioCrcTypes_t;

/*!
 * \brief Radio whitening mode activated or deactivated
 */
typedef enum {
  RADIO_DC_FREE_OFF = 0x00,
  RADIO_DC_FREEWHITENING = 0x01,
} RadioDcFree_t;

typedef struct {
  uint16_t PreambleLength;                     //!< The preamble Tx length for GFSK packet type in bit
  RadioPreambleDetection_t PreambleMinDetect;  //!< The preamble Rx length minimal for GFSK packet type
  uint8_t SyncWordLength;                      //!< The synchronization word length for GFSK packet type
  RadioAddressComp_t AddrComp;                 //!< Activated SyncWord correlators
  RadioPacketLengthModes_t HeaderType;         //!< If the header is explicit, it will be transmitted in the GFSK packet. If the header is implicit, it will not be transmitted
  uint8_t PayloadLength;                       //!< Size of the payload in the GFSK packet
  RadioCrcTypes_t CrcLength;                   //!< Size of the CRC block in the GFSK packet
  RadioDcFree_t DcFree;
} PacketParams_t;
