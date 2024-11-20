/*
 * entrypoint.cpp
 *
 * Dummy file so that our main main.c function can call into Scheduler, which is
 * C++
 *
 *  Created on: Mar 29, 2023
 *      Author: mmorley@na.jnj.com
 */

#include "entrypoint.h"

#include "scheduler.h"

extern "C" {

void main_cpp() {
  Scheduler s;
  s.run();

  while (1) {
  }
}
}
