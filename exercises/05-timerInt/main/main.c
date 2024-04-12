#include <stdint.h>
#include <unistd.h>

#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_timer.h"

// Red LED is controlled by GPIO 21 (check the schematic).
#define RED_LED GPIO_NUM_21

// Timer period, in microseconds.
#define TIMER_PERIOD_US 500000

// Time period to wait for before stopping, in microseconds.
#define RUN_PERIOD_US 20000000

static const char TAG[] = "TIM";
static const DRAM_ATTR char DRAM_TAG[] = "TIM";

static void periodic_timer_callback(void* arg)
{

	// TODO - Reverse LED state.

	// Usual ESP_LOGx macros should not be called from an ISR.
	ESP_DRAM_LOGI(DRAM_TAG, "Timer callback");

}

void app_main(void)
{

	// TODO - Initialize GPIO associated to red LED, and set it off.

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            .name = "Exercise-05"
    };

    esp_timer_handle_t periodic_timer;

    // TODO - Create and start periodic_timer, as a periodic timer of
    // period TIMER_PERIOD_MICROS.

	// Let's wait a bit before exiting.
	usleep(RUN_PERIOD_US);

	// TODO - Stop and deallocate timer resources.

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Exiting");

}
