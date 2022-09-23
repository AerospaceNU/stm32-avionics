/*
 * adc_mcp3564.c
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#include "adc_mcp3564.h"
#include "errno.h"

#define SPI_TIMEOUT_MS 50

// ======== Chip defines ========

// Command types
#define MCP356X_CMD_TYPE_FAST_CMD 0x0
#define MCP356X_CMD_TYPE_STATIC_READ 0x1
#define MCP356X_CMD_TYPE_INCR_WRITE 0x2
#define MCP356X_CMD_TYPE_INCR_READ 0x3

// Fast commands
#define MCP356X_FAST_CMD_ADC_CONV_START 0b1010
#define MCP356X_FAST_CMD_ADC_STANDBY 0b1011
#define MCP356X_FAST_CMD_ADC_SHUTDOWN 0b1100
#define MCP356X_FAST_CMD_FULL_SHUTDOWN 0b1101
#define MCP356X_FAST_CMD_FULL_RESET 0b1110

// Register addresses

// 0x0 = 4/24/32b wide
// 0x1 to 0x6 = 8b wide
// 0x7 to 0xB = 24b wide
// 0xC to 0xD = 8b wide
// 0xE to 0xF = 16b wide

#define MCP356X_REG_ADCDATA 0x0
#define MCP356X_REG_CONFIG0 0x1
#define MCP356X_REG_CONFIG1 0x2
#define MCP356X_REG_CONFIG2 0x3
#define MCP356X_REG_CONFIG3 0x4
#define MCP356X_REG_IRQ 0x5
#define MCP356X_REG_MUX 0x6
#define MCP356X_REG_SCAN 0x7
#define MCP356X_REG_TIMER 0x8
#define MCP356X_REG_OFFSETCAL 0x9
#define MCP356X_REG_GAINCAL 0xA
#define MCP356X_REG_LOCK 0xD
#define MCP356X_REG_CRFCFG 0xF

// Number of available channels on the MUX register
// Technically there are 8 channels but I'd like to permit
// the other 8 channels to be read for debugging purposes
#define MCP356X_MUX_N_CHAN 16

// TODO wtf does this do?
static const uint8_t DEV_ADDR = 0x01;

//! Converts a 24-bit reading from the ADC to a int32
static int32_t adc_24bit_reading_to_int32(uint32_t reg) {
        // The register we get has the top 8 bits blank,
        // and the bottom 24 bits are the actual adc reading
        // as a signed, 24-bit integer
        // When we assign f.v, the compiler will deal with converting
        // to an actual int32 by "extending" the sign bits (see: 2's compliment)
        // Note that structs are MSB to LSB, read left to right
        // (or top to bottom)
        struct { int unused:8; int v:24; } f;
        f.v = reg;
        return f.v;
}

// ======== End chip defines ========

/**
 * Modify an 8-bit register on the ADC.
 *
 * @param dev the target ADC device
 * @param reg_addr the register address
 * @param mask the bitmask, 1 at every bit to be modified
 * @param val the value to set into the register
 * @return an error code, if any
 */
static int mcp356x_modify_reg8(AdcMcp3564Ctrl_s *dev,
                               int reg_addr,
                               int mask,
                               int val) {

    // 1: Read data from register
    uint8_t tx_read[2];

    // Static read (Table 6-2)
    tx_read[0] = DEV_ADDR << 6
                 | reg_addr << 2
                 | (MCP356X_CMD_TYPE_STATIC_READ & 0x3);

//    struct spi_buf tx_read_spi_buf = {.buf = tx_read, .len = 2};
//    struct spi_buf_set tx_read_bufset = {.buffers = &tx_read_spi_buf, .count = 1};

    uint8_t rx_read[2] = {0};
//    struct spi_buf rx_read_spi_buf = {.buf = rx_read, .len = 2};
//    struct spi_buf_set rx_read_bufset = {.buffers = &rx_read_spi_buf, .count = 1};

    int ec_read_reg = HAL_SPI_TransmitReceive(dev->hspi, tx_read, rx_read, 2, SPI_TIMEOUT_MS);
//    int ec_read_reg = spi_transceive(spi_bus, spi_cfg, &tx_read_bufset, &rx_read_bufset);
    if (ec_read_reg) {
        return ec_read_reg;
    }

    rx_read[1] &= ~mask;

    // 2: Configure the register
    // Use incremental write command (Table 6-2)
    // idx 0 = command byte (Table 6-1)
    // idx 1 = data
    uint8_t tx_write[2] = {
            DEV_ADDR << 6 | reg_addr << 2 | (MCP356X_CMD_TYPE_INCR_WRITE & 0x3),
            rx_read[1] | val
    };
//    struct spi_buf tx_write_spi_buf = {.buf = tx_write, .len = 2};
//    struct spi_buf_set tx_write_bufset = {.buffers = &tx_write_spi_buf, .count = 1};

    int ec_write_reg = HAL_SPI_Transmit(dev->hspi, tx_write, 2, SPI_TIMEOUT_MS);
//    int ec_write_reg = spi_write(spi_bus, spi_cfg, &tx_write_bufset);
    if (ec_write_reg) {
        return ec_write_reg;
    }

    return 0;
}

/**
 * Modify a 24-bit register on the ADC.
 *
 * @param dev the target ADC device
 * @param reg_addr the register address
 * @param mask the bitmask, 1 at every bit to be modified
 * @param val the value to set into the register
 * @return an error code, if any
 */
static int mcp356x_modify_reg24(AdcMcp3564Ctrl_s *dev,
                                int reg_addr,
                                int mask,
                                int val) {

    // 1: Read data from register
    uint8_t tx_read[4];

    // Static read (Table 6-2)
    tx_read[0] = DEV_ADDR << 6
                 | reg_addr << 2
                 | (MCP356X_CMD_TYPE_STATIC_READ & 0x3);

//    struct spi_buf tx_read_spi_buf = {.buf = tx_read, .len = 4};
//    struct spi_buf_set tx_read_bufset = {.buffers = &tx_read_spi_buf, .count = 1};

    uint8_t rx_read[4] = {0};
//    struct spi_buf rx_read_spi_buf = {.buf = rx_read, .len = 4};
//    struct spi_buf_set rx_read_bufset = {.buffers = &rx_read_spi_buf, .count = 1};

//    int ec_read_reg = spi_transceive(spi_bus, spi_cfg, &tx_read_bufset, &rx_read_bufset);
    int ec_read_reg = HAL_SPI_TransmitReceive(dev->hspi, tx_read, rx_read, 4, SPI_TIMEOUT_MS);
    if (ec_read_reg) {
        return ec_read_reg;
    }

    int32_t reg = rx_read[1] << 16 | rx_read[2] << 8 | rx_read[3];
    reg &= ~mask;
    reg |= val;

    // 2: Configure the register
    // Use incremental write command (Table 6-2)
    // idx 0 = command byte (Table 6-1)
    // idx 1 = data
    uint8_t tx_write[4] = {
            DEV_ADDR << 6 | reg_addr << 2 | (MCP356X_CMD_TYPE_INCR_WRITE & 0x3),
            reg >> 16, reg >> 8, reg & 0xFF
    };
//    struct spi_buf tx_write_spi_buf = {.buf = tx_write, .len = 4};
//    struct spi_buf_set tx_write_bufset = {.buffers = &tx_write_spi_buf, .count = 1};

//    int ec_write_reg = spi_write(spi_bus, spi_cfg, &tx_write_bufset);
    int ec_write_reg = HAL_SPI_Transmit(dev->hspi, tx_write, 4, SPI_TIMEOUT_MS);
    if (ec_write_reg) {
        return ec_write_reg;
    }

    return 0;
}

int mcp3564_init(AdcMcp3564Ctrl_s *dev, SPI_HandleTypeDef *hspi,
            GPIO_TypeDef *csPort, uint16_t csPin,
            GPIO_TypeDef *intPort, uint16_t intPin) {
	  dev->hspi = hspi;
	  dev->csPort = csPort;
	  dev->csPin = csPin;
	  dev->intPort = intPort;
	  dev->intPin = intPin;

	int ec;

    // CONFIG0: Configure:
    //  - VREF_SEL -> no buffer
    //  - CLK_SEL -> internal
    //  - ADC_MODE -> standby
    ec = mcp356x_modify_reg8(dev, MCP356X_REG_CONFIG0,
                             0b1 << 6 | 0b11 << 4 | 0b11,
                             0b0 << 6 | 0b10 << 4 | 0b10);
    if (ec) {
        return ec;
    }

    // CONFIG1: Force defaults
    ec = mcp356x_modify_reg8(dev, MCP356X_REG_CONFIG1,
                             (1 << 8) - 1,
                             0b00001100);
    if (ec) {
        return ec;
    }

    // CONFIG2: Force defaults
    ec = mcp356x_modify_reg8(dev, MCP356X_REG_CONFIG2,
                             (1 << 8) - 1,
                             0b10001011);
    if (ec) {
        return ec;
    }

    // CONFIG3: Configure:
    //   - CONV_MODE -> one shot to standby
    //   - DATA_FORMAT -> force 24 bit
    //   - OFFCAL -> force to off
    //   - GAINCAL -> force to off
    ec = mcp356x_modify_reg8(dev, MCP356X_REG_CONFIG3,
                             0b11 << 6 | 0b11 << 4 | 0b11,
                             0b10 << 6 | 0b00 << 4 | 0b11);
    if (ec) {
        return ec;
    }

    // IRQ
    //  - IRQ_MODE -> use internal pullup
    ec = mcp356x_modify_reg8(dev, MCP356X_REG_IRQ,
                             0b1 << 2,
                             0b1 << 2);
    if (ec) {
        return ec;
    }

    // SCAN: Disable SCAN
    ec = mcp356x_modify_reg24(dev, MCP356X_REG_SCAN,
                              (1 << 24) - 1, 0);
    if (ec) {
        return ec;
    }

    // OFFSETCAL: Force to 0
    ec = mcp356x_modify_reg24(dev, MCP356X_REG_OFFSETCAL,
                              (1 << 24) - 1, 0);
    if (ec) {
        return ec;
    }

    // GAINCAL: Force to unity
    ec = mcp356x_modify_reg24(dev, MCP356X_REG_GAINCAL,
                              (1 << 24) - 1, 8388608);
    if (ec) {
        return ec;
    }

    return 0;
}

int mcp356x_channel_setup(AdcMcp3564Ctrl_s *dev,
                          const int channel_id) {

    // 1: Configure the MUX register to select the correct channel
    // Select MUX_VIN+ as the specified channel and MUX_VOUT- as A_GND
    int ec_write_mux = mcp356x_modify_reg8(dev, MCP356X_REG_MUX,
                                           (1 << 8) - 1,
                                           channel_id << 4 | 0b1100);
    if (ec_write_mux) {
        return ec_write_mux;
    }

    // 2: Request a new conversion on the ADC
    // ADC conversion fast command (Table 6-2)
    uint8_t tx_conv_buf[1] = {
            DEV_ADDR << 6 | MCP356X_FAST_CMD_ADC_CONV_START << 2 | (MCP356X_CMD_TYPE_FAST_CMD & 0x3),
    };
//    struct spi_buf tx_conv_spi_buf = {.buf = tx_conv_buf, .len = 1};
//    struct spi_buf_set tx_conv_bufset = {.buffers = &tx_conv_spi_buf, .count = 1};

    int ec_conv = HAL_SPI_Transmit(dev->hspi, tx_conv_buf, 1, SPI_TIMEOUT_MS);
    if (ec_conv) {
        return ec_conv;
    }

    return 0;
}

int mcp356x_read(AdcMcp3564Ctrl_s *dev) {

    // 3: Read data from ADCDATA register
    uint8_t tx_read_buf[4] = {0};

    // Static read (Table 6-2)
    tx_read_buf[0] = DEV_ADDR << 6
                     | MCP356X_REG_ADCDATA << 2
                     | (MCP356X_CMD_TYPE_STATIC_READ & 0x3);

//    struct spi_buf tx_read_spi_buf = {.buf = tx_read_buf, .len = read_n_bytes};
//    struct spi_buf_set tx_read_bufset = {.buffers = &tx_read_spi_buf, .count = 1};

    	uint8_t rx_read_buf[4] = {0};
//    uint8_t *rx_read_buf = sequence->buffer;
//    struct spi_buf rx_read_spi_buf = {.buf = rx_read_buf, .len = sequence->buffer_size};
//    struct spi_buf_set rx_read_bufset = {.buffers = &rx_read_spi_buf, .count = 1};

    int ec_read = HAL_SPI_TransmitReceive(dev->hspi, tx_read_buf, rx_read_buf, 4, SPI_TIMEOUT_MS);
//    int ec_read = spi_transceive(spi_bus, spi_cfg, &tx_read_bufset, &rx_read_bufset);
//    k_free(tx_read_buf);

    if (ec_read) {
        return ec_read;
    }

    // third byte from bottom, DR_STATUS
    // Active low, cleared when ready (figure 6-1)
    if (rx_read_buf[0] & 0x4) {
        return -EBUSY;
    }

    // First byte of rx_read_buf should be the status byte
    // Next 3 are big (?) endian data, per figure 5-8
	uint32_t raw = rx_read_buf[1] << 16 | rx_read_buf[2] << 8 | rx_read_buf[3];
	// TODO make this respect different DATA_FORMAT modes
	dev->result = adc_24bit_reading_to_int32(raw);

    return 0;
}

