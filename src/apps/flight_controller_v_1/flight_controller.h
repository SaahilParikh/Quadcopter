#pragma once
#include "nrf_twi_mngr.h"

#define pwm_update_rate 2

void init_sensors(void);
void init_flight_controller(void);
void arm(void);