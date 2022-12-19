#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include <desktop_hardware_manager.h>
#include <data_log.h>

void retrieveCluster(uint32_t mscAddress, uint32_t lengthBytes,
                     uint8_t *pCluster);
void mapFlashToClusters();

#define FILE_LEN 0x1e848000

int main() {
  output_file =  "";
  ext_flash_path = "ext_flash_test.hex";
  int_flash_path = "int_flash_test.hex";


  hm_hardwareInit();

  // create some dummy files
  dataLog_init();
  dataLog_assignFlight();
  dataLog_getFlightMetadata()->pressureRef = filter_getPressureRef();
  dataLog_getFlightMetadata()->gravityRef = filter_getGravityRef();
  dataLog_getFlightMetadata()->launchedCliConfigs = *cli_getConfigs();
  dataLog_getFlightMetadata()->launched = 1;
  dataLog_writeFlightMetadata();

  for (int i = 0; i < 100; i++) {
    SensorData_s sensorData = {0};
    sensorData.barometerData->pressureAtm = i / 100.0;
    FilterData_s filterData = {0};
    filterData.pos_z_agl = i;
    dataLog_write(&sensorData, &filterData, 1);
  }

  mapFlashToClusters();

  FILE *pFile;
  pFile = fopen("fat32_test.img", "wb");  // w for write, b for binary

  // seek to the start of the file (to be sure)
  fseek(pFile, 0, SEEK_SET);

  // Retrieve every cluster
  uint8_t buf[512];
  for (size_t i = 0; i < FILE_LEN; i += 512) {
    retrieveCluster(i, sizeof(buf), buf);

    fwrite(buf, sizeof(buf), 1, pFile);
  }
}
