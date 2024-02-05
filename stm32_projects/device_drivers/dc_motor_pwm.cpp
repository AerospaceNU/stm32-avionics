#include "dc_motor_pwm.h"

#if HAS_DEV(DC_MOTOR_PWM)

#include <stdbool.h>
#include <stdio.h>

/* inits a DC motor specified by the user */
void dcMotorPwm_init(DcMotorPwmCtrl_s* dev, TIM_HandleTypeDef* htim,
                     uint32_t channel) {
  dev->htim = htim;
  dev->channel = channel;

  /* check that PWM was started successfully */
  if (HAL_OK != HAL_TIM_PWM_Start(dev->htim, dev->channel)) {
    printf("can't start PWM\r\n");
  }
}

/* STARTS a given user defined DC motor, with a certain speed percentage */
void dcMotorPwm_setPercent(DcMotorPwmCtrl_s* dev, double speed_percent) {
  /* If percent too large, stop */
  if (fabs(speed_percent) > 100) return;

  /* if the ratio is equal to or less than zero, stop the motor */
  if (speed_percent < 0.0001) {
    HAL_TIM_PWM_Stop(dev->htim, dev->channel);
    return;
  }

  /* define the period */
  int setPeriod = (speed_percent / 100.0) * dev->htim->Init.Period;
  __HAL_TIM_SET_COMPARE(dev->htim, dev->channel, setPeriod);
}

#endif  // HAS_DEV(DC_MOTOR_PWM)
