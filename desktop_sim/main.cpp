#include "desktop_hardware_manager.h"
#include "scheduler.h"
#include "tcp_socket.h"

#include "Application.h"

int main(int argC, char** argv) {
  Application app;

  // add widgets

  app.run();

  if (argC > 3) {
    output_file = argv[1];
    ext_flash_path = argv[2];
    int_flash_path = argv[3];
    printf("Running with output file %s, ext flash %s, int flash %s\n",
           output_file.c_str(), ext_flash_path.c_str(), int_flash_path.c_str());
  } else {
    printf(
        "Must have 3 args! Eg ./desktop_sim path/to/flight.csv "
        "external_flash.dat internal_flash.dat\n");
    return 1;
  }

  Scheduler s = Scheduler();
  s.run();

  return 0;
}
