#include "sim_hardware_manager.h"
#include <thread>

uint32_t SimHardwareManager::hm_millis() {
  return timing::GetProgramTimeMillis();
}

void SimHardwareManager::hm_observeTickComplete(uint64_t tickNum) {
  timing::SetTickCompleted(tickNum);
}
