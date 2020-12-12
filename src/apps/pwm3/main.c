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

#include "app_pwm.h"


#include "nrf_drv_pwm.h"

#include "app_util_platform.h"

#include "app_error.h"

#include "boards.h"

#include "bsp.h"

#include "nrf_drv_clock.h"

#include "nrf_delay.h"

#include "nrfx_gpiote.h"




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
  
  nrf_delay_ms(5000);
  NRF_CLOCK->TASKS_HFCLKSTART = 1; 

  // Wait for clock to start
  while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

  pwm_init();
  uint16_t up[] = {0, 0, 0, 0};

  for(int i = 0; i < 50; i++){
    up[0] = 100;
    pwm_update_duty_cycle(up);
    nrf_delay_ms(50);
  }

  up[0] = 0;

  // for(int i = 0; i < 50; i++){
  //   up[0] = 253;
  //   pwm_update_duty_cycle(up);
  //   nrf_delay_ms(50);
  // }

  nrf_gpio_cfg_output(5);
  nrf_gpio_cfg_output(6);

  while(1) {
    // Start clock for accurate frequencies

      for(uint16_t i = 0; i <= 130; i++)
      {
          //nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0, 22));
          nrf_delay_ms(2);

          up[0] += 1;
          //printf("%u, ", up[0]);
          up[1] += 1;
          up[2] += 1;
          up[3] += 1;


          up[0] %= 312;
          up[1] %= 312;
          up[2] %= 312;
          up[3] %= 312;


          pwm_update_duty_cycle(up);
      }

  }
    
}







// Declare variables holding PWM sequence values. In this example only one channel is used 









// Set duty cycle between 0 and 312%










