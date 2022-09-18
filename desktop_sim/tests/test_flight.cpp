#include <gtest/gtest.h>
#include "scheduler.h"
#include <thread>
#include "desktop_hardware_manager.h"

TEST(FullFlight, TestFlightLands) {
    // It's tempermental about the working directory
    output_file = "resources/superguppy-fcb-output-post.csv";
    ext_flash_path = "ext-flash-test.hex";
    int_flash_path = "int-flash-test.hex";
    do_networking = false;
    Scheduler s = Scheduler();
    // std::thread flightThread([&](){
        // s.run();
    // });
    // flightThread.join();
}