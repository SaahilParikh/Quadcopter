// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "nrf_twi_mngr.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "display.h"
#include "nrf_delay.h"

#include "buckler.h"
#include "pwm.h"


APP_PWM_INSTANCE(PWM2, 1);
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
  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 24));
  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 25));


  
  pwm_init_two(&PWM2, NRF_GPIO_PIN_MAP(0, 24), NRF_GPIO_PIN_MAP(0, 25), pwm_ready_callback);

  while(1) {

    //update_pwm(&PWMLED1, 0, 90);
  //   printf("led 0 %x", BUCKLER_LED0);
  // printf("Converter: %X", NRF_GPIO_PIN_MAP(0, 25));

    update_pwm(&PWM2, 1, 30);
    update_pwm(&PWM2, 0, 30);
    nrf_delay_ms(500);
    update_pwm(&PWM2, 0, 50);
    nrf_delay_ms(500);
    update_pwm(&PWM2, 1, 100);
    update_pwm(&PWM2, 0, 20);
    nrf_delay_ms(500);
  }
}

