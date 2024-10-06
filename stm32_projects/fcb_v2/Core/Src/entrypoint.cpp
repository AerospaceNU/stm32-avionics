/*
` * entrypoint.cpp
 *
 *  Created on: Mar 29, 2023
 *      Author: mmorley@na.jnj.com
 */


#include "scheduler.h"

extern "C" {

void main_cpp() {
	Scheduler s;
	s.run();

	while (1);
}
}
