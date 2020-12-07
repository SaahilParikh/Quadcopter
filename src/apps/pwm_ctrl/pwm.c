#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "app_pwm.h"
#include "nrf_twi_mngr.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "nrf_delay.h"

#include "buckler.h"


APP_PWM_INSTANCE(PWM1, 1);
void pwm_init(app_pwm_t const * const p_instance, app_pwm_callback_t p_ready_callback) {
  app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(20000, BUCKLER_LED0);
  int err_code = app_pwm_init(p_instance, &pwm1_cfg, p_ready_callback);

  APP_ERROR_CHECK(err_code);

  app_pwm_enable(p_instance);
}

void updatepwm(app_pwm_t const * const p_instance, app_pwm_duty_t pwm_duty){
	if (!app_pwm_busy_check(p_instance)){
		app_pwm_channel_duty_set(p_instance, 0, pwm_duty);
	}
}