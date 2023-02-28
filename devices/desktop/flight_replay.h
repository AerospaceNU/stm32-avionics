//
// Created by matth on 3/26/2022.
//

#ifndef DEVICES_DESKTOP_FLIGHT_REPLAY_H_
#define DEVICES_DESKTOP_FLIGHT_REPLAY_H_

#include "hardware_manager.h"
#include "rapidcsv.h"

class FlightReplay {
 public:
  virtual void getNext(SensorData_s *data) {}
};

class OpenRocketFLightReplay : FlightReplay {
 public:
  explicit OpenRocketFLightReplay(std::string path);

  void getNext(SensorData_s *data) override;

 private:
  rapidcsv::Document doc;
  int m_ticksToLaunch = 15 * 65; // TODO
  double lastTime = 0;
  size_t m_row = 0;
};

class FcbCsvFlightReplay : FlightReplay {
 public:
  explicit FcbCsvFlightReplay(std::string path);

  void getNext(SensorData_s *data) override;

 private:
  rapidcsv::Document doc;
  size_t m_row = 0;
};

#endif  // DEVICES_DESKTOP_FLIGHT_REPLAY_H_
