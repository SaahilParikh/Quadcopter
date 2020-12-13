#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "display.h"

#include "simple_ble.h"
#include "buckler.h"

#include "max44009.h"

#include "buttons.h"

extern int32_t button_state;
void init_ble(void);

