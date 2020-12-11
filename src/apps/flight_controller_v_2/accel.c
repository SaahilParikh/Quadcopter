#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrfx_saadc.h"

#include "buckler.h"
#include "accel.h"
#include "MedianFilter.h"


#define NUM_ELEMENTS    7

static sMedianFilter_t medianFilter;
static sMedianNode_t medianBuffer[NUM_ELEMENTS];


// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}


// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_accel_value (uint8_t channel) {
	nrf_saadc_value_t val;
	for(int i = 0; i < NUM_ELEMENTS; i++) {
		ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
		APP_ERROR_CHECK(error_code);
		val = MEDIANFILTER_Insert(&medianFilter, (float) val);
	}
	return val;
}



void get_accel_angles(accel_angles_t* angle_struct) {
    float gx = ((float)sample_accel_value(X_CHANNEL)*(ADC_VOLTAGE_RANGE/ADC_BITS) - VX_0)/VX_D;
    float gy = ((float)sample_accel_value(Y_CHANNEL)*(ADC_VOLTAGE_RANGE/ADC_BITS) - VY_0)/VY_D;
    float gz = ((float)sample_accel_value(Z_CHANNEL)*(ADC_VOLTAGE_RANGE/ADC_BITS) - VZ_0)/VZ_D;

    angle_struct->theta = -57.29577*atan2(gx, sqrt(pow(gz, 2) + pow(gy, 2)));
    angle_struct->psi = 57.29577*atan2(gy, sqrt(pow(gz, 2) + pow(gx, 2)));
    angle_struct->phi = 57.29577*atan2(sqrt(pow(gx, 2) + pow(gy, 2)), gz);
    
    return;
} 


void init_accel(void){
	ret_code_t error_code = NRF_SUCCESS;

	//Create the median filter
	medianFilter.numNodes = NUM_ELEMENTS;
	medianFilter.medianBuffer = medianBuffer;

	MEDIANFILTER_Init(&medianFilter);

	// initialize analog to digital converter
	nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
	saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
	error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
	APP_ERROR_CHECK(error_code);

	// initialize analog inputs
	// configure with 0 as input pin for now
	nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
	channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
	channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

	// specify input pin and initialize that ADC channel
	channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
	error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
	APP_ERROR_CHECK(error_code);

	// specify input pin and initialize that ADC channel
	channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
	error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
	APP_ERROR_CHECK(error_code);

	// specify input pin and initialize that ADC channel
	channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
	error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
	APP_ERROR_CHECK(error_code);
}