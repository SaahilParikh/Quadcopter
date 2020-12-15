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

  bool sensors_initialized = false;
  bool flying = false;
  COPTER_MASS = 470;

  update_target(&pitch_displacement, 0);
	update_target(&roll_displacement, 0);
	update_target(&yaw_displacement, 0);

  while(1) {
    if (sensors_initialized) {
      get_orientation(&current_orient);
    }
    if (flying) {
      	fly_w_o_sensorget();
    }
    enum Buttons b = button_state;
    switch (b) {
      case NONE:
        
      break;
      case RLEFT:
      if (prev_state == NONE) {
        update_target(&roll_displacement, roll_displacement.target + 5);
    	}
        
      break;
      case RRIGHT:
         if (prev_state == NONE) {
        update_target(&roll_displacement, roll_displacement.target + -5);
    	}

      break;
      case PFWD:
         if (prev_state == NONE) {
        update_target(&pitch_displacement, pitch_displacement.target + 5);
    	}
      break;
      case PBACK:
        if (prev_state == NONE) {
        update_target(&pitch_displacement, pitch_displacement.target - 5);
    	}
      break;
      case YLEFT:
      if (prev_state == NONE) {
        update_target(&yaw_displacement, yaw_displacement.target + 10);
    	}
      break;
      case YRIGHT:
      if (prev_state == NONE) {
        update_target(&yaw_displacement, yaw_displacement.target - 10);
      }
      break;
      case ALTUP:
      	if (prev_state == NONE) {
          COPTER_MASS += 10.0;
        }
      	
      break;
      case ALTDOWN:
      	if (prev_state == NONE) {
          COPTER_MASS -= 10.0;
        }
      break;
      case ARM:
        if (prev_state == NONE) {
          arm();
          sensors_initialized = true;
        }
      break;
      case INIT:
      	if (prev_state == NONE) {
          init_flight_controller();
        }
      break;
      case KILL:
      	if (prev_state == NONE) {
      		if (flying) {
      			kill();
      		}
      		flying = !flying;
      	}
      break;
    }
    nrf_delay_ms(1);
    prev_state = b;
  }
}

