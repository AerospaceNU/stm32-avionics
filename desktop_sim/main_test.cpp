#include <data_log.h>
#include <desktop_hardware_manager.h>
#include <minmea.h>
#include <stdint.h>
#include <stdio.h>

#include <cstring>

void retrieveSector(uint32_t mscAddress, uint32_t lengthBytes,
                    uint8_t *pCluster);
void mapFlashToClusters();

#define FILE_LEN 0x1e848000

int main() {
  output_file = "";
  ext_flash_path = "ext_flash_test.hex";
  int_flash_path = "int_flash_test.hex";

  hm_hardwareInit();

  // create some dummy files

  uint64_t timestamp;
  {
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));

    const struct minmea_date date = {10, 12, 22};
    const struct minmea_time time = {12, 10, 11, 0};

    if (!minmea_gettime(&ts, &date, &time)) timestamp = ts.tv_sec;
  }

  dataLog_init();
  dataLog_assignFlight();
  dataLog_getFlightMetadata()->pressureRef = filter_getPressureRef();
  dataLog_getFlightMetadata()->gravityRef = filter_getGravityRef();
  dataLog_getFlightMetadata()->launchedCliConfigs = *cli_getConfigs();
  dataLog_getFlightMetadata()->launched = 1;
  dataLog_getFlightMetadata()->gpsTimestamp = timestamp;
  dataLog_writeFlightMetadata();

  for (int i = 0; i < 100; i++) {
    SensorData_s sensorData = {0};
    sensorData.barometerData->pressureAtm = i / 100.0;
    FilterData_s filterData = {0};
    filterData.pos_z_agl = i;
    sensorData.timestampMs = i * 60;
    dataLog_write(&sensorData, &filterData, 1);
  }

  // Create a 2nd flight in order to end the first
  dataLog_assignFlight();

  mapFlashToClusters();

  FILE *pFile;
  pFile = fopen("fat32_test.img", "wb");  // w for write, b for binary

  // seek to the start of the file (to be sure)
  fseek(pFile, 0, SEEK_SET);

  // Retrieve every cluster
  uint8_t buf[512];
  for (size_t i = 0; i < FILE_LEN; i += 512) {
    retrieveSector(i, sizeof(buf), buf);

    // printf("Writing cluster %i...", i / 512);
    fwrite(buf, sizeof(buf), 1, pFile);
    // printf("done\n");
  }

  printf("All done!\n");
  fclose(pFile);
}
