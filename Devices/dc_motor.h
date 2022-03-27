#ifndef DEVICES_DC_MOTOR_H_
#define DEVICES_DC_MOTOR_H_

#include "board_config.h"
#include "tim.h"

/*
 This is how one runs a motor
 pwm_control_t pwm_test;
 pwm_test.timer = &htim3;
 pwm_test.channel = TIM_CHANNEL_3;
 DC_motor_init(&pwm_test);
 DC_motor_run(&pwm_test, 25);

 */

#ifdef __cplusplus
extern "C" {
#endif

/* definition of PWM struct */
/* user needs to config timer */
typedef struct pwm_struct {
  TIM_HandleTypeDef *timer;
  uint32_t channel;
} pwm_control_t;

/* function declarations */

/* creates a new DC motor that can be started and stopped */
void DC_motor_init(pwm_control_t *pwm_Handle);

/* start a specified DC motor with a given speed percentage */
void DC_motor_run(pwm_control_t *pwmObj, uint32_t speed_percent);

/* stops a specified DC motor */
void DC_motor_stop(pwm_control_t *pwmObj);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_DC_MOTOR_H_
