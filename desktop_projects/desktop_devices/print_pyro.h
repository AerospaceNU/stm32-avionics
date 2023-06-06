/*
 * desktop_pyro.h
 *
 *  Created on: Aug 9, 2022
 *      Author: Ben
 */

#ifndef DESKTOP_PROJECTS_DESKTOP_DEVICES_PRINT_PYRO_H_
#define DESKTOP_PROJECTS_DESKTOP_DEVICES_PRINT_PYRO_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

typedef struct {
  uint32_t expireTime;
  int id;
} PrintPyroCtrl_s;

void printPyro_init(PrintPyroCtrl_s *pyro, int id);
void printPyro_start(PrintPyroCtrl_s *pyro, uint32_t duration);
void printPyro_set(PrintPyroCtrl_s *pyro, bool enable);
void printPyro_tick(PrintPyroCtrl_s *pyro);
void printPyro_pwmStart(PrintPyroCtrl_s *pyro, uint32_t duration,
                        uint32_t frequency, uint32_t pulseWidth);
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DESKTOP_PROJECTS_DESKTOP_DEVICES_PRINT_PYRO_H_
