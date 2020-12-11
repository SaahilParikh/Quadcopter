#include "pid.h"
#include "accel.h"
#include "mag.h"
#include "lsm9ds1.h"
#include "pwm.h"
#include "nrf_twi_mngr.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "flight_controller.h"

//I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
uint16_t arming_pwm[] = {100, 100, 100, 100};


PID_Controller pitch_displacement;
PID_Controller roll_displacement;
PID_Controller yaw_displacement;

PID_Controller pitch_velocity;
PID_Controller roll_velocity;
PID_Controller yaw_velocity;


Controller_Constants ang_d_stab_pitch[] = {1.0, 0.000, 0.0};
Controller_Constants ang_d_stab_roll[] = {1.0, 0.000, 0.0};
Controller_Constants ang_d_stab_yaw[] = {1.0, 0.000, 0.0};

Controller_Constants ang_v_stab_pitch[] = {1.0, 0.000, 0.0};
Controller_Constants ang_v_stab_roll[] = {1.0, 0.000, 0.0};
Controller_Constants ang_v_stab_yaw[] = {1.0, 0.000, 0.0};

uint16_t desired_pwm[] = {0, 0, 0, 0};

void init_sensors(){
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



void init_flight_controller() {
	
	init_controller(&pitch_displacement, ang_d_stab_pitch, 0.02, 100);
	update_target(&pitch_displacement, 0);

	
	init_controller(&roll_displacement, ang_d_stab_roll, 0.02, 100);
	update_target(&roll_displacement, 0);

	
	init_controller(&yaw_displacement, ang_d_stab_yaw, 0.02, 100);
	update_target(&yaw_displacement, 0);

	
	init_controller(&pitch_velocity, ang_v_stab_pitch, 0.02, 100);
	update_target(&pitch_velocity, 0);

	
	init_controller(&roll_velocity, ang_v_stab_roll, 0.02, 100);
	update_target(&roll_velocity, 0);

	
	init_controller(&yaw_velocity, ang_v_stab_yaw, 0.02, 100);
	update_target(&yaw_velocity, 0);

	//start sensors
	init_sensors();


	//start pwm
	NRF_CLOCK->TASKS_HFCLKSTART = 1; 

	// Wait for clock to start
	while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

	pwm_init();

}

void arm() {
	for(int i = 0; i < 50; i++){
	    pwm_update_duty_cycle(arming_pwm);
	    nrf_delay_ms(2);
  	}
}