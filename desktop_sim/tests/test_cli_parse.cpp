#include <gtest/gtest.h>

#include "cli.h"
#include "cli_tasks.h"
#include "hardware_manager.h"

TEST(CliParse, ParseLineCutter) {
  HM_HardwareInit();
  auto opt = cliParse(CLI_PHONE);
  ASSERT_EQ(LINECUTTER, opt);
}

TEST(CliParse, CliTick) {
  HM_HardwareInit();
  cli_tasks::ConfigureForGround();
  auto opt = cli_tasks::cliTick();
  ASSERT_EQ(NoChange, opt);
}
