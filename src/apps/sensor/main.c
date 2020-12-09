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
#include "accel.h"
#include "lsm9ds1.h"
#include "nrf_twi_mngr.h"


#include "nrfx_gpiote.h"
#include "nrf_serial.h"
#include "mag.h"


//I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();


  
  init_accel();

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_400K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize LSM9DS1 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("lsm9ds1 initialized\n");

  init_mag();

  lsm9ds1_start_gyro_integration();
  lsm9ds1_measurement_t angle = lsm9ds1_read_gyro_integration();

  // initialization complete
  printf("Buckler initialized!\n");

  
  
  mag_angles magang;
  angles angleptr;


  // loop forever
  while (1) {
    // sample analog inputs
    angle = lsm9ds1_read_gyro_integration();
    
    get_angles(&angleptr);
    get_mag(&magang);
    

    printf("theta: %f\tpsi: %f\tphi: %f\n", angleptr.theta, angleptr.psi, angleptr.phi);
    printf("theta: %f\tphi: %f\tyaw: %f\n", angle.x_axis, angle.y_axis, angle.z_axis);
    printf("pitch: %f\tyroll: %f\tyaw: %f\n", magang.roll, magang.pitch, magang.yaw);
    nrf_delay_ms(20);
  }
}
