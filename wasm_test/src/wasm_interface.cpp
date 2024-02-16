#include <emscripten/bind.h>

#include <iostream>

#include "cli.h"
#include "cli_tasks.h"
#include "event_manager.h"
#include "expression_store.h"
#include "hardware_manager.h"
#include "wasm_hardware_manager.h"
#include "wasm_helpers.h"

using namespace emscripten;

int main() {
  // do setup things here, main called in setup
  std::cout << "Hi from main!" << std::endl;
}

// Define a struct to hold the data
struct AltitudeResult {
  double timestamp;
  bool success;
  double angle_vertical;
  double pos_z_agl;
  double rocket_vel_z;
  double rocket_acc_z;
};

static std::vector<std::tuple<double, FilterData_s>> generateFakeFlight() {
  std::vector<std::tuple<double, FilterData_s>> ret;

  auto tmax = 30;
  auto dt = 1.0/65.0;

  FilterData_s lastFilterData = {0};

  for (double t = 0; t < tmax; t += dt) {
    double az;
    // fake impulse
    if (t < 5) az=0;
    else if (t < 7) az=30;
    else az=-10;

    lastFilterData.rocket_acc_z = az;

    // forward euler go brr via double integration
    lastFilterData.rocket_vel_z += lastFilterData.rocket_acc_z * dt;
    lastFilterData.pos_z_agl += lastFilterData.rocket_vel_z * dt + lastFilterData.rocket_acc_z * dt * dt;

    // cap fall speed
    auto MAX_SPEED=-30;
    if (lastFilterData.rocket_vel_z < MAX_SPEED) lastFilterData.rocket_vel_z = MAX_SPEED;

    if (lastFilterData.pos_z_agl < 0) {
      lastFilterData.rocket_vel_z = 0;
      lastFilterData.pos_z_agl = 0;
      lastFilterData.rocket_acc_z = 0;
    }

    // add to list
    ret.emplace_back(t, lastFilterData);
  }

  return ret;
}

auto parseExpression(std::string expression) {
  hm_hardwareInit();
  cli_init();
  cli_setDefaultConfig();
  ExpressionStore expressionStore;
  expressionStore.init();

  std::cout << "Got expression: " << expression << std::endl;

  const char* data = expression.data();
  StringSlice slice1 = StringSlice(&data, 0, expression.length());

  uint16_t res1ID;
  ExpressionValueType_e valueType =
      expressionStore.parseForTrigger(&res1ID, 0, slice1, 0);
  expressionStore.writeNewConfigs();

  char stringBuf[1000];
  expressionStore.conditionToString(res1ID, stringBuf, sizeof(stringBuf));
  std::cout << "Recited: " << stringBuf << std::endl;

  auto flight = generateFakeFlight();

  std::vector<AltitudeResult> ret;
  for (auto& [time, filterData]: flight) {
    expressionStore.tick(&filterData);

    ret.push_back({.timestamp = time,
                   .success = expressionStore.getExprBoolValue(res1ID),
                   .pos_z_agl = filterData.pos_z_agl,
                   .rocket_vel_z = filterData.rocket_vel_z,
                   .rocket_acc_z = filterData.rocket_acc_z,
                   });
  }

  return ret;
}

EMSCRIPTEN_BINDINGS(wasm_meme) {
  value_object<AltitudeResult>("AltitudeResult")
      .field("timestamp", &AltitudeResult::timestamp)
      .field("success", &AltitudeResult::success)
      .field("pos_z_agl", &AltitudeResult::angle_vertical)
      .field("angle_z", &AltitudeResult::pos_z_agl)
      .field("rocket_vel_z", &AltitudeResult::rocket_vel_z)
      .field("rocket_acc_z", &AltitudeResult::rocket_acc_z)
      ;

  function("parseExpression", &parseExpression);
}