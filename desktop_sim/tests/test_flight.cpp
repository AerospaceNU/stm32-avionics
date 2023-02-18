#include <gtest/gtest.h>

#include "desktop_hardware_manager.h"
#include "scheduler.h"
#include <thread>
#include "../sim_timing.h"

TEST(FullFlight, TestFlightLands) {
  // It's tempermental about the working directory
  output_file = "/home/mmorley@na.jnj.com/Documents/github/stm32-avionics/resources/superguppy-fcb-output-post.csv";
  ext_flash_path = "ext-flash-test.hex";
  int_flash_path = "int-flash-test.hex";
  do_networking = false;

  timing::RestartTiming();
  timing::PauseTiming();

  Scheduler s = Scheduler();
  std::thread flightThread([&](){
    s.run();
  });

  auto start = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (int i = 0; i < 1; i++) {
    timing::StepTiming(14 * 1e3);
  } 
  auto delta = (std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch()) - start).count();

  printf("Done stepping in %lu ticks %lu\n", delta, timing::GetTickNumber());
  HM_Sim_Exit();

  flightThread.join();

}
