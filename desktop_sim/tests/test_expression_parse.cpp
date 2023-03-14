#include <gtest/gtest.h>

#include <iostream>

#include "cli.h"
#include "cli_tasks.h"
#include "expression_store.h"
#include "hardware_manager.h"

// TEST(CliParse, ParseLineCutter) {
//   HM_HardwareInit();
//   auto opt = cliParse(CLI_PHONE);
//   ASSERT_EQ(LINECUTTER, opt);
// }

TEST(ExpressionStorage, expressionParsing) {
  FilterData_s filterData;
  char stringBuf[1000];
  hm_hardwareInit();
  cli_init();
  cli_setDefaultConfig();
  ExpressionStore expressionStore;
  expressionStore.init();
  expressionStore.removeExpressionsForTrigger(0);
  expressionStore.removeExpressionsForTrigger(1);
  expressionStore.removeExpressionsForTrigger(2);
  ASSERT_EQ(expressionStore.getNextExpressionSpot(0), 0);
  int resID;
  const char *test1Input =
      "((2.00 after burnout) and ((not apogee) and ((world_vel_z > 35.00) and "
      "((angle_vertical < 30.00) and (not (ever (launch and (angle_vertical > "
      "45.00))))))))";
  StringSlice slice = StringSlice(&test1Input, 0, strlen(test1Input));
  ExpressionValueType_e valueType =
      expressionStore.parseForTrigger(&resID, 0, slice, 0);
  expressionStore.writeNewConfigs();
  ASSERT_EQ(valueType, ExpressionValueType_e::boolean);
  expressionStore.conditionToString(resID, stringBuf, sizeof(stringBuf));
  ASSERT_EQ(strcmp(stringBuf, test1Input), 0);

  const char *test2Input =
      "(-6.00 == (12.00 - (9.00 * (1.00 + (2.00 / 2.00)))))";
  slice = StringSlice(&test2Input, 0, strlen(test2Input));
  valueType = expressionStore.parseForTrigger(&resID, 0, slice, 0);
  expressionStore.writeNewConfigs();
  ASSERT_EQ(valueType, ExpressionValueType_e::boolean);
  expressionStore.conditionToString(resID, stringBuf, sizeof(stringBuf));
  ASSERT_EQ(strcmp(stringBuf, test2Input), 0);
  expressionStore.tick(&filterData);
  ASSERT_TRUE(expressionStore.getExprBoolValue(resID));
}
