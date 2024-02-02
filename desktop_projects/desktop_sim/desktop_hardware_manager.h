#ifndef DESKTOP_PROJECTS_DESKTOP_SIM_DESKTOP_HARDWARE_MANAGER_H_
#define DESKTOP_PROJECTS_DESKTOP_SIM_DESKTOP_HARDWARE_MANAGER_H_

#include <string>

#include "hardware_manager.h"

extern std::string int_flash_path;
extern std::string ext_flash_path;
extern std::string output_file;

extern bool do_networking;

void desktophm_teardown();

#endif  // DESKTOP_PROJECTS_DESKTOP_SIM_DESKTOP_HARDWARE_MANAGER_H_
