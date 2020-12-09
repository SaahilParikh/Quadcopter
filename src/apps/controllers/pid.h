

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

float step(PID_Controller *controller, float current);
void init_controller(PID_Controller *controller, Controller_Constants kConst, float dt, float integral_max);
void update_target(PID_Controller *controller, float new_target);