//
// Created by matth on 3/26/2022.
//

#ifndef DEVICES_DESKTOP_FLIGHT_REPLAY_H_
#define DEVICES_DESKTOP_FLIGHT_REPLAY_H_

#include "board_config.h"
#include "board_config_common.h"
#include "hardware_manager.h"
#include "rapidcsv.h"

class FlightReplay {
 public:
  virtual void getNext(SensorData_s *data) {}
};

#include <krpc.hpp>
#include <krpc/services/space_center.hpp>
#include <krpc/services/ui.hpp>
#include <krpc/services/drawing.hpp>

class KRPCFlightReplay : public FlightReplay {
 public:
  explicit KRPCFlightReplay();

  void getNext(SensorData_s *data) override;

  void writeActionGroup(int group, bool state);

 private:
  krpc::Client conn;
  krpc::services::SpaceCenter spaceCenter;
  krpc::services::SpaceCenter::Vessel vessel;
  krpc::services::SpaceCenter::Control control;
  krpc::services::SpaceCenter::Flight flight;
  krpc::Stream<double> utcTime;
  krpc::Stream<float> staticPress;
  krpc::Stream<float> airTemp;
  krpc::Stream<double> latitude;
  krpc::Stream<double> longitude;
  krpc::Stream<double> gpsAlt;

  krpc::services::SpaceCenter::ReferenceFrame vesselRef;
  krpc::Stream<std::tuple<double, double, double>> angularVel;
};

class CsvReplay : FlightReplay {
 public:
  explicit CsvReplay(std::string path);

  void getNext(SensorData_s *data) override;

 private:
  rapidcsv::Document doc;
  size_t m_row = 0;
};


#endif  // DEVICES_DESKTOP_FLIGHT_REPLAY_H_
