//
// Created by matth on 3/26/2022.
//

#ifndef DESKTOP_PROJECTS_DESKTOP_DEVICES_FLIGHT_REPLAY_H_
#define DESKTOP_PROJECTS_DESKTOP_DEVICES_FLIGHT_REPLAY_H_

#include "hardware_manager.h"
#include "rapidcsv.h"

class FlightReplay {
 public:
  virtual void getNext(SensorData_s *data) {}

  virtual ~FlightReplay() = default;
};

class CsvReplay : FlightReplay {
 public:
  explicit CsvReplay(std::string path);

  void getNext(SensorData_s *data) override;

  ~CsvReplay() = default;

 private:
  rapidcsv::Document doc;
  size_t m_row = 0;
};

#endif  // DESKTOP_PROJECTS_DESKTOP_DEVICES_FLIGHT_REPLAY_H_
