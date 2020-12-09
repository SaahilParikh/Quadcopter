#include "pid.h"


typedef struct {
	float kP;
	float kI;
	float kD;
}	Controller_Constants;

typedef struct {
	float error = 0;
	float target;
	
	float integral = 0;
	float integral_bound;

	float derivitive = 0;
	float prev_error = 0;

	float time_step;

	Controller_Constants constants;
}	PID_Controller;



float step(PID_Controller *controller, float current) {
	controller->error = controller->target - current;

	float temp_integral = controller->integral + controller.error * controller.time_step;
	controller->integral = abs(temp_integral) < controller->integral_bound ? temp_integral : controller->integral_bound;

	controller->derivitive = (controller->prev_error - controller->error) / controller->time_step;

	return controller->constants.kP * controller->error + controller->constants.kI + controller->integral + controller->constants.kD * controller->derivitive;
}

void init_controller(PID_Controller *controller, Controller_Constants kConst, float dt, float integral_max) {
	controller->integral_bound = integral_max;
	controller->time_step = dt;
	controller->constants = kConst; 
	return;
}

void update_target(PID_Controller *controller, float new_target) {
	controller->target = new_target;
}