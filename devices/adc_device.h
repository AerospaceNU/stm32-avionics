/**
 * adc_device.h
 */

#ifndef DEVICES_ADC_DEVICE_H_
#define DEVICES_ADC_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "board_config_common.h"

#define MAX_CHANNELS 14

typedef struct {
  ADC_HandleTypeDef *hadc;
  uint8_t rank;
  float scaler;
  float offset;
  bool bConvCplt;
  uint32_t rawVals[MAX_CHANNELS];
} AdcDevCtrl_s;

/**
 * Initializes/calibrates ADC.
 * @param rank: the sequence number the channel is collected in for given ADC
 * handle (1-based index for consistency with STM32Cube)
 * @param minVal: correlates to raw 0
 * @param maxVal: correlates to highest raw value of ADC's resolution
 */
void adcInit(AdcDevCtrl_s *adc, ADC_HandleTypeDef *hadc, uint8_t rank,
             float minVal, float maxVal, bool bSingleEnded);

/**
 * Starts a one-time ADC read/conversion. This type of conversion takes time, so
 * starting read gives flexibility for higher code to perform other tasks while
 * the read/conversion is occurring
 */
void adcStartSingleRead(AdcDevCtrl_s *adc);

/**
 * Returns true with converted ADC value if available, otherwise returns false
 * (hardware conversion hasn't completed)
 */
bool adcGetValue(AdcDevCtrl_s *adc, float *pval, uint32_t timeoutMS);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_ADC_DEVICE_H_
