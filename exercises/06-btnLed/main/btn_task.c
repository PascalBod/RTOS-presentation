#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"

#include "common.h"

// Maximum period of time between two successive button pushes for
// generating the "push" event.
#define PRESS_PERIOD_US 1000000

typedef enum {
	ST_WAIT_FIRST_PRESS,
	ST_WAIT_SECOND_PRESS,
} state_t;

static state_t current_state;

static const char TAG[] = "BTN";

static volatile bool press_detected;

static int64_t prev_press_time;

// Get current time with one microsecond resolution.
static int64_t get_current_time_us(void) {

	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);
	int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
	return time_us;

}

// GPIO interrupt handler.
static void gpio_interrupt_handler(void *arg) {

    press_detected = true;

}

void btn_task_init(void) {

	// Install GPIO ISR service. According to Espressif's
	// gpio_example_main.c example, 0 seems to be a good value for the
	// argument to gpio_install_isr_service.
	esp_err_t es_rs = gpio_install_isr_service(0);
	if (es_rs != ESP_OK) {
			ESP_LOGE(TAG, "Error from gpio_install_isr_service: %s",
					 esp_err_to_name(es_rs));
			goto exit_on_fatal_error;
		}

	// Let's configure the GPIO.
	gpio_config_t io_conf = {0};
	// Pin bit mask is 64-bit long => ULL suffix.
	io_conf.pin_bit_mask = (1ULL << GPIO_NUM_15);
	io_conf.mode = GPIO_MODE_INPUT;
	// Activate internal pull-up so that when the button is not pushed,
	// GPIO level is 3.3 V.
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	// Disable interrupts while configuring.
	io_conf.intr_type = GPIO_INTR_DISABLE;
	es_rs = gpio_config(&io_conf);
	if (es_rs != ESP_OK) {
		ESP_LOGE(TAG, "Error from gpio_config: %s",
				 esp_err_to_name(es_rs));
		goto exit_on_fatal_error;
	}
	// At this stage, GPIO configuration is OK.
	press_detected = false;
	// Now, configure interruption.
	// Assign the interrupt handler.
	es_rs = gpio_isr_handler_add(GPIO_NUM_15, gpio_interrupt_handler,
	                             NULL);
	if (es_rs != ESP_OK) {
	    ESP_LOGE(TAG, "Error from gpio_isr_handler_add: %s",
	            esp_err_to_name(es_rs));
	    goto exit_on_fatal_error;
	}
	// Now, enable interrupt for the GPIO.
	es_rs = gpio_set_intr_type(GPIO_NUM_15, GPIO_INTR_NEGEDGE);
	if (es_rs != ESP_OK) {
	    ESP_LOGE(TAG, "Error from gpio_set_intr_type: %s",
	            esp_err_to_name(es_rs));
	    goto exit_on_fatal_error;
	}

	current_state = ST_WAIT_FIRST_PRESS;

	return;

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Resetting");
	esp_restart();

}

void btn_task_run(void) {

	if (!press_detected) {
		return;
	}

	// At this stage, the button has been pushed.
	press_detected = false;
	switch (current_state) {
	case ST_WAIT_FIRST_PRESS:
		prev_press_time = get_current_time_us();
		current_state = ST_WAIT_SECOND_PRESS;
		break;
	case ST_WAIT_SECOND_PRESS:
		int64_t press_time = get_current_time_us();
		if (press_time - prev_press_time < PRESS_PERIOD_US) {
			// Generate push event.
			push_event = true;
			ESP_LOGI(TAG, "Push event");
			//
			current_state = ST_WAIT_FIRST_PRESS;
			break;
		}
		// At this stage, too much time between the two pushes. Forget about
		// previous one.
		prev_press_time = press_time;
		// Stay in same state;
		break;
	default:
		ESP_LOGE(TAG, "Unknown state for btn_task_run: %d", current_state);
		goto exit_on_fatal_error;
	}

	return;

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Resetting");
	esp_restart();

}
