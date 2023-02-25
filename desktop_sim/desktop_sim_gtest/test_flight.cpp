#include <gtest/gtest.h>

#include <thread>

#include "desktop_hardware_manager.h"
#include "scheduler.h"
#include "sim_timing.h"

TEST(FullFlight, TestFlightLands) {
  // It's tempermental about the working directory
  // output_file =
  //     "/home/mmorley@na.jnj.com/Documents/github/stm32-avionics/resources/"
  //     "superguppy-fcb-output-post.csv";
  // ext_flash_path = "ext-flash-test.hex";
  // int_flash_path = "int-flash-test.hex";
  // do_networking = false;

  timing::RestartTiming();
  timing::PauseTiming();

  Scheduler s = Scheduler();

  uint32_t lastExpireTime = hm_millis();
  for (int i = 0; i < 50; i++) {
    // This should guarantee that we wait the correct quantity of time
    uint32_t nextExpireTime = s.getNextExpirationTimeMillis();
    uint32_t delta = nextExpireTime - lastExpireTime;
    timing::StepTimingAsync(delta * 1000);

    if (s.hasTimerExpired()) s.tick();
  }

  printf("Done stepping ticks %lu\n", timing::GetTickNumber());
}
