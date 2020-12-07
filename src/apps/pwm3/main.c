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



static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);

nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};

nrf_pwm_sequence_t const seq =
{
    .values.p_common = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};



void pwm_update_duty_cycle(uint8_t duty_cycle[4])
{
    seq_values->channel_0 = duty_cycle[0] >= 100 ? 100 : duty_cycle[0];
    seq_values->channel_1 = duty_cycle[1] >= 100 ? 100 : duty_cycle[1];
    seq_values->channel_2 = duty_cycle[2] >= 100 ? 100 : duty_cycle[2];
    seq_values->channel_3 = duty_cycle[3] >= 100 ? 100 : duty_cycle[3];

    nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}


static void pwm_init(void)
{
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            NRF_GPIO_PIN_MAP(0, 25), // channel 0
            NRF_GPIO_PIN_MAP(0, 24),             // channel 1
            NRF_GPIO_PIN_MAP(0, 23),             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 100,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));
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
  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 24));
  nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 25));


  NRF_CLOCK->TASKS_HFCLKSTART = 1; 

  // Wait for clock to start
  while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

  pwm_init();

  uint8_t up[] = {0, 40, 80, 100};
  while(1) {
    // Start clock for accurate frequencies
      for(uint16_t i = 0; i <= 100; i++)
      {
          nrf_delay_ms(10);
          up[0] += 1;
          up[1] += 1;
          up[2] += 1;
          up[3] += 1;

          up[0] %= 100;
          up[1] %= 100;
          up[2] %= 100;
          up[3] %= 100;


          pwm_update_duty_cycle(up);
      }

  }
    
}







// Declare variables holding PWM sequence values. In this example only one channel is used 









// Set duty cycle between 0 and 100%










