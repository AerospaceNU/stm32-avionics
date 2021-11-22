#include "stdint.h"
#include <stdbool.h>

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
#define IMU_WHO_AM_I 0x0 // We expect
#define ICM20948_WHOAMI_ID 0xEA
#define PWR_MGMT_1 0x06
#define PWR_MGMT_2 0x07;
// This is the start of RawData_t
#define ACCEL_XOUT_H 0x2D