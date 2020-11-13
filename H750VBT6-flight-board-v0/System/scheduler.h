#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "data.h"
#include "state.h"

#include "ms5607.h"
#include "LSM9DS1.h"
#include "H3LIS331DL.h"

#include "i2c.h"
#include "spi.h"
#include "gpio.h"

#include <cstdint>

#define SCHEDULER_1HZ_RATE		(1000)
#define SCHEDULER_10HZ_RATE		(100)
#define SCHEDULER_20HZ_RATE		(50)
#define SCHEDULER_25HZ_RATE		(40)
#define SCHEDULER_100HZ_RATE	(10)

class Scheduler{

    public:

        Data data;

        Scheduler(void);

        void run(void);

};

#endif
