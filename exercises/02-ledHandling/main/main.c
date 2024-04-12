#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "driver/gpio.h"
#include "esp_log.h"

// Red LED is controlled by GPIO 21 (check the schematic).
#define RED_LED GPIO_NUM_21

static const char TAG[] = "LED";

void app_main(void)
{

	esp_err_t rs;

	// Let's configure the GPIO.
	// No need to check returned status: it is always ESP_OK.
	// Reset actions:
	// - Selects gpio function
	// - Enables pullup
	// - Disables input and output
	gpio_reset_pin(RED_LED);
	// Configure for output.
	rs = gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
	if (rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_set_direction: %s", esp_err_to_name(rs));
		goto exit_on_fatal_error;
	}
	// Disable pull-up, as there is an external pull-down.
	rs = gpio_set_pull_mode(RED_LED, GPIO_FLOATING);
	if (rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_set_pull_mode: %s", esp_err_to_name(rs));
		goto exit_on_fatal_error;
	}

	// TODO - mMake the red LED blink 500 ms off / 500 ms on

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Exiting");

}
