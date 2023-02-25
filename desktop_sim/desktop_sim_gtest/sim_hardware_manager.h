#pragma once

#include "board_config.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "file_backed_flash.h"
#include "flight_replay.h"
#include "hardware_manager.h"
#include "hardware_manager_interface.h"
#include "print_pyro.h"
#include "sim_timing.h"

class SimHardwareManager : public HardwareManagerInterface {
 public:
  inline SimHardwareManager() : HardwareManagerInterface() {}

 private:
  SensorData_s data;
  SensorProperties_s properties;

 public:
  inline SensorData_s &SensorData() { return data; }
  inline SensorProperties_s &SensorProperties() { return properties; }

 public:
  uint32_t hm_millis() override;
  void hm_observeTickComplete(uint64_t tickNum) override;
};
