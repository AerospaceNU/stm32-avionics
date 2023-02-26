/*
 * nt_pyro.h
 *
 *  Created on: Aug 9, 2022
 *      Author: Ben
 */

#ifndef DEVICES_DESKTOP_NT_PYRO_H_
#define DEVICES_DESKTOP_NT_PYRO_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "board_config.h"
#include "board_config_common.h"

#if HAS_DEV(PYRO_DESKTOP_NT)

#include <ntcore_c.h>
#include <stdint.h>

typedef struct {
  uint32_t expireTime;
  int id;
  NT_Topic stateTopic;
  NT_Publisher statePublisher;
} NtPyroCtrl_s;

void ntPyro_init(NtPyroCtrl_s *pyro, int id);
void ntPyro_start(NtPyroCtrl_s *pyro, uint32_t duration);
void ntPyro_set(NtPyroCtrl_s *pyro, bool enable);
void ntPyro_tick(NtPyroCtrl_s *pyro);
void ntPyro_pwmStart(NtPyroCtrl_s *pyro, uint32_t duration, uint32_t frequency,
                     uint32_t pulseWidth);

#endif  // HAS_DEV(PYRO_DESKTOP_NT)

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DEVICES_DESKTOP_NT_PYRO_H_
