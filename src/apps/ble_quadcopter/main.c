// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "nrf_twi_mngr.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "display.h"

#include "simple_ble.h"
#include "buckler.h"

#include "max44009.h"

#include "buttons.h"
#include "ble_comm.h"
#include "flight_controller.h"

/*******************************************************************************
 *   State for this application
 ******************************************************************************/

static enum Buttons prev_state = NONE;

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
  display_write("Quadcopter", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  init_ble();

  while(1) {
    enum Buttons b = button_state;
    switch (b) {
      case NONE:
      //TODO
      break;
      case RLEFT:
      //TODO
      break;
      case RRIGHT:
      //TODO
      break;
      case PFWD:
      //TODO
      break;
      case PBACK:
      //TODO
      break;
      case YLEFT:
      //TODO
      break;
      case YRIGHT:
      //TODO
      break;
      case ALTUP:
      //TODO
      break;
      case ALTDOWN:
      //TODO
      break;
      case ARM:
        if (prev_state == NONE) {
          arm();
        }
      break;
      case INIT:
      	if (prev_state == NONE) {
          init_flight_controller();
        }
      break;
      case KILL:
      	kill();
      break;
    }
    nrf_delay_ms(1);
    prev_state = b;
  }
}

