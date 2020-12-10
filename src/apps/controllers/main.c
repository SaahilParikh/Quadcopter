// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"

#include "buckler.h"
#include "pid.h"


int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  Controller_Constants normal_movement[] = {1.0, 0.000, 0.0};
  PID_Controller regular;
  init_controller(&regular, normal_movement, 0.02, 100);

  update_target(&regular, 100);


  // loop forever
  while (1) {

    for (int i = 0; i < 100; i++) {
      printf("(%d, %f),\t", i, step(&regular, (float)i));
      nrf_delay_ms(200);
    }
    
    
  }
}
