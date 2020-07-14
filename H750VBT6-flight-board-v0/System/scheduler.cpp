#include "scheduler.h"

extern SPI_HandleTypeDef hspi1;

extern LSM9DS1Ctrl_t lsm9ds1_1;
extern LSM9DS1Ctrl_t lsm9ds1_2;

extern MS5607Ctrl_t ms5607_2;
extern MS5607Ctrl_t ms5607_1;

extern H3LIS331DLCtrl_t h3lis_1;

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
    /* LSM9DS1 1
     * SPI 3
     * Mag CS      -> PD0
     * Acc/Gyro CS -> PD1
     */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);
    lsm9ds1_1.ag.LSM9DS1SPI.hspi = &hspi3;
    lsm9ds1_1.ag.LSM9DS1SPI.port = GPIOD;
    lsm9ds1_1.ag.LSM9DS1SPI.pin = GPIO_PIN_1;
    lsm9ds1_1.m.LSM9DS1SPI.hspi = &hspi3;
    lsm9ds1_1.m.LSM9DS1SPI.port = GPIOD;
    lsm9ds1_1.m.LSM9DS1SPI.pin = GPIO_PIN_0;
    LSM9DS1_init(&lsm9ds1_1);

    /* LSM9DS1 1
	 * SPI 6
	 * Mag CS      -> PE0
	 * Acc/Gyro CS -> PE1
	 */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);
	lsm9ds1_2.ag.LSM9DS1SPI.hspi = &hspi6;
	lsm9ds1_2.ag.LSM9DS1SPI.port = GPIOE;
	lsm9ds1_2.ag.LSM9DS1SPI.pin = GPIO_PIN_1;
	lsm9ds1_2.m.LSM9DS1SPI.hspi = &hspi6;
	lsm9ds1_2.m.LSM9DS1SPI.port = GPIOE;
	lsm9ds1_2.m.LSM9DS1SPI.pin = GPIO_PIN_0;
	LSM9DS1_init(&lsm9ds1_2);

    /* ms5607 barometer */
    /* ms5607 1
     * SPI3
     * CS -> PA15
     */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    ms5607_1.spiconfig.hspi = hspi3;
    ms5607_1.spiconfig.port = GPIOA;
    ms5607_1.spiconfig.pin = GPIO_PIN_15;
    MS5607_init(&ms5607_1);

    /*
     * ms5607 2
     * SPI6
     * CS -> PE2
     */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
    ms5607_2.spiconfig.hspi = hspi6;
    ms5607_2.spiconfig.port = GPIOE;
    ms5607_2.spiconfig.pin = GPIO_PIN_2;
    MS5607_init(&ms5607_2);

    /* H3LIS331DL High G Accelerometer */
    /* these configs are not correct for the final
     * this will be on SPI 3
     * High G CS -> PD2
     */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    h3lis_1.H3LIS331DLSPI.hspi = &hspi3;
    h3lis_1.H3LIS331DLSPI.port = GPIOD;
    h3lis_1.H3LIS331DLSPI.pin = GPIO_PIN_2;
    H3LIS331DL_init(&h3lis_1);

    /* setup for scheduler */
    State state1 = State(&(this->data));
    State state2 = State(&(this->data));

    State states[] = {state1, state2};

    uint32_t lastTime = HAL_GetTick();

    while(1)
    {
        /* rate limiting code for 1 Hz */
        while((HAL_GetTick() - lastTime) < SCHEDULER_1HZ_RATE);
        lastTime = HAL_GetTick();

        /* scheduler main loop */
        state = states[state].run();
        
    }

}
