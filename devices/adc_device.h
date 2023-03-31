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

#if (HAS_DEV(STM_HADC))

typedef struct {
  ADC_HandleTypeDef *hadc;
  bool bConvCplt;
  uint32_t rawVals[MAX_CHANNELS];
  float maxRawVal;
} AdcDevCtrl_s;

/**
 * Initializes/calibrates ADC.
 * @param minVal: correlates to raw 0
 * @param maxVal: correlates to highest raw value of ADC's resolution
 */
bool adcDev_init(AdcDevCtrl_s *adc, ADC_HandleTypeDef *hadc,
                 bool bSingleEnded);

/**
 * Starts a one-time ADC read/conversion on all channels currently configured
 * @param pOutBuffer: Output array. Must have same length of hadc.Init.NbrOfConversions
 */
void adcDev_convertAllChannels(AdcDevCtrl_s *adc);

/**
 * Returns true with converted ADC value if available, otherwise returns false
 * (hardware conversion hasn't completed)
 * @param rank: The rank of the ADC channel to retrieve the value orf
 */
bool adcDev_getValue(AdcDevCtrl_s *adc, uint8_t rank, float *pval, float minValue, float maxValue, uint32_t timeoutMS);

#endif

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_ADC_DEVICE_H_
