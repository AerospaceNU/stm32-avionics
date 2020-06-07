#include "scheduler.h"
/*
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c4;
extern SPI_HandleTypeDef hspi1;

extern BNO055Ctrl_t bno_1;
extern BNO055Ctrl_t bno_2;
extern MS5607Ctrl_t ms5607_1;
extern MS5607Ctrl_t ms5607_2;
*/
Scheduler::Scheduler(void)
{

    /* initialize data structure */
    this->data = Data();

}

void Scheduler::run(void)
{

    typedef enum{
        STATE1 = 0,
        STATE2
    }state_e;

    State::state_t state = STATE1;

    /* initialize sensors */
    /* lsm9ds1 IMU */

    /* ms5607 barometer *//*
    ms5607_1.spiconfig.hspi = hspi1;
    ms5607_1.spiconfig.port = GPIOD;
    ms5607_1.spiconfig.pin = GPIO_PIN_14;
    MS5607_init(&ms5607_1);

    ms5607_2.spiconfig.hspi = hspi1;
    ms5607_2.spiconfig.port = GPIOD;
    ms5607_2.spiconfig.pin = GPIO_PIN_15;
    MS5607_init(&ms5607_2);
     */

    /* setup for scheduler */
    State state1 = State(&(this->data));
    State state2 = State(&(this->data));

    State states[] = {state1, state2};

    uint32_t lastTime = HAL_GetTick();

    while(1)
    {
        /* rate limiting code for 10 Hz */
        while((HAL_GetTick() - lastTime) < SCHEDULER_10HZ_RATE);
        lastTime = HAL_GetTick();

        /* scheduler main loop */
        state = states[state].run();
        
    }

}
