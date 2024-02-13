#include <gtest/gtest.h>

#include "cli.h"
#include "cli_tasks.h"
#include "desktop_hardware_manager.h"
#include "hardware_manager.h"

// TEST(CliParse, ParseLineCutter) {
//   HM_HardwareInit();
//   auto opt = cliParse(CLI_PHONE);
//   ASSERT_EQ(LINECUTTER, opt);
// }

TEST(CliParse, CliTick) {
  hm_hardwareInit();
  CliTasks::configureForGround();
  auto opt = CliTasks::tick();
  ASSERT_EQ(NoChange, opt);
  desktophm_teardown();
}
