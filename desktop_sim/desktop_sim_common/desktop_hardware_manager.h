#ifndef DESKTOP_SIM_DESKTOP_SIM_COMMON_DESKTOP_HARDWARE_MANAGER_H_
#define DESKTOP_SIM_DESKTOP_SIM_COMMON_DESKTOP_HARDWARE_MANAGER_H_

#include <memory>
#include <string>

#include "hardware_manager.h"
#include "hardware_manager_interface.h"

void hm_sim_setHM(std::shared_ptr<HardwareManagerInterface> hmInterface);

#endif  // DESKTOP_SIM_DESKTOP_SIM_COMMON_DESKTOP_HARDWARE_MANAGER_H_
