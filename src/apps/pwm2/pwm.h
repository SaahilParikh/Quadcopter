#include "app_pwm.h"

void pwm_ready_callback(uint32_t pwm_id);
void pwm_init_one(app_pwm_t const * const p_instance, uint32_t pin, app_pwm_callback_t p_ready_callback);
void pwm_init_two(app_pwm_t const * const p_instance, uint32_t pin1, uint32_t pin2, app_pwm_callback_t p_ready_callback);
ret_code_t update_pwm(app_pwm_t const * const p_instance, uint8_t channel, app_pwm_duty_t pwm_duty);