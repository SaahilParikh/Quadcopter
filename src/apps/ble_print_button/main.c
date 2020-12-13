// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "app_pwm.h"
#include "nrf_twi_mngr.h"
#include "nrf_pwm.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "display.h"

#include "simple_ble.h"
#include "buckler.h"

#include "max44009.h"

#include "buttons.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x1234, // TODO: replace with your lab bench number
        .adv_name          = "EE149 Quadcopter", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(50, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(100, UNIT_1_25_MS),
};

//62295813-af1c-3eb5-4a07-c92f9053c5c5
static simple_ble_service_t button_service = {{
    .uuid128 = {0xc5, 0xc5, 0x53, 0x90, 0x2f, 0xc9, 0x07, 0x4a,
                0xb5, 0x3e, 0x1c, 0xaf, 0x13, 0x58, 0x29, 0x62}
}};

static simple_ble_char_t button_state_char = {.uuid16 = 0x108a};
static int32_t button_state = 0;

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &button_state_char)) {
      enum Buttons b = button_state;
      char buf[64];
      snprintf(buf, sizeof(buf), "b: %s", get_button_name(b));
      display_write(buf, DISPLAY_LINE_1);
      printf("%s\n", buf);
    }
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

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&button_service);

  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(button_state), (uint8_t*)&button_state,
      &button_service, &button_state_char);
  
  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}

