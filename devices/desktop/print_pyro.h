/*
 * desktop_pyro.h
 *
 *  Created on: Aug 9, 2022
 *      Author: Ben
 */

#ifndef DEVICES_DESKTOP_PRINT_PYRO_H_
#define DEVICES_DESKTOP_PRINT_PYRO_H_

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
void printPyro_tick(PrintPyroCtrl_s *pyro);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DEVICES_DESKTOP_PRINT_PYRO_H_
