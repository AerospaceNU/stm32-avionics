#include "state.h"
/*
extern BNO055Ctrl_t bno_1;
extern BNO055Ctrl_t bno_2;
extern MS5607Ctrl_t ms5607_1;
extern MS5607Ctrl_t ms5607_2;
*/
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
	debugprintf("Toggle GPIO\r\n");

}
