#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"

#include "common.h"

// Timer period, in microseconds.
#define TIMER_PERIOD_US 500000

// Red LED is controlled by GPIO 21 (check the schematic).
#define RED_LED GPIO_NUM_21

typedef enum {
	ST_BLINKING,
	ST_NOT_BLINKING,
} state_t;

static state_t current_state;

static const char TAG[] = "LED";
static const DRAM_ATTR char DRAM_TAG[] = "LED";

static esp_timer_handle_t periodic_timer;

static uint32_t led_level;

static void periodic_timer_callback(void* arg)
{

	if (led_level == 0) {
		led_level = 1;
	} else {
		led_level = 0;
	}
	esp_err_t es_rs = gpio_set_level(RED_LED, led_level);

	// Usual ESP_LOGx macros should not be called from an ISR.
	ESP_DRAM_LOGI(DRAM_TAG, "Timer callback - %d", es_rs);

}

void led_task_init(void) {

	// Let's configure the GPIO.
	// No need to check returned status: it is always ESP_OK.
	// Reset actions:
	// - Selects gpio function
	// - Enables pullup
	// - Disables input and output
	gpio_reset_pin(RED_LED);
	// Configure for output.
	esp_err_t es_rs = gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
	if (es_rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_set_direction: %s", esp_err_to_name(es_rs));
		goto exit_on_fatal_error;
	}
	// Disable pull-up, as there is an external pull-down.
	es_rs = gpio_set_pull_mode(RED_LED, GPIO_FLOATING);
	if (es_rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_set_pull_mode: %s", esp_err_to_name(es_rs));
		goto exit_on_fatal_error;
	}

	// Shut the LED off.
	led_level = 0;
	es_rs = gpio_set_level(RED_LED, led_level);
	if (es_rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_set_level: %s", esp_err_to_name(es_rs));
		goto exit_on_fatal_error;
	}

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            .name = "Exercise-05"
    };

    es_rs = esp_timer_create(&periodic_timer_args, &periodic_timer);
	if (es_rs != ESP_OK) {
	    ESP_LOGE(TAG, "Error from esp_timer_create: %s",
	             esp_err_to_name(es_rs));
	    goto exit_on_fatal_error;
	}

	es_rs = esp_timer_start_periodic(periodic_timer, TIMER_PERIOD_US);
	if (es_rs != ESP_OK) {
	    ESP_LOGE(TAG, "Error from esp_timer_start_periodic: %s",
	             esp_err_to_name(es_rs));
	    goto exit_on_fatal_error;
	}
	current_state = ST_BLINKING;

	return;

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Resetting");
	esp_restart();

}

void led_task_run(void) {

	if (!push_event) {
		return;
	}

	// At this stage, a push event occurred, process it.
	esp_err_t es_rs;
	push_event = false;
	switch (current_state) {
	case ST_BLINKING:
		// Stop blinking.
		es_rs = esp_timer_stop(periodic_timer);
		if (es_rs != ESP_OK) {
		    ESP_LOGE(TAG, "Error from esp_timer_stop: %s",
		             esp_err_to_name(es_rs));
		    goto exit_on_fatal_error;
		}
		// Set the LED off.
		es_rs = gpio_set_level(RED_LED, 0);
		if (es_rs != ESP_OK) {
		    ESP_LOGE(TAG, "Error from gpio_set_level: %s",
		             esp_err_to_name(es_rs));
		    goto exit_on_fatal_error;
		}
		current_state = ST_NOT_BLINKING;
		break;
	case ST_NOT_BLINKING:
		// Start blinking.
		es_rs = esp_timer_start_periodic(periodic_timer, TIMER_PERIOD_US);
		if (es_rs != ESP_OK) {
		    ESP_LOGE(TAG, "Error from esp_timer_start_periodic: %s",
		             esp_err_to_name(es_rs));
		    goto exit_on_fatal_error;
		}
		current_state = ST_BLINKING;
		break;
	default:
		ESP_LOGE(TAG, "Unknown state for led_task_run: %d", current_state);
		goto exit_on_fatal_error;
	}

	return;

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Resetting");
	esp_restart();

}
