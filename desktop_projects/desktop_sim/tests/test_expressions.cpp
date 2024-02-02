#include <gtest/gtest.h>

#include <iostream>

#include "cli.h"
#include "cli_tasks.h"
#include "desktop_hardware_manager.h"
#include "event_manager.h"
#include "expression_store.h"
#include "hardware_manager.h"

class ExpressionStorageTests : public testing::Test {
 protected:
  void SetUp() override {
    hm_hardwareInit();
    cli_init();
    cli_setDefaultConfig();
    expressionStore.init();
  }

  void TearDown() override { desktophm_teardown(); }

  FilterData_s filterData;
  char stringBuf[1000];
  ExpressionStore expressionStore;
  uint16_t res1ID;
  uint16_t res2ID;
};

TEST_F(ExpressionStorageTests, expressionParsing) {
  uint16_t resID;
  const char *test1Input =
      "((not unclean_restart) and ((2.00 after burnout) and ((not apogee) and "
      "((world_vel_z > 30.00) and ((angle_vertical < 40.00) and ((pos_z_agl > "
      "150.00) and (not (ever (launch and (angle_vertical > 45.00))))))))))";
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

TEST_F(ExpressionStorageTests, expressionEvaluation) {
  eventManager_setEventIncomplete(Event_e::launch);
  ASSERT_EQ(expressionStore.getNextExpressionSpot(0), 0);

  const char *test1Input = "(angle_vertical < 30)";

  const char *test2Input = "(not (ever (launch and (angle_vertical > 45))))";

  StringSlice slice1 = StringSlice(&test1Input, 0, strlen(test1Input));
  StringSlice slice2 = StringSlice(&test2Input, 0, strlen(test2Input));
  ExpressionValueType_e valueType =
      expressionStore.parseForTrigger(&res1ID, 0, slice1, 0);
  ASSERT_EQ(valueType, ExpressionValueType_e::boolean);
  valueType = expressionStore.parseForTrigger(&res2ID, 1, slice2, 0);
  ASSERT_EQ(valueType, ExpressionValueType_e::boolean);
  expressionStore.writeNewConfigs();
  filterData.angle_vertical = 10;
  expressionStore.tick(&filterData);
  ASSERT_TRUE(expressionStore.getExprBoolValue(res1ID));
  ASSERT_TRUE(expressionStore.getExprBoolValue(res2ID));
  filterData.angle_vertical = 45;
  expressionStore.tick(&filterData);
  ASSERT_FALSE(expressionStore.getExprBoolValue(res1ID));
  ASSERT_TRUE(expressionStore.getExprBoolValue(res2ID));
  eventManager_setEventComplete(Event_e::launch);
  filterData.angle_vertical = 40;
  expressionStore.tick(&filterData);
  ASSERT_FALSE(expressionStore.getExprBoolValue(res1ID));
  ASSERT_TRUE(expressionStore.getExprBoolValue(res2ID));
  filterData.angle_vertical = 46;
  expressionStore.tick(&filterData);
  ASSERT_FALSE(expressionStore.getExprBoolValue(res1ID));
  ASSERT_FALSE(expressionStore.getExprBoolValue(res2ID));
  filterData.angle_vertical = 10;
  expressionStore.tick(&filterData);
  ASSERT_TRUE(expressionStore.getExprBoolValue(res1ID));
  ASSERT_FALSE(expressionStore.getExprBoolValue(res2ID));
}

TEST_F(ExpressionStorageTests, chainedAndOr) {
  ASSERT_EQ(expressionStore.getNextExpressionSpot(0), 0);

  const char *test1Input = "((3 == (3 / 1)) and (not (3 > 4)))";

  const char *test2Input =
      "((1 == 1) and ((1 == 1) and ((not (not (1 == 1))) or (not ((1 == 2) or "
      "(3 > 4))))))";

  StringSlice slice1 = StringSlice(&test1Input, 0, strlen(test1Input));
  StringSlice slice2 = StringSlice(&test2Input, 0, strlen(test2Input));
  ExpressionValueType_e valueType =
      expressionStore.parseForTrigger(&res1ID, 0, slice1, 0);
  ASSERT_EQ(valueType, ExpressionValueType_e::boolean);
  valueType = expressionStore.parseForTrigger(&res2ID, 1, slice2, 0);
  ASSERT_EQ(valueType, ExpressionValueType_e::boolean);
  expressionStore.writeNewConfigs();
  expressionStore.tick(&filterData);
  ASSERT_TRUE(expressionStore.getExprBoolValue(res1ID));
  ASSERT_TRUE(expressionStore.getExprBoolValue(res2ID));
}
