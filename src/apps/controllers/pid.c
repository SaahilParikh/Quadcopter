#include "pid.h"
#include <math.h>
#include <stdio.h>


float step(PID_Controller* controller, float current) {
	controller->error = controller->target - current;
	printf("%f", controller->error);

	float temp_integral = controller->integral + controller->error * controller->time_step;
	controller->integral = fabs(temp_integral) < controller->integral_bound ? temp_integral : controller->integral_bound;

	controller->derivitive = (controller->prev_error - controller->error) / controller->time_step;
	controller->prev_error = controller->error;

	return controller->constants.kP * controller->error + controller->constants.kI * controller->integral + controller->constants.kD * controller->derivitive;
}

void init_controller(PID_Controller* controller, Controller_Constants* kConst, float dt, float integral_max) {
	controller->integral_bound = integral_max;
	controller->time_step = dt;
	controller->constants = *kConst; 
	controller->error = 0;
	controller->prev_error = 0;
	controller->derivitive = 0;
	controller->integral = 0;
	return;
}

void update_target(PID_Controller* controller, float new_target) {
	controller->target = new_target;
	return;
}


