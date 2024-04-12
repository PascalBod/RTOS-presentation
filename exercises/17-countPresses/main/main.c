#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"

// Side button is connected to GPIO15 (check the schematic).
#define GPIO_SIDE_BUTTON GPIO_NUM_15

#define QUEUE_ITEM_WAIT_PERIOD_MS   10000

#define QUEUE_LENGTH    10

static const char TAG[] = "017";
static const DRAM_ATTR char DRAM_TAG[] = "017";

// Queue to be used.
static QueueHandle_t queue;

// GPIO interrupt handler.
static void gpio_interrupt_handler(void *arg) {

    // TODO - Add some code here.
    ESP_DRAM_LOGI(DRAM_TAG, "blablabla");

}

void app_main(void) {

    BaseType_t os_rs;

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

    // Create the queue. Important: must be created before enabling
    // the interruption.
    queue = xQueueCreate(QUEUE_LENGTH, sizeof(TickType_t));
    if (queue == NULL) {
        ESP_LOGE(TAG, "Error from xQueueCreate");
        goto exit_on_fatal_error;
    }

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

    // Now, wait for presses.
    TickType_t tickCount;
    while (true) {

        // TODO - Add some code here.

    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting");

}
