#include "nrfx_saadc.h"

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

#define ADC_BITS  4095
#define ADC_VOLTAGE_RANGE 3.6

#define VX_0  1.465
#define VY_0  1.454
#define VZ_0  1.4845

#define VX_D  0.397
#define VY_D  0.401
#define VZ_D  0.4045

typedef struct {
	float theta;
	float psi;
	float phi;
}	angles;

nrf_saadc_value_t sample_value (uint8_t channel);
void get_angles(angles *angle_struct);
void init_accel(void);