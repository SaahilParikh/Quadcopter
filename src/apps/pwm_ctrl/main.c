// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "app_pwm.h"
#include "nrf_twi_mngr.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "display.h"
#include "nrf_delay.h"

#include "buckler.h"




void pwm_ready_callbackg(uint32_t pwm_id)    // PWM callback function
{
    
}

APP_PWM_INSTANCE(PWM31, 1);
void pwm_inist(app_pwm_t const * const p_instance, app_pwm_callback_t p_ready_callback) {
  app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(20000, BUCKLER_LED0);
  int err_code = app_pwm_init(p_instance, &pwm1_cfg, p_ready_callback);

  APP_ERROR_CHECK(err_code);

  app_pwm_enable(p_instance);
}

ret_code_t update_pwm(app_pwm_t const * const p_instance, uint8_t channel, app_pwm_duty_t pwm_duty){
  if (!app_pwm_busy_check(p_instance)){
    return app_pwm_channel_duty_set(p_instance, channel, pwm_duty);
  }
  return false;
}

int main(void) {

  // Initialize

  // initialize display
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };

  ret_code_t error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);
  display_init(&spi_instance);
  display_write("Quadcopter :)", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(BUCKLER_LED0);

  // // Setup BLE
  // simple_ble_app = simple_ble_init(&ble_config);

  // simple_ble_add_service(&pwm_service);

  // simple_ble_add_characteristic(1, 1, 0, 0,
  //     sizeof(pwm_duty), (uint8_t*)&pwm_duty,
  //     &pwm_service, &pwm_state_char);
  
  pwm_inist(&PWM31, pwm_ready_callbackg);
  // Start Advertising
  // simple_ble_adv_only_name();

  while(1) {

    update_pwm(&PWM31, 0, 90);
    nrf_delay_ms(500);
    update_pwm(&PWM31, 0, 50);
    nrf_delay_ms(500);
    update_pwm(&PWM31, 0, 20);
    nrf_delay_ms(500);
  }
}

