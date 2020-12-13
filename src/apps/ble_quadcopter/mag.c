
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrfx_saadc.h"

#include "buckler.h"
#include "mag.h"
#include "MedianFilter.h"
#include "lsm9ds1.h"

#define NUM_ELEMENTS    7

static sMedianFilter_t medianFilterMagY;
static sMedianNode_t medianBufferMagY[NUM_ELEMENTS];

static sMedianFilter_t medianFilterMagX;
static sMedianNode_t medianBufferMagX[NUM_ELEMENTS];

static sMedianFilter_t medianFilterMagZ;
static sMedianNode_t medianBufferMagZ[NUM_ELEMENTS];

float initial_yaw;
float initial_pitch;
float initial_roll;


void init_mag(void) {
	//Create the median filter
	medianFilterMagX.numNodes = NUM_ELEMENTS;
	medianFilterMagX.medianBuffer = medianBufferMagX;

	MEDIANFILTER_Init(&medianFilterMagX);

	//Create the median filter
	medianFilterMagY.numNodes = NUM_ELEMENTS;
	medianFilterMagY.medianBuffer = medianBufferMagY;

	MEDIANFILTER_Init(&medianFilterMagY);

	//Create the median filter
	medianFilterMagZ.numNodes = NUM_ELEMENTS;
	medianFilterMagZ.medianBuffer = medianBufferMagZ;

	MEDIANFILTER_Init(&medianFilterMagZ);

	float valx = 0;
	float valy = 0;
	float valz = 0;
	lsm9ds1_measurement_t init_mag;

	for(int i = 0; i < NUM_ELEMENTS; i++) {
		init_mag = lsm9ds1_read_magnetometer();
		valx = MEDIANFILTER_Insert(&medianFilterMagX, init_mag.x_axis);
		valy = MEDIANFILTER_Insert(&medianFilterMagY, init_mag.y_axis);
		valz = MEDIANFILTER_Insert(&medianFilterMagZ, init_mag.z_axis);
	}
	initial_yaw = atan2(valy, valy);
	initial_pitch = atan2(valx, sqrt(pow(valz, 2) + pow(valy, 2)));
	initial_roll = atan2(valy, sqrt(pow(valz, 2) + pow(valx, 2)));

	return;
}

void get_mag(mag_angles_t *struct_angles) {
	float valx = 0;
	float valy = 0;
	float valz = 0;
	lsm9ds1_measurement_t mag;

	for(int i = 0; i < NUM_ELEMENTS; i++) {
		mag = lsm9ds1_read_magnetometer();
		valx = MEDIANFILTER_Insert(&medianFilterMagY, mag.x_axis);
		valy = MEDIANFILTER_Insert(&medianFilterMagX, mag.y_axis);
		valz = MEDIANFILTER_Insert(&medianFilterMagX, mag.z_axis);
	}

	struct_angles->yaw = (initial_yaw - atan2(valx, valy)) * 57.29577;
	struct_angles->pitch = -(initial_pitch - atan2(valx, sqrt(pow(valz, 2) + pow(valy, 2)))) * 57.29577;
	struct_angles->roll = (initial_roll - atan2(valy, sqrt(pow(valz, 2) + pow(valx, 2)))) * 57.29577;
	return;

}