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
#include "accel.h"
#include "lsm9ds1.h"
#include "flight_controller.h"
#include "nrf_twi_mngr.h"
#include "mag.h"


// //I2C manager
// NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);


int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  

  init_flight_controller();

  nrf_delay_ms(2000);

  arm();

  nrf_delay_ms(2000);

  for(int i = 0; i < 2000; i++){
     fly();
     nrf_delay_us(1);
   }

  kill();
  printf("(dead)\n" );

  nrf_delay_ms(1000);
    
  
}
