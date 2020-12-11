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


  init_sensors();
  orientation_data orient;

  lsm9ds1_measurement_t gyro_v;
  accel_angles_t accel_d;
  mag_angles_t mag_d;
  lsm9ds1_measurement_t gyro_d;

  orient.gyro_v = &gyro_v;
  orient.gyro_d = &gyro_d;
  orient.mag_d = &mag_d;
  orient.accel_d = &accel_d;


  // loop forever
  while (1) {
    get_orientation(&orient);
    
    printf("d(%f, %f, %f)\t", gyro_v.x_axis, orient.gyro_v->y_axis, orient.accel_d->theta);
    //printf("v(%f, %f, %f)\n", step(&roll_velocity, anglev.x_axis), step(&pitch_velocity, anglev.y_axis), step(&yaw_velocity, anglev.z_axis));
    nrf_delay_ms(200);
    }
    
  
}
