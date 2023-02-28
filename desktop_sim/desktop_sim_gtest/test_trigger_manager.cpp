#include <gtest/gtest.h>

#include "cli.h"
#include "desktop_hardware_manager.h"
#include "sim_hardware_manager.h"
#include "trigger_manager.h"

bool testPyrosFired[NUM_PYRO] = {0};

class TriggerHardwareManager : public SimHardwareManager {
  void hm_pyroFire(int pyroId, uint32_t duration) override {
    printf("PYRO %i FIRED\n", pyroId);
    testPyrosFired[pyroId] = true;
  }
  void hm_pyroSet(int pyroId, bool enable) override { printf("PYRO SET\n"); }
  void hm_pyroSetPwm(int pyroId, uint32_t frequency, uint32_t pulseWidth,
                     uint32_t duration) override {
    printf("PWM SET\n");
  }
};

TEST(TriggerManager, setStringConfigs) {
  for (int i = 0; i < NUM_PYRO; i++) testPyrosFired[i] = false;

  const int PYRO_PORT = 0;

  auto hm = std::make_shared<TriggerHardwareManager>();
  hm_sim_setHM(hm);

  cli_init();
  cli_setDefaultConfig();
  for (int i = 0; i < MAX_TRIGGER; i++) {
    (cli_getConfigs()->triggerConfiguration + i)->mode = TRIGGER_TYPE_EMPTY;
  }

  triggerManager_init();
  
  const char* apogee = "apogee";
  const char* main = "(apogee and (pos_z_agl < 200))";
  triggerManager_setTriggerConfig(0, &apogee);
  triggerManager_setTriggerConfig(1, &main);

  FilterData_s filterData = {0};
  triggerManager_update(&filterData);

  EXPECT_FALSE(testPyrosFired[PYRO_PORT]);
  EXPECT_FALSE((triggerManager_status() >> PYRO_PORT) & 0b1);

  triggerManager_triggerFire(0, true);

  EXPECT_TRUE(testPyrosFired[PYRO_PORT]);
  EXPECT_TRUE((triggerManager_status() >> PYRO_PORT) & 0b1);
}


TEST(TriggerManager, fire) {
  for (int i = 0; i < NUM_PYRO; i++) testPyrosFired[i] = false;

  const int PYRO_PORT = 0;

  auto hm = std::make_shared<TriggerHardwareManager>();
  hm_sim_setHM(hm);

  cli_init();
  cli_setDefaultConfig();
  for (int i = 0; i < MAX_TRIGGER; i++) {
    (cli_getConfigs()->triggerConfiguration + i)->mode = TRIGGER_TYPE_EMPTY;
  }
  
  // Set up port 0 as a pyro
  (cli_getConfigs()->triggerConfiguration + 0)->mode = TRIGGER_TYPE_PYRO;
  (cli_getConfigs()->triggerConfiguration + 0)->port = PYRO_PORT;
  (cli_getConfigs()->triggerConfiguration + 0)->duration = 1;

  triggerManager_init();

  FilterData_s filterData = {0};
  triggerManager_update(&filterData);

  EXPECT_FALSE(testPyrosFired[PYRO_PORT]);
  EXPECT_FALSE((triggerManager_status() >> PYRO_PORT) & 0b1);

  triggerManager_triggerFire(0, true);

  EXPECT_TRUE(testPyrosFired[PYRO_PORT]);
  EXPECT_TRUE((triggerManager_status() >> PYRO_PORT) & 0b1);
}
