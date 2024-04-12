#include <stdbool.h>

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_sleep.h"

// Side button is connected to GPIO15 (check the schematic).
#define GPIO_SIDE_BUTTON GPIO_NUM_15

static const char TAG[] = "BTN";

static volatile bool press_detected;

// GPIO interrupt handler.
static void gpio_interrupt_handler(void *arg) {

    press_detected = true;

}

void app_main(void) {

	esp_err_t es_rs;

	press_detected = false;

	// TODO - Configure the ESP32 so that the above ISR (gpio_interrupt_handler)
	// is called when the side button is pushed.
	// Note: use gpio_install_isr_service function.
	// TODO - Why to use gpio_install_isr_service instead of gpio_isr_register?

	// Configure for wakeup.
	es_rs = gpio_wakeup_enable(GPIO_NUM_15, GPIO_INTR_LOW_LEVEL);
	if (es_rs != ESP_OK) {
	    ESP_LOGE(TAG, "Error from gpio_wakeup_enable: %s",
	            esp_err_to_name(es_rs));
	    goto exit_on_fatal_error;
	}
	es_rs = esp_sleep_enable_gpio_wakeup();
	if (es_rs != ESP_OK) {
	    ESP_LOGE(TAG, "Error from esp_sleep_enable_gpio_wakeup: %s",
	            esp_err_to_name(es_rs));
	    goto exit_on_fatal_error;
	}

	// Now, check the global flag.
	while (true) {
	    if (press_detected) {
	        press_detected = false;
	        ESP_LOGI(TAG, "Press detected");
	    }
	    // We enter sleep mode, to show that we can do nothing at all
	    // while waiting for the button to be pushed.
	    ESP_LOGI(TAG, "Ready to sleep");
		// Wait for print log message request to be completed.
		es_rs = uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM);
		if (es_rs != ESP_OK) {
			ESP_LOGE(TAG, "uart_wait_tx_idle_polling: %s - exiting",
					esp_err_to_name(es_rs));
			goto exit_on_fatal_error;
		}
		// Then start sleeping.
	    es_rs = esp_light_sleep_start();
		if (es_rs != ESP_OK) {
		    ESP_LOGE(TAG, "Error from esp_sleep_enable_gpio_wakeup: %s",
		            esp_err_to_name(es_rs));
		    goto exit_on_fatal_error;
		}
		ESP_LOGI(TAG, "Awake!");
	}

	exit_on_fatal_error:
	ESP_LOGI(TAG, "Exiting");

}
