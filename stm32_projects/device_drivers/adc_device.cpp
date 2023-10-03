/*
 * adc_device.c
 */

#include "adc_device.h"

#if HAS_DEV(STM_HADC)

#include "hal_callbacks.h"

#define MAX_RAW_VAL_16B 1 << 16
#define MAX_RAW_VAL_14B 1 << 14
#define MAX_RAW_VAL_12B 1 << 12
#define MAX_RAW_VAL_10B 1 << 10
#define MAX_RAW_VAL_8B 1 << 8

static void adcDev_conversionCpltCallback(void *adc) {
  (static_cast<AdcDevCtrl_s *>(adc))->bConvCplt = true;
}

bool adcDev_init(AdcDevCtrl_s *adc, ADC_HandleTypeDef *hadc,
                 bool bSingleEnded) {
  // Set struct values
  adc->hadc = hadc;
  adc->bConvCplt = false;  // No conversion has occurred, so the value in the
                           // register would initially be random

  switch (hadc->Init.Resolution) {
#ifdef ADC_RESOLUTION_16B
    case ADC_RESOLUTION_16B:
      adc->maxRawVal = MAX_RAW_VAL_16B;
      break;
#endif  // ADC_RESOLUTION_16B
#ifdef ADC_RESOLUTION_14B
    case ADC_RESOLUTION_14B:
      adc->maxRawVal = MAX_RAW_VAL_14B;
      break;
#endif  // ADC_RESOLUTION_14B

    case ADC_RESOLUTION_12B:
      adc->maxRawVal = MAX_RAW_VAL_12B;
      break;

    case ADC_RESOLUTION_10B:
      adc->maxRawVal = MAX_RAW_VAL_10B;
      break;

    case ADC_RESOLUTION_8B:
      adc->maxRawVal = MAX_RAW_VAL_8B;
      break;

    default:
      adc->maxRawVal = 0;
      break;
  }

  // Make sure ADC is actually idle
  HAL_ADC_Stop(hadc);

  // Run ADC calibration. Don't care about result because there's no critical
  // harm if it fails
  uint32_t singleDiff =
      bSingleEnded ? ADC_SINGLE_ENDED : ADC_DIFFERENTIAL_ENDED;
#ifdef STM32L4
  HAL_ADCEx_Calibration_Start(hadc, singleDiff);
#else
  HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, singleDiff);
#endif  // STM32L4

  return true;
}

void adcDev_convertAllChannels(AdcDevCtrl_s *adc) {
  // Register callback for completed adc conversion (will overwrite any existing
  // callback/data for existing ADC handle)
  halCallbacks_registerAdcConvCpltCallback(adc->hadc,
                                           adcDev_conversionCpltCallback, adc);

  adc->bConvCplt = false;

  // Start ADC conversion with DMA.
  // Don't care about result because failure will appear in inability to get a
  // value.
  HAL_ADC_Start_DMA(adc->hadc, adc->rawVals, adc->hadc->Init.NbrOfConversion);
}

bool adcDev_getValue(AdcDevCtrl_s *adc, uint8_t rank, float *pval, float minVal,
                     float maxVal, uint32_t timeoutMS) {
  // Wait for something to happen
  uint32_t startTime = HAL_GetTick();
  while (!adc->bConvCplt && HAL_GetTick() - startTime < timeoutMS) {
  }

  // Get ADC value if conversion complete
  if (adc->bConvCplt) {
    float offset = minVal;
    float scaler = adc->maxRawVal / (maxVal - minVal);

    // Get value from buffer and convert into meaningful number
    *pval = (static_cast<float>(adc->rawVals[rank - 1])) / scaler + offset;

    // Stop interrupt. If this fails, don't care because stopping is just best
    // practice, not critical.
    HAL_ADC_Stop_DMA(adc->hadc);

    return true;
  }

  // If conversion isn't complete, the value in the register would be wrong, so
  // return false
  return false;
}

#endif  // HAS_DEV(VBAT_STM_ADC) || HAS_DEV(PYRO_CONT_ADC)
