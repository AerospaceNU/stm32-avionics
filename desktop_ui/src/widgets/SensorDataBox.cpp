#include "SensorDataBox.h"

#include <fmt/format.h>

// #include "board_config.h"
// #include "board_config_common.h"

using namespace ImGui;

template <typename XYZ>
static void AddXYZData(XYZ data, const char* fmt_string, const char* valueFmt,
                       const char* units) {
  TableNextRow();
  TableSetColumnIndex(1);
  Text(fmt::format(fmt_string, "x").c_str());
  TableSetColumnIndex(2);
  Text(valueFmt, data.z);
  TableSetColumnIndex(3);
  Text(units);

  TableNextRow();
  TableSetColumnIndex(1);
  Text(fmt::format(fmt_string, "y").c_str());
  TableSetColumnIndex(2);
  Text(valueFmt, data.z);
  TableSetColumnIndex(3);
  Text(units);

  TableNextRow();
  TableSetColumnIndex(1);
  Text(fmt::format(fmt_string, "z").c_str());
  TableSetColumnIndex(2);
  Text(valueFmt, data.z);
  TableSetColumnIndex(3);
  Text(units);
}

template <typename T>
static void AddSensorReading(const char* name, const char* fmt, T reading,
                             const char* units) {
  TableNextRow();
  TableSetColumnIndex(1);
  Text(name);
  TableSetColumnIndex(2);
  Text(fmt, reading);
  TableSetColumnIndex(3);
  Text(units);
}

void SensorDataBox::update_widget(const FrameUpdate& f) { state = f; }
void SensorDataBox::display() const {
  ImGui::Begin("SensorData");

  if (ImGui::BeginTable("Test table", 4)) {

    TableSetupColumn("Sensor");
    TableSetupColumn("Key");
    TableSetupColumn("Value");
    TableSetupColumn("Units");
    TableHeadersRow();

    bool open;

// #if HAS_DEV(IMU)
//     //   ImuData_s imuData[NUM_IMU];

//     for (int i = 0; i < NUM_IMU; i++) {
//       // New row for the tree node
//       TableNextRow();
//       TableSetColumnIndex(0);
//       open = TreeNodeEx(fmt::format("imu_{}", i).c_str(),
//                         ImGuiTreeNodeFlags_SpanFullWidth, "IMU %i", i);

//       if (open) {
//         // accel, per IMU
//         TableNextRow();
//         TableSetColumnIndex(0);
//         open = TreeNodeEx(fmt::format("imu_{}_accel", i).c_str(),
//                           ImGuiTreeNodeFlags_SpanFullWidth, "Accel");
//         if (open) {
//           auto& data = state.sensorData.imuData[i];

//           AddXYZData<>(data.accelRealMps2, "a{}_real", "%.2f", "(m/s/s)");
//           AddXYZData<>(data.accelRaw, "a{}_raw", "%.2f", "(ticks)");

//           TreePop();
//         }

//         // angular velocity, per IMU
//         TableNextRow();
//         if (open) Unindent();
//         TableSetColumnIndex(0);
//         open = TreeNodeEx(fmt::format("imu_{}_gyro", i).c_str(),
//                           ImGuiTreeNodeFlags_SpanFullWidth, "Gyro");
//         if (open) {
//           auto& data = state.sensorData.imuData[i];

//           AddXYZData<>(data.angVelRealRadps, "w{}_real", "%.2f", "(rad/s)");
//           AddXYZData<>(data.angVelRaw, "w{}_raw", "%.2f", "(ticks)");

//           TreePop();
//         }

//         // mag flux, per IMU
//         TableNextRow();
//         // if (open) Unindent();
//         TableSetColumnIndex(0);
//         open = TreeNodeEx(fmt::format("imu_{}_mag", i).c_str(),
//                           ImGuiTreeNodeFlags_SpanFullWidth, "Mag");
//         if (open) {
//           auto& data = state.sensorData.imuData[i];

//           AddXYZData<>(data.angVelRealRadps, "b{}_real", "%.2f", "(Gauss)");
//           AddXYZData<>(data.angVelRaw, "b{}_raw", "%.2f", "(ticks)");

//           TreePop();
//         }
//         TreePop();
//       }
//     }

// #endif  // HAS_DEV(IMU)

// #if HAS_DEV(ACCEL)
//     for (int i = 0; i < NUM_ACCEL; i++) {
//       // New row for the tree node
//       TableNextRow();
//       TableSetColumnIndex(0);
//       open = TreeNodeEx(fmt::format("accel_{}", i).c_str(),
//                         ImGuiTreeNodeFlags_SpanFullWidth, "Accel %i", i);

//       if (open) {
//         auto& data = state.sensorData.accelData[i];

//         AddXYZData<>(data.realMps2, "a{}_real", "%.2f", "(m/s/s)");
//         AddXYZData<>(data.raw, "a{}_raw", "%.2f", "(ticks)");

//         TreePop();
//       }
//     }
// #endif  // HAS_DEV(ACCEL)
// #if HAS_DEV(BAROMETER)
//     for (int i = 0; i < NUM_BAROMETER; i++) {
//       // New row for the tree node
//       TableNextRow();
//       TableSetColumnIndex(0);
//       open = TreeNodeEx(fmt::format("baro_{}", i).c_str(),
//                         ImGuiTreeNodeFlags_SpanFullWidth, "Baro %i", i);

//       if (open) {
//         auto& data = state.sensorData.barometerData[i];

//         AddSensorReading("pressure", "%.2f", data.pressureAtm, "atm");
//         AddSensorReading("termperature", "%.2f", data.temperatureC, "degC");

//         TreePop();
//       }
//     }
// #endif  // HAS_DEV(BAROMETER)
// #if HAS_DEV(GPS)
//     for (int i = 0; i < NUM_GPS; i++) {
//       // New row for the tree node
//       TableNextRow();
//       TableSetColumnIndex(0);
//       open = TreeNodeEx(fmt::format("gps_{}", i).c_str(),
//                         ImGuiTreeNodeFlags_SpanFullWidth, "GPS %i", i);

//       if (open) {
//         auto& data = state.sensorData.gpsData[i];

//         AddSensorReading("latitude", "%.2f", data.generalData.latitude, "deg");
//         AddSensorReading("longitude", "%.2f", data.generalData.longitude, "deg");
//         AddSensorReading("altitude", "%.2f", data.generalData.altitude, "M (MSL)");
//         AddSensorReading("sats", "%i", data.generalData.satsTracked, "");
//         AddSensorReading("hdop", "%.2f", data.generalData.hdop, "");
//         AddSensorReading("timestamp", "%llu", data.timeData.timestamp, "");

//         TreePop();
//       }
//     }
// #endif  // HAS_DEV(GPS)
// #if HAS_DEV(VBAT)
//     double vbatData[NUM_VBAT];
// #endif  // HAS_DEV(VBAT)
// #if HAS_DEV(PYRO_CONT)
//     bool pyroContData[NUM_PYRO_CONT];
// #endif  // HAS_DEV(PYRO_CONT)

    ImGui::EndTable();
  }

  ImGui::End();
}
