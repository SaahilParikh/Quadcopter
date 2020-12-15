#pragma once
#include "nrf_drv_pwm.h"

void pwm_update_duty_cycle(const uint16_t duty_cycle[4]);
void pwm_init(void);
