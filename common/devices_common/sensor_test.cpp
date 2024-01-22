#include "sensor.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "bad_barometer.hpp"
#include "data_structures.h"

int main() {
  using namespace std::chrono_literals;
  BadBarometer bad_baro_1{3};
  BadBarometer bad_baro_2{5};

  Barometer* sensors[] = {&bad_baro_1, &bad_baro_2};
  static BarometerData_s baro_data;
  UpdateStatus baro_status;
  while (true) {
    for (int barometer_num = 0;
         barometer_num < sizeof(sensors) / sizeof(sensors[0]);
         ++barometer_num) {
      std::cout << "Updating barometer " << barometer_num << std::endl;
      baro_status = sensors[barometer_num]->update(baro_data);
      if (baro_status == UpdateStatus::NEW_DATA) {
        std::cout << "New barometer data from sensor " << barometer_num << ": "
                  << std::endl;
        std::cout << "Temperature: " << baro_data.temperatureC << std::endl;
        std::cout << "Pressure: " << baro_data.pressureAtm << std::endl;
      }
    }

    std::this_thread::sleep_for(2000ms);
  }
}