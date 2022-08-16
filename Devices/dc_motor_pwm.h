#ifndef DEVICES_DC_MOTOR_PWM_H_
#define DEVICES_DC_MOTOR_PWM_H_

#include "board_config_common.h"

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

/* Definition of DC motor control struct */
/* user needs to config timer */
typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
} DcMotorPwmCtrl_t;

/* function declarations */

/* creates a new DC motor that can be started and stopped */
void dcMotorPwmInit(DcMotorPwmCtrl_t *dev, TIM_HandleTypeDef *htim,
                    uint32_t channel);

/* start a specified DC motor with a given speed percentage -100-100 */
void dcMotorPwmSetPercent(DcMotorPwmCtrl_t *dev, double speed_percent);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_DC_MOTOR_PWM_H_
