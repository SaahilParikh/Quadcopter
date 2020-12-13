#pragma once
#include "nrfx_saadc.h"

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

#define ADC_BITS  4095
#define ADC_VOLTAGE_RANGE 3.6

#define VX_0  1.457057
#define VY_0  1.464284
#define VZ_0  1.468276

#define VX_D  0.398242
#define VY_D  0.401594
#define VZ_D  0.410853

#define GX_0  0.013674
#define GY_0  -0.004232
#define GZ_0  0.011011

typedef struct {
	float theta;
	float psi;
	float phi;
}	accel_angles_t;

nrf_saadc_value_t sample_value (uint8_t channel);
void get_accel_angles(accel_angles_t *angle_struct);
void init_accel(void);

void calibrate_accel2();