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
#include "nrf_drv_pwm.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "boards.h"
#include "bsp.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
#include "pwm.h"


//Modified from example//
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(1);

nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};

nrf_pwm_sequence_t const seq =
{
    .values.p_common = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};


void pwm_update_duty_cycle(const uint16_t duty_cycle[4])
{
    seq_values->channel_0 = (duty_cycle[0] >= 312 ? 312 : duty_cycle[0])  | (1 << 15);
    seq_values->channel_1 = (duty_cycle[1] >= 312 ? 312 : duty_cycle[1]) | (1 << 15);
    seq_values->channel_2 = (duty_cycle[2] >= 312 ? 312 : duty_cycle[2]) | (1 << 15);
    seq_values->channel_3 = (duty_cycle[3] >= 312 ? 312 : duty_cycle[3]) | (1 << 15);

    nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}


void pwm_init(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 25));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 23));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 6));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 5));
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            NRF_GPIO_PIN_MAP(0, 25), // channel 0
            NRF_GPIO_PIN_MAP(0, 23),             // channel 1
            NRF_GPIO_PIN_MAP(0, 6),             // channel 2
            NRF_GPIO_PIN_MAP(0, 5),             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_125kHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 312,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));
}


