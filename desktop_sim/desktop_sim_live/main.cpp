#include "desktop_hardware_manager.h"
#include "nt_hardware_manager.h"
#include "scheduler.h"
#include "tcp_socket.h"
#include "trigger_manager.h"

int main(int argC, char** argv) {
  if (argC > 3) {
    // output_file = argv[1];
    // ext_flash_path = argv[2];
    // int_flash_path = argv[3];
    // printf("Running with output file %s, ext flash %s, int flash %s\n",
    //        output_file.c_str(), ext_flash_path.c_str(),
    //        int_flash_path.c_str());

    // do_networking = true;

    hm_sim_setHM(std::make_unique<NtHardwareManager>(argv[1], argv[2], argv[3],
                                                     argv[4]));

  } else {
    printf(
        "Must have 3 args! Eg ./desktop_sim path/to/flight.csv "
        "external_flash.dat internal_flash.dat\n");
    return 1;
  }

  Scheduler s{};
  // HACK -- step forward 1/2 sec
  for (int i = 0; i < 30; i++) {
    s.tick();
    while (!s.hasTimerExpired()) {
      // busywait
    }
  }

  // Hack in hard-coded pyro configs
  // const char* apogee = "apogee";
  // const char* main = "(apogee and (pos_z_agl < 200))";
  // triggerManager_setTriggerConfig(0, &apogee);
  // triggerManager_setTriggerConfig(1, &main);

  while (true) {
    s.tick();
    while (!s.hasTimerExpired()) {
      // busywait
    }
  }

  return 0;
}
