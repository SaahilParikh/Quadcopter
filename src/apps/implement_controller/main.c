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
#include "nrf_twi_mngr.h"
#include "mag.h"

//I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

void sensor_init(){
  init_accel();

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_400K;
  ret_code_t error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize LSM9DS1 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("lsm9ds1 initialized\n");

  init_mag();

  lsm9ds1_start_gyro_integration();
}


int main (void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  Controller_Constants stabilization[] = {1.0, 0.000, 0.0};
  PID_Controller pitch_displacement;
  init_controller(&pitch_displacement, stabilization, 0.02, 100);
  update_target(&pitch_displacement, 0);

  PID_Controller roll_displacement;
  init_controller(&roll_displacement, stabilization, 0.02, 100);
  update_target(&roll_displacement, 0);

  PID_Controller yaw_displacement;
  init_controller(&yaw_displacement, stabilization, 0.02, 100);
  update_target(&yaw_displacement, 0);


  PID_Controller pitch_velocity;
  init_controller(&pitch_velocity, stabilization, 0.02, 100);
  update_target(&pitch_velocity, 0);

  PID_Controller roll_velocity;
  init_controller(&roll_velocity, stabilization, 0.02, 100);
  update_target(&roll_velocity, 0);

  PID_Controller yaw_velocity;
  init_controller(&yaw_velocity, stabilization, 0.02, 100);
  update_target(&yaw_velocity, 0);


  sensor_init();
  lsm9ds1_measurement_t angle = lsm9ds1_read_gyro_integration();
  lsm9ds1_measurement_t anglev;
  // loop forever
  while (1) {
    angle = lsm9ds1_read_gyro_integration();
    anglev = lsm9ds1_read_gyro();
    
    printf("d(%f, %f, %f)\t", step(&roll_displacement, angle.x_axis), step(&pitch_displacement, angle.y_axis), step(&yaw_displacement, angle.z_axis));
    printf("v(%f, %f, %f)\n", step(&roll_velocity, anglev.x_axis), step(&pitch_velocity, anglev.y_axis), step(&yaw_velocity, anglev.z_axis));
    nrf_delay_ms(2000);
    }
    
  
}
