//
// Created by matth on 3/26/2022.
//

#ifndef DEVICES_DESKTOP_FLIGHT_REPLAY_H_
#define DEVICES_DESKTOP_FLIGHT_REPLAY_H_

#include "hardware_manager.h"
#include "rapidcsv.h"

class FlightReplay {
 public:
  virtual void GetNext(SensorData_t *data) {}
};

class CsvReplay : FlightReplay {
 public:
  explicit CsvReplay(std::string path);

  void GetNext(SensorData_t *data) override;

 private:
  rapidcsv::Document doc;
  size_t m_row = 0;
};

#endif  // DEVICES_DESKTOP_FLIGHT_REPLAY_H_
