#include "SensorDataBox.h"

#include "board_config.h"
#include "board_config_common.h"

#include <fmt/format.h>

void SensorDataBox::update_widget(const FrameUpdate& f) { state = f; }
void SensorDataBox::display() const {
  ImGui::Begin("SensorData");

  if (ImGui::BeginTable("Test table", 3)) {
    using namespace ImGui;

    TableSetupColumn("Sensor");
    TableSetupColumn("Key");
    TableSetupColumn("Value");
    TableHeadersRow();

	bool open;

#if HAS_DEV(IMU)
    //   ImuData_s imuData[NUM_IMU];

    for (int i = 0; i < NUM_IMU; i++) {
      // New row for the tree node
      TableNextRow();
      TableSetColumnIndex(0);
      open = TreeNodeEx(fmt::format("imu_{}", i).c_str(), ImGuiTreeNodeFlags_SpanFullWidth, "IMU %i", i);

      if (open) {
		TableNextRow();
		TableSetColumnIndex(0);
		open = TreeNodeEx(fmt::format("imu_{}_accel", i).c_str(), ImGuiTreeNodeFlags_SpanFullWidth, "Accel");
		if (open) {
			auto& data = state.sensorData.imuData[i];

			TableNextRow();
			TableSetColumnIndex(1); Text("ax");
			TableSetColumnIndex(2); Text("%.2f", data.accelRealMps2.x); 
			TableNextRow();
			TableSetColumnIndex(1); Text("ay");
			TableSetColumnIndex(2); Text("%.2f", data.accelRealMps2.y); 
			TableNextRow();
			TableSetColumnIndex(1); Text("az");
			TableSetColumnIndex(2); Text("%.2f", data.accelRealMps2.z); 

			TableNextRow();
			TableSetColumnIndex(1); Text("ax_raw");
			TableSetColumnIndex(2); Text("%i", data.accelRaw.x); 
			TableNextRow();
			TableSetColumnIndex(1); Text("ay_raw");
			TableSetColumnIndex(2); Text("%i", data.accelRaw.y); 
			TableNextRow();
			TableSetColumnIndex(1); Text("az_raw");
			TableSetColumnIndex(2); Text("%i", data.accelRaw.z); 

			TreePop();
		}
		TableNextRow();
		if (open) Unindent();
		TableSetColumnIndex(0);
		open = TreeNodeEx(fmt::format("imu_{}_gyro", i).c_str(), ImGuiTreeNodeFlags_SpanFullWidth, "Gyro");
		if (open) {
			auto& data = state.sensorData.imuData[i];

			TableNextRow();
			TableSetColumnIndex(1); Text("wx");
			TableSetColumnIndex(2); Text("%.2f", data.angVelRealRadps.x); 
			TableNextRow();
			TableSetColumnIndex(1); Text("wy");
			TableSetColumnIndex(2); Text("%.2f", data.angVelRealRadps.y); 
			TableNextRow();
			TableSetColumnIndex(1); Text("wz");
			TableSetColumnIndex(2); Text("%.2f", data.angVelRealRadps.z); 

			TableNextRow();
			TableSetColumnIndex(1); Text("wx_raw");
			TableSetColumnIndex(2); Text("%i", data.angVelRaw.x); 
			TableNextRow();
			TableSetColumnIndex(1); Text("wy_raw");
			TableSetColumnIndex(2); Text("%i", data.angVelRaw.y); 
			TableNextRow();
			TableSetColumnIndex(1); Text("wz_raw");
			TableSetColumnIndex(2); Text("%i", data.angVelRaw.z); 

			TreePop();
		}
		TreePop();
      }
    }

#endif  // HAS_DEV(IMU)

#if HAS_DEV(ACCEL)
    AccelData_s accelData[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
    BarometerData_s barometerData[NUM_BAROMETER];
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(GPS)
    GpsData_s gpsData[NUM_GPS];
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
    double vbatData[NUM_VBAT];
#endif  // HAS_DEV(VBAT)
#if HAS_DEV(PYRO_CONT)
    bool pyroContData[NUM_PYRO_CONT];
#endif  // HAS_DEV(PYRO_CONT)

    ImGui::EndTable();
  }

  ImGui::End();
}
