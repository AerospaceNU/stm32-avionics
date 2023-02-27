//
// Created by matth on 3/26/2022.
//

#include "board_config.h"
#include "board_config_common.h"

#if HAS_DEV(PYRO_SIM_KRPC)

#include "flight_replay.h"

#include <algorithm>  // std::min
#include <string>

#include "hardware_manager.h"

KRPCFlightReplay::KRPCFlightReplay()
  : conn(krpc::connect("FCB Sim"))
  , spaceCenter(&conn)
  , vessel(spaceCenter.active_vessel())
  , control(vessel.control())
  , flight(vessel.flight())
  , utcTime { spaceCenter.ut_stream() }
  , staticPress { flight.static_pressure_stream() }
  , airTemp { flight.total_air_temperature_stream() }
  , latitude { flight.latitude_stream() }
  , longitude { flight.longitude_stream() }
  , gpsAlt { flight.mean_altitude_stream() }

  // the y-axis points in the same direction that the vessel is pointing,
  // the x-axis points out of the right side of the vessel,
  // the z-axis points downwards out of the bottom of the vessel,
  // But we have +X forward
  // Want a 90 degree rotation about -z
  , vesselRef(vessel.reference_frame())
  , angularVel { vessel.angular_velocity_stream(vesselRef) }
  {}


void KRPCFlightReplay::writeActionGroup(int group, bool state) {
  group = group + 1;
  printf("Setting group %i to %i\n", group, state);
  control.set_action_group(group, state);
}

#define R_DRY_AIR 8.314   // J/K/kg
#define G_ACCEL_EARTH 9.80665   // m/s**2

void KRPCFlightReplay::getNext(SensorData_s* data) {
  data->timestampMs = utcTime() * 1000;

#if HAS_DEV(GPS_DESKTOP_FILE)
    for (int i = 0; i < NUM_GPS_DESKTOP_FILE; i++) {
      data->gpsData[i].generalData.latitude = latitude();
      data->gpsData[i].generalData.longitude = longitude();
      data->gpsData[i].generalData.altitude = gpsAlt();
    }
#endif  // HAS_DEV(GPS_DESKTOP_FILE)

#if HAS_DEV(BAROMETER_DESKTOP_FILE)
    for (int i = 0; i < NUM_BAROMETER_DESKTOP_FILE; i++) {
      // https://en.wikipedia.org/wiki/Barometric_formula
      double gps = gpsAlt();
      data->barometerData[i].pressureAtm = 1.0 * exp(-G_ACCEL_EARTH * 0.0289644 * gps / (R_DRY_AIR * 273.15));
      data->barometerData[i].temperatureC = airTemp() - 273.15;
    }
#endif  // HAS_DEV(BAROMETER_DESKTOP_FILE)

    // X up, Y north, Z east
    auto [wx, wy, wz] = vessel.angular_velocity(vessel.surface_reference_frame());

    // +x is to the right
    // +y up
    // +Z also out to the side
    auto [fx, fy, fz] = flight.aerodynamic_force();
    fy = fy + vessel.thrust();
    if (fy < 1e6 && gpsAlt() < 90) {
      // on ground
      fy = 9.81;
    }
    auto mass = vessel.mass(); // kg

    auto ax = fx / mass;
    auto ay = fy / mass;
    auto az = fz / mass;
    if (mass < 1e-4) {
      ax = 0;
      ay = 0;
      az = 0;
    }

#if HAS_DEV(IMU_DESKTOP_FILE)

    for (int i = 0; i < NUM_IMU_DESKTOP_FILE; i++) {
      data->imuData[i].accelRealMps2.x = ax;
      data->imuData[i].accelRealMps2.y = ay;
      data->imuData[i].accelRealMps2.z = az;

      data->imuData[i].angVelRealRadps.x = wx;
      data->imuData[i].angVelRealRadps.y = wy;
      data->imuData[i].angVelRealRadps.z = wz;

      // data->imuData[i].magRealG.x = 
      // data->imuData[i].magRealG.y = 
      // data->imuData[i].magRealG.z = 
    }
#endif  // HAS_DEV(IMU_DESKTOP_FILE)


#if HAS_DEV(ACCEL_DESKTOP_FILE)
    for (int i = 0; i < NUM_ACCEL_DESKTOP_FILE; i++) {
      // high g x in -y
      // y in -x
      // z in z 
      // relative to IMU
      data->accelData[i].realMps2.x = -ay;
      data->accelData[i].realMps2.y = -ax;
      data->accelData[i].realMps2.z = az;
    }
#endif  // HAS_DEV(ACCEL_DESKTOP_FILE)
}

#endif // HAS_DEV(PYRO_SIM_KRPC)
