#include "stdint.h"
#include <stdbool.h>

// From the sparkfun driver
typedef enum
{
    ICM_20948_Stat_Ok = 0x00, // The only return code that means all is well
    ICM_20948_Stat_Err,       // A general error
    ICM_20948_Stat_NotImpl,   // Returned by virtual functions that are not implemented
    ICM_20948_Stat_ParamErr,
    ICM_20948_Stat_WrongID,
    ICM_20948_Stat_InvalSensor, // Tried to apply a function to a sensor that does not support it (e.g. DLPF to the temperature sensor)
    ICM_20948_Stat_NoData,
    ICM_20948_Stat_SensorNotSupported,
    ICM_20948_Stat_DMPNotSupported,    // DMP not supported (no #define ICM_20948_USE_DMP)
    ICM_20948_Stat_DMPVerifyFail,      // DMP was written but did not verify correctly
    ICM_20948_Stat_FIFONoDataAvail,    // FIFO contains no data
    ICM_20948_Stat_FIFOIncompleteData, // FIFO contained incomplete data
    ICM_20948_Stat_FIFOMoreDataAvail,  // FIFO contains more data
    ICM_20948_Stat_UnrecognisedDMPHeader,
    ICM_20948_Stat_UnrecognisedDMPHeader2,
    ICM_20948_Stat_InvalDMPRegister, // Invalid DMP Register

    ICM_20948_Stat_NUM,
    ICM_20948_Stat_Unknown,
} ICM_20948_Status_e;

typedef enum
{
  AK09916_REG_WIA1 = 0x00,
  AK09916_REG_WIA2,
  AK09916_REG_RSV1,
  AK09916_REG_RSV2, // Reserved register. We start reading here when using the DMP. Secret sauce...
  // discontinuity - containing another nine reserved registers? Secret sauce...
  AK09916_REG_ST1 = 0x10,
  AK09916_REG_HXL,
  AK09916_REG_HXH,
  AK09916_REG_HYL,
  AK09916_REG_HYH,
  AK09916_REG_HZL,
  AK09916_REG_HZH,
  // discontinuity
  AK09916_REG_ST2 = 0x18,
  // discontinuity
  AK09916_REG_CNTL2 = 0x31,
  AK09916_REG_CNTL3,
} AK09916_Reg_Addr_e;

typedef struct
{
    uint8_t DLY : 5;
    uint8_t REG_DIS : 1;
    uint8_t INT_EN : 1;
    uint8_t EN : 1;
} ICM_20948_I2C_PERIPH4_CTRL_t;

typedef struct
{
    uint8_t I2C_PERIPH0_NACK : 1;
    uint8_t I2C_PERIPH1_NACK : 1;
    uint8_t I2C_PERIPH2_NACK : 1;
    uint8_t I2C_PERIPH3_NACK : 1;
    uint8_t I2C_PERIPH4_NACK : 1;
    uint8_t I2C_LOST_ARB : 1;
    uint8_t I2C_PERIPH4_DONE : 1;
    uint8_t PASS_THROUGH : 1;
} ICM_20948_I2C_MST_STATUS_t;

// I2c magnetometer stupidity
#define MAG_AK09916_I2C_ADDR 0x0C
#define MAG_AK09916_WHO_AM_I 0x4809
#define MAG_REG_WHO_AM_I 0x00

// Registers
// This IMU as 4 different banks, so we (may) need
// to switch between them. bits 5-4 are the actual bank
// and the 4 below them do nothing
#define BANK_SELECT 0X7f
#define REG_BANK_0 0u << 4
#define REG_BANK_1 1u << 4
#define REG_BANK_2 2u << 4
#define REG_BANK_3 3u << 4

// Bank 0 registers
#define IMU_WHO_AM_I 0x0 
#define USER_CTRL 0x03
#define PWR_MGMT_1 0x06
#define PWR_MGMT_2 0x07
#define INT_PIN_CFG 0x0F
#define ICM20948_WHOAMI_ID 0xEA
#define AGB0_REG_I2C_MST_STATUS 23

// This is the start of RawData_t
#define ACCEL_XOUT_H 0x2D

// Bank 2
#define AGB2_ACCEL_CONFIG 0x14
#define AGB2_GYRO_CONFIG_1 0x01

// Bank 3
#define AGB3_REG_I2C_MST_CTRL 0x01
#define AGB3_REG_I2C_PERIPH4_ADDR 19
#define AGB3_REG_I2C_PERIPH4_REG 20
#define AGB3_REG_I2C_PERIPH4_CTRL 21
#define AGB3_REG_I2C_PERIPH4_DO 22
#define AGB3_REG_I2C_PERIPH4_DI 23