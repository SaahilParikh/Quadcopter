#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "app_pwm.h"
#include "nrf_twi_mngr.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "nrf_delay.h"

#include "app_pwm.h"

#include "buckler.h"

void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    printf("Duty Change");
}

void pwm_init_two(app_pwm_t const * const p_instance, uint32_t pin1, uint32_t pin2, app_pwm_callback_t p_ready_callback) {
  app_pwm_config_t pwm_cfg = APP_PWM_DEFAULT_CONFIG_2CH(20000, pin1, pin2);
  int err_code = app_pwm_init(p_instance, &pwm_cfg, p_ready_callback);

  APP_ERROR_CHECK(err_code);

  app_pwm_enable(p_instance);
}

void pwm_init_one(app_pwm_t const * const p_instance, uint32_t pin, app_pwm_callback_t p_ready_callback) {
  app_pwm_config_t pwm_cfg = APP_PWM_DEFAULT_CONFIG_1CH(20000, pin);
  int err_code = app_pwm_init(p_instance, &pwm_cfg, p_ready_callback);

  APP_ERROR_CHECK(err_code);

  app_pwm_enable(p_instance);
}


ret_code_t update_pwm(app_pwm_t const * const p_instance, uint8_t channel, app_pwm_duty_t pwm_duty){
  if (!app_pwm_busy_check(p_instance)){
    return app_pwm_channel_duty_set(p_instance, channel, pwm_duty);
  }
  return false;
}