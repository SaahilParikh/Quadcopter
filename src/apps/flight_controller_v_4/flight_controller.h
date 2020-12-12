#pragma once
#include "nrf_twi_mngr.h"

// SETUP FOR SENSORS
orientation_data current_orient;

lsm9ds1_measurement_t gyro_v;
accel_angles_t accel_d;
mag_angles_t mag_d;
lsm9ds1_measurement_t gyro_d;
// END OF SENSORS SET UP

#define pwm_update_rate 2

void init_sensors(void);
void init_flight_controller(void);
void arm(void);
void fly(void);
void kill(void);