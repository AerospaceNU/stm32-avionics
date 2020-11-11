#include "state.h"

extern LSM9DS1Ctrl_t lsm9ds1_1;
extern LSM9DS1Ctrl_t lsm9ds1_2;

extern MS5607Ctrl_t ms5607_1;
extern MS5607Ctrl_t ms5607_2;

extern H3LIS331DLCtrl_t h3lis_1;

State::State(Data *data)
{
    this->data = data;
}

/**
 * run function for each state
 * calls the general functions then state-specific functions
 */
State::state_t State::run(void)
{

    State::state_t state;

    /** general functions that run in every state */
    this->general();

    /** call state specific functions */
    state = this->state_specific();

    return state;

}

/** 
 *  generic implementation of state specific functions
 *  each implementation of this class should implement this fuction
 *  any class that does not override this function will reset the state-machine
 */
State::state_t State::state_specific(void)
{
    return 0;
}

/** 
 * general functions that run in every state
 */
void State::general(void)
{

    /* read data from sensors */
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
	MS5607_get_data(&ms5607_1);
	LSM9DS1_get_data(&lsm9ds1_1);
	H3LIS331DL_get_data(&h3lis_1);

	MS5607_get_data(&ms5607_2);
	LSM9DS1_get_data(&lsm9ds1_2);

	data_log_write(&lsm9ds1_1, &lsm9ds1_2, &h3lis_1, &ms5607_1, &ms5607_2);

	//debugprintf("Atmospheric Pressure: %f Pa\t Temperature %f C\r\n", ms5607_1.altData.baro, ms5607_1.altData.temp);

}
