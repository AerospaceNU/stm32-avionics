/*
 * adc_device.c
 */

#include "adc_device.h"
#include "hal_callbacks.h"

static void adcConversionCpltCallback(void *adc) {
	((AdcCtrl_t *) adc)->bConvCplt = true;
}

void adcInit(AdcCtrl_t *adc, ADC_HandleTypeDef *hadc, uint8_t rank, float minVal, float maxVal, bool bSingleEnded) {

	// Set struct values
	adc->hadc = hadc;
	adc->bConvCplt = false; // No conversion has occurred, so the value in the register would initially be random
	adc->rank = rank;
	adc->offset = minVal;

	float maxRawVal = 0;
	switch(hadc->Init.Resolution) {
		case ADC_RESOLUTION_16B:
			maxRawVal = 65535; // 2 << 16
			break;

		case ADC_RESOLUTION_14B:
			maxRawVal = 16384; // 2 << 14
			break;

		case ADC_RESOLUTION_12B:
			maxRawVal = 4096; // 2 << 12
			break;

		case ADC_RESOLUTION_10B:
			maxRawVal = 1024; // 2 << 10
			break;

		case ADC_RESOLUTION_8B:
			maxRawVal = 256; // 2 << 8
			break;

		default:
			maxRawVal = 0;
			break;
	}
	adc->scaler = maxRawVal / (maxVal - minVal);

	// Run ADC calibration. Don't care about result because there's no critical harm if it fails
	uint32_t singleDiff = bSingleEnded ? ADC_SINGLE_ENDED : ADC_DIFFERENTIAL_ENDED;
	HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, singleDiff);
}

void adcStartSingleRead(AdcCtrl_t *adc) {

	// Register callback for completed adc conversion (will overwrite any existing callback/data for existing ADC handle)
	register_HAL_ADC_ConvCpltCallback(adc->hadc, adcConversionCpltCallback, adc);

	adc->bConvCplt = false;

	// Start ADC conversion with DMA.
	// Don't care about result because failure will appear in inability to get a value.
	HAL_ADC_Start_DMA(adc->hadc, adc->rawVals, adc->hadc->Init.NbrOfConversion);
}

bool adcGetValue(AdcCtrl_t *adc, float *pval, uint32_t timeoutMS) {

	// Wait for something to happen
	uint32_t startTime = HAL_GetTick();
	while (!adc->bConvCplt && HAL_GetTick() - startTime < timeoutMS);

	// Get ADC value if conversion complete
	if (adc->bConvCplt) {
		// Get value from buffer and convert into meaningful number
		*pval = adc->rawVals[adc->rank - 1] / adc->scaler + adc->offset;

		// Stop interrupt. If this fails, don't care because stopping is just best practice, not critical.
		HAL_ADC_Stop_DMA(adc->hadc);

		return true;
	}

	// If conversion isn't complete, the value in the register would be wrong, so return false
	return false;
}