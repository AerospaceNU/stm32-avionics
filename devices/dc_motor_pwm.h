#ifndef DEVICES_DC_MOTOR_PWM_H_
#define DEVICES_DC_MOTOR_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"

#if HAS_DEV(DC_MOTOR_PWM)

/* Definition of DC motor control struct */
/* user needs to config timer */
typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
} DcMotorPwmCtrl_s;

/* creates a new DC motor that can be started and stopped */
void dcMotorPwm_init(DcMotorPwmCtrl_s *dev, TIM_HandleTypeDef *htim,
                     uint32_t channel);

/* start a specified DC motor with a given speed percentage -100-100 */
void dcMotorPwm_setPercent(DcMotorPwmCtrl_s *dev, double speed_percent);

#endif

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_DC_MOTOR_PWM_H_
