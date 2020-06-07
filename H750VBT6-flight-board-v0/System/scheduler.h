#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "data.h"
#include "state.h"
#include "ms5607.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

#include <cstdint>

#define SCHEDULER_10HZ_RATE		(100)
#define SCHEDULER_100HZ_RATE	(10)

class Scheduler{

    public:

        Data data;

        Scheduler(void);

        void run(void);

};

#endif
