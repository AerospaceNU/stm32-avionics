/*
 * desktop_pyro.h
 *
 *  Created on: Aug 9, 2022
 *      Author: Ben
 */

#ifndef DEVICES_DESKTOP_KRPC_PYRO_H_
#define DEVICES_DESKTOP_KRPC_PYRO_H_

#include <functional>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

typedef struct {
  uint32_t expireTime;
  int id;
  std::function<void(int, bool)> callback;
} KRPCPyroCtrl_s;

void krpcPyro_init(KRPCPyroCtrl_s *pyro, int id, std::function<void(int, bool)> callback);
void krpcPyro_start(KRPCPyroCtrl_s *pyro, uint32_t duration);
void krpcPyro_set(KRPCPyroCtrl_s *pyro, bool enable);
void krpcPyro_tick(KRPCPyroCtrl_s *pyro);
void krpcPyro_pwmStart(KRPCPyroCtrl_s *pyro, uint32_t duration,
                        uint32_t frequency, uint32_t pulseWidth);
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DEVICES_DESKTOP_PRINT_PYRO_H_
