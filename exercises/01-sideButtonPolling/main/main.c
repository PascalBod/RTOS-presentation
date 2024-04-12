#include <stdbool.h>
#include <unistd.h>

#include "driver/gpio.h"
#include "esp_log.h"

// Polling period, in microseconds.
#define POLL_PERIOD_US 333000

static const char TAG[] = "BTN";

void app_main(void)
{

	esp_err_t es_rs;

	// Let's configure the GPIO.
	gpio_config_t io_conf = {0};
	// Pin bit mask is 64-bit long => ULL suffix.
	io_conf.pin_bit_mask = (1ULL << GPIO_NUM_15);
	io_conf.mode = GPIO_MODE_INPUT;
	// Activate internal pull-up so that when the button is not pushed,
	// GPIO level is 3.3 V.
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	// Disable interrupts.
	io_conf.intr_type = GPIO_INTR_DISABLE;
	es_rs = gpio_config(&io_conf);
	if (es_rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_config: %s",
				 esp_err_to_name(es_rs));
		goto exit_on_fatal_error;
	}
	// At this stage, GPIO configuration is OK.

	while (true) {

		// TODO - Add here code to read value of GPIO 15 and to display it.

		// If we don't sleep a little, ESP-IDF considers there is a problem:
		// a watchdog resets the microcontroller. Minimum period is 10 ms.
		usleep(POLL_PERIOD_US);

		// TODO - Answer the question: why the delay, above, in addition to the
		// watchdog constraint?

		// TODO - Can you list two drawbacks of the "architecture" of this code?

	}

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Exiting");

}
