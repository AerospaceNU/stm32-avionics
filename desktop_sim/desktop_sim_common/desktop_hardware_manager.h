#ifndef DESKTOP_SIM_DESKTOP_SIM_COMMON_DESKTOP_HARDWARE_MANAGER_H_
#define DESKTOP_SIM_DESKTOP_SIM_COMMON_DESKTOP_HARDWARE_MANAGER_H_

#include <string>

#include "hardware_manager.h"

extern std::string int_flash_path;
extern std::string ext_flash_path;
extern std::string output_file;

extern bool do_networking;

#ifdef __cplusplus
extern "C" {
#endif
void HM_Sim_Exit();
#ifdef __cplusplus
}
#endif

#endif  // DESKTOP_SIM_DESKTOP_SIM_COMMON_DESKTOP_HARDWARE_MANAGER_H_
