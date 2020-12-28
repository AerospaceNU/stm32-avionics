#include "scheduler.h"
#include "data_log.h"

extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi6;

extern LSM9DS1Ctrl_t lsm9ds1_1;
extern LSM9DS1Ctrl_t lsm9ds1_2;

extern MS5607Ctrl_t ms5607_2;
extern MS5607Ctrl_t ms5607_1;

extern H3LIS331DLCtrl_t h3lis_1;

extern ServoCtrl_t servo1;
extern ServoCtrl_t servo2;
extern ServoCtrl_t servo3;
extern ServoCtrl_t servo4;

extern BuzzerCtrl_t buzzer;

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

    /* data log initialization
     *
     * Parameters passed in are for S25FLx flash
     */
    data_log_init(&hspi1, GPIOC, GPIO_PIN_5);
    // data_log_init(&hspi3, GPIOA, GPIO_PIN_15);

    /* Servo initialization
     *
     * TODO: minPulseMS and maxPulseMS will need a method of tuning based on servo (currently 0.75-2.25)
     * TODO: Initial angles need to be determined
     */
    servoInit(&servo1, &htim8, TIM_CHANNEL_1, 20, 0.75, 2.25, -90, 90, 0);
    servoInit(&servo2, &htim8, TIM_CHANNEL_2, 20, 0.75, 2.25, -90, 90, 0);
    servoInit(&servo3, &htim8, TIM_CHANNEL_3, 20, 0.75, 2.25, -90, 90, 0);
    servoInit(&servo4, &htim8, TIM_CHANNEL_4, 20, 0.75, 2.25, -90, 90, 0);

    /*
     * Buzzer initialization
     * htim8 -> Channel 1
     */
    buzzerInit(&buzzer, &htim1, TIM_CHANNEL_1, 500);

    /* setup for scheduler */
    State state1 = State(&(this->data));
    State state2 = State(&(this->data));

    State states[] = {state1, state2};

    uint32_t lastTime = HAL_GetTick();

    volatile uint32_t t = 0;
    volatile uint32_t t_last = 0;
    volatile uint32_t dt = 0;
    uint8_t i = 0;

    while(1)
    {
        /* rate limiting code for 1 Hz */
        while((HAL_GetTick() - lastTime) < SCHEDULER_20HZ_RATE);
        lastTime = HAL_GetTick();

        /* scheduler main loop */
        state = states[state].run();
        
        i++;

        if(i == 100){
        	i = 0;
        	t = HAL_GetTick();
        	dt = t - t_last;
        	t_last = HAL_GetTick();
        }

    }

}

void Scheduler::terminal()
{

	uint32_t lastTime = 0;

	while(1)
	{
		/* rate limiting code for 1 Hz */
		while((HAL_GetTick() - lastTime) < SCHEDULER_1HZ_RATE);
		lastTime = HAL_GetTick();

		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);

	}
}
