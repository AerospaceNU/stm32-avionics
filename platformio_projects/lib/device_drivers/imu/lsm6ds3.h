//
// Created by nathan on 9/29/23.
//

#ifndef STM32_AVIONICS_LSM6DS3_H
#define STM32_AVIONICS_LSM6DS3_H

#include "imu.h"

class Lsm6ds3 : public Imu {
public:
    Lsm6ds3();

    void init();

    void tick() override;

};


#endif //STM32_AVIONICS_LSM6DS3_H
