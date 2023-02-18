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

  uint32_t nextExpireTime = HM_Millis();
  for (int i = 0; i < 100000; i++) {
    // This should guarantee that we wait the correct quantity of time
    uint32_t delta = s.getNextExpirationTimeMillis() - nextExpireTime;
    timing::StepTimingAsync(delta);

    s.tick();
  } 

  printf("Done stepping ticks %lu\n", timing::GetTickNumber());
  HM_Sim_Exit();

}
