#include "ICM20948.h"
#include "ICM20948_defs.h"
#include <stdbool.h> // Not sure this is necessary?

bool set_bank(ICM20948Ctrl_t* sensor, uint8_t bank) {
    SPI_WriteRegister(&sensor->spiconfig, BANK_SELECT, bank);
}

uint8_t spi_read(ICM20948Ctrl_t* sensor, uint8_t bank, uint8_t reg) {
    if(bank != sensor->last_bank) {
        set_bank(sensor, bank);
    }
    return SPI_ReadRegister(&sensor->spiconfig, reg);
}

void spi_write(ICM20948Ctrl_t* sensor, uint8_t bank, uint8_t reg, uint8_t val) {
    if(bank != sensor->last_bank) {
        set_bank(sensor, bank);
    }
    return SPI_WriteRegister(&sensor->spiconfig, reg, val);
}

bool ICM20948_init(ICM20948Ctrl_t* sensor) {
    // Check to make sure we can see the sensor
    if(!ICM20948_is_connected(sensor)) {
        return false;
    }

    // Reset (this should also disable sleep and LP mode)
    uint8_t reg = 0b1000000; // Internal 20MHz, set reset flag
    spi_write(sensor, 0, PWR_MGMT_1, reg);
    // Sparkfun driver sleeps for 50ms here
    HAL_Delay(50);

    // === Start the magnetometer ===

    // disable master passthrough
    

    // We shouldn't need to do any DMP stuff?
}
bool ICM20948_is_connected(ICM20948Ctrl_t* sensor) {
    return spi_read(sensor, 0, IMU_WHO_AM_I) == ICM20948_WHOAMI_ID;
}

bool ICM20948_read(ICM20948Ctrl_t* sensor) {

}
