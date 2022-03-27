#include "dc_motor.h"

#ifdef HAS_DC_MOTOR

#include <stdio.h>

#include "stdbool.h"
#include "tim.h"

/* inits a DC motor specified by the user */
void DC_motor_init(pwm_control_t *pwm_control) {
  /* check that PWM was started successfully */
  if (HAL_OK != HAL_TIM_PWM_Start(pwm_control->timer, pwm_control->channel)) {
    printf("can't start PWM\r\n");
  }
}

/* STOP a DC motor using user defined object */
void DC_motor_stop(pwm_control_t *pwmObj) {
  if (HAL_OK != HAL_TIM_PWM_Stop(pwmObj->timer, pwmObj->channel)) {
    printf("can't stop DC Motor\r\n");
  }
}

/* STARTS a given user defined DC motor, with a certain speed percentage */
void DC_motor_run(pwm_control_t *pwmObj, uint32_t speed_percent) {  // 0 -> 255?
  /* if the ratio is zero, stop the motor */
  if (speed_percent == 0) {
    HAL_TIM_PWM_Stop(pwmObj->timer, pwmObj->channel);
    return;
  }

  /* define the period */
  int setPeriod = (speed_percent / 100.0) * pwmObj->timer->Init.Period;
  __HAL_TIM_SET_COMPARE(pwmObj->timer, pwmObj->channel, setPeriod);
}

#endif
