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
#include <math.h>;

//I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
const uint16_t arming_pwm[] = {100, 100, 100, 100};
const uint16_t kill_pwm[] = {0, 0, 0, 0};

const float stablize_targets[] = {0.0, 0.0, 0.0};

#define W	0.5
#define KROLL	1
#define KPITCH	1
#define KYAW	1



#define	PWM_TO_FORCE(pwm)	((1.774487006 * pwm) + 12.12438228)
#define	FORCE_TO_PWM(force)	((0.5617907179 * force) + -6.707191437)

typedef struct {
	float RF;
	float RB;
	float LB;
	float LF;
}	motor_pwm_t;

typedef struct {
	float pitch;
	float roll;
	float yaw;
}	angles_t;


PID_Controller pitch_displacement;
PID_Controller roll_displacement;
PID_Controller yaw_displacement;

PID_Controller pitch_velocity;
PID_Controller roll_velocity;
PID_Controller yaw_velocity;

// Controller_Constants ang_d_stab_pitch[] = {3.2, 0.4, 0.0};
// Controller_Constants ang_d_stab_roll[] = {2.9, 0.000, 0.0};
// Controller_Constants ang_d_stab_yaw[] = {2.3, 0.000, 0.0};

// Controller_Constants ang_v_stab_pitch[] = {1.5, 0.000, 0.000};
// Controller_Constants ang_v_stab_roll[] = {1.0, 0.000, 0.000};
// Controller_Constants ang_v_stab_yaw[] = {1.0, 0.00, 0.0};


// Controller_Constants ang_d_stab_pitch[] = {3.2, 0.0, 0.0};
// Controller_Constants ang_d_stab_roll[] = {3.2, 0.000, 0.0};
// Controller_Constants ang_d_stab_yaw[] = {2.3, 0.000, 0.0};

// Controller_Constants ang_v_stab_pitch[] = {2.2, 0.0, 0.000};
// Controller_Constants ang_v_stab_roll[] = {2.2, 0.00, 0.000};
// Controller_Constants ang_v_stab_yaw[] = {1.0, 0.00, 0.0};

Controller_Constants ang_d_stab_pitch = {2.3, 0.0, 0.0};
Controller_Constants ang_d_stab_roll = {2.3, 0.000, 0.0};
Controller_Constants ang_d_stab_yaw = {1.4, 0.000, 0.0};

Controller_Constants ang_v_stab_pitch = {1.4, 0.0, 0.000};
Controller_Constants ang_v_stab_roll = {1.4, 0.00, 0.000};
Controller_Constants ang_v_stab_yaw = {1.0, 0.00, 0.0};


uint16_t current_pwm[] = {0, 0, 0, 0};

void get_target_pwm(motor_pwm_t* new_pwm, angles_t* error_v, angles_t* error_d){
	float roll = KROLL*(error_d->roll + error_v->roll);
	float pitch = KPITCH*(error_d->pitch + error_v->pitch);
	float yaw = KYAW*(error_d->yaw + error_v->yaw);
	new_pwm->RF =	-roll	-	pitch 	+	yaw;
	new_pwm->RB =	-roll 	+	pitch 	-	yaw;
	new_pwm->LB =	roll 	+	pitch 	+	yaw;
	new_pwm->LF =	roll	-	pitch 	-	yaw;
	return;
}

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

	current_orient.gyro_v = &gyro_v;
	current_orient.gyro_d = &gyro_d;
	current_orient.mag_d = &mag_d;
	current_orient.accel_d = &accel_d;

	printf("Sensors initialized\n");
}



void init_flight_controller() {
	init_controller(&pitch_displacement, &ang_d_stab_pitch, 0.004, 50);
	update_target(&pitch_displacement, 0);

	
	init_controller(&roll_displacement, &ang_d_stab_roll, 0.004, 30);
	update_target(&roll_displacement, 0);

	
	init_controller(&yaw_displacement, &ang_d_stab_yaw, 0.004, 30);
	update_target(&yaw_displacement, 0);

	
	init_controller(&pitch_velocity, &ang_v_stab_pitch, 0.004, 30);
	update_target(&pitch_velocity, 0);

	
	init_controller(&roll_velocity, &ang_v_stab_roll, 0.004, 20);
	update_target(&roll_velocity, 0);

	
	init_controller(&yaw_velocity, &ang_v_stab_yaw, 0.004, 10);
	update_target(&yaw_velocity, 0);
  
  printf("Flight controller initialized!\n");
	return;
}

void spam_pwm(const uint16_t spam[4]) {
	for(int i = 0; i < 50; i++){
	    pwm_update_duty_cycle(spam);
	    nrf_delay_ms(2);
  	}
  	current_pwm[0] = spam[0];
  	current_pwm[1] = spam[1];
  	current_pwm[2] = spam[2];
  	current_pwm[3] = spam[3];
  	return;
}

void arm() {
	//start sensors
	init_sensors();


	//start pwm
	//NRF_CLOCK->TASKS_HFCLKSTART = 1; 

	// Wait for clock to start
	//while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

	pwm_init();

	lsm9ds1_start_gyro_integration();
	spam_pwm(arming_pwm);
  printf("Armed\n");
	return;
}

void kill() {
	lsm9ds1_stop_gyro_integration();
	spam_pwm(kill_pwm);
  printf("Killed\n");
	return;
}

void update_ang_d(const float ang_d[3]) {
	update_target(&pitch_displacement, ang_d[0]);
	update_target(&roll_displacement, ang_d[1]);
	update_target(&yaw_displacement, ang_d[2]);
	return;
}

void get_u_ang_v(float u_ang_v[3], float u_ang_d[3]) {
	update_target(&pitch_velocity, (u_ang_d[0] = step(&pitch_displacement, gyro_d.x_axis)));
	update_target(&roll_velocity, (u_ang_d[1] = step(&roll_displacement, gyro_d.y_axis)));
	update_target(&yaw_velocity, (u_ang_d[2] = step(&yaw_displacement, gyro_d.z_axis)));

	u_ang_v[0] = step(&pitch_velocity, gyro_v.x_axis);
	u_ang_v[1] = step(&roll_velocity, gyro_v.y_axis);
	u_ang_v[2] = step(&yaw_velocity, gyro_v.z_axis);

	return;
}

void stablize(){
	update_ang_d(stablize_targets);
}


#define	PWM_MIN	115
#define	PWM_MAX	270

void fly() {
	float u_ang_v[3];
	float u_ang_d[3];
	float output_pwm[4];

	get_orientation(&current_orient);
	get_u_ang_v(u_ang_v, u_ang_d);
	get_target_pwm(output_pwm, u_ang_v, u_ang_d);
	get_include_z_pwm(output_pwm);

	current_pwm[0] = ((uint16_t) round(output_pwm[0]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[0]) + PWM_MIN;
	current_pwm[1] = ((uint16_t) round(output_pwm[1]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[1]) + PWM_MIN;
	current_pwm[2] = ((uint16_t) round(output_pwm[2]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[2]) + PWM_MIN;
	current_pwm[3] = ((uint16_t) round(output_pwm[3]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[3]) + PWM_MIN;

	//  printf("gd(%f, %f, %f)\t", gyro_d.x_axis, gyro_d.y_axis, gyro_d.z_axis);
	// // // printf("gd(%f)\t", pitch_velocity.integral);
	// // // //printf("gd(%f, %f, %f, %f)\t", output_pwm[3], output_pwm[2], output_pwm[1], output_pwm[0]);
	// // // // printf("gv(%f, %f, %f)\t", gyro_v.x_axis, gyro_v.y_axis, gyro_v.z_axis);
	//   printf("a(%f, %f, %f)\t", accel_d.theta, accel_d.psi, accel_d.phi);
	// // // printf("m(%f, %f, %f)\t", mag_d.roll, mag_d.pitch, mag_d.yaw);
	//  printf("d(%u, %u, %u, %u)\n", current_pwm[3], current_pwm[2], current_pwm[1], current_pwm[0]);


	pwm_update_duty_cycle(current_pwm);
	return;
}

void fly_w_o_sensorget() {
	float u_ang_v[3];
	float u_ang_d[3];
	float output_pwm[4];

	get_u_ang_v(u_ang_v, u_ang_d);
	get_target_pwm(output_pwm, u_ang_v, u_ang_d);
	get_include_z_pwm(output_pwm);

	current_pwm[0] = ((uint16_t) round(output_pwm[0]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[0]) + PWM_MIN;
	current_pwm[1] = ((uint16_t) round(output_pwm[1]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[1]) + PWM_MIN;
	current_pwm[2] = ((uint16_t) round(output_pwm[2]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[2]) + PWM_MIN;
	current_pwm[3] = ((uint16_t) round(output_pwm[3]) + PWM_MIN) > PWM_MAX ? PWM_MAX : (uint16_t) round(output_pwm[3]) + PWM_MIN;

	//  printf("gd(%f, %f, %f)\t", gyro_d.x_axis, gyro_d.y_axis, gyro_d.z_axis);
	// // // printf("gd(%f)\t", pitch_velocity.integral);
	// // // //printf("gd(%f, %f, %f, %f)\t", output_pwm[3], output_pwm[2], output_pwm[1], output_pwm[0]);
	// // // // printf("gv(%f, %f, %f)\t", gyro_v.x_axis, gyro_v.y_axis, gyro_v.z_axis);
	//   printf("a(%f, %f, %f)\t", accel_d.theta, accel_d.psi, accel_d.phi);
	// // // printf("m(%f, %f, %f)\t", mag_d.roll, mag_d.pitch, mag_d.yaw);
	//  printf("d(%u, %u, %u, %u)\n", current_pwm[3], current_pwm[2], current_pwm[1], current_pwm[0]);


	pwm_update_duty_cycle(current_pwm);
}

void get_include_z_pwm(motor_pwm_t* add_z) {
	float rf_f = PWM_TO_FORCE(add_z->RF);
	float lf_f = PWM_TO_FORCE(add_z->LF);
	float rb_f = PWM_TO_FORCE(add_z->RB);
	float lb_f = PWM_TO_FORCE(add_z->LB);

	float total_thrust_curr = sqrt(rf_f*rf_f + lf_f*lf_f + lb_f*lb_f + rb_f*rb_f);

	float thrust_needed_in_z = COPTER_MASS / (cos(gyro_d.x_axis/57.29577) * cos(gyro_d.y_axis/57.29577));
	float thrust_left_per_motor = (thrust_needed_in_z - total_thrust_curr)/4;
	float pwm_needed_per_motor = FORCE_TO_PWM(thrust_left_per_motor);
	add_z->RF += pwm_needed_per_motor;
	add_z->RB += pwm_needed_per_motor;
	add_z->LF += pwm_needed_per_motor;
	add_z->LB += pwm_needed_per_motor;

	return;
}




