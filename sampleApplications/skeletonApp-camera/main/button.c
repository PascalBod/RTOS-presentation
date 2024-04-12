/**
 * See 02-sideButton sample application for information about
 * this code.
 */
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

#include "button.h"
#include "messages.h"

// Side button is connected to GPIO15 (check the schematic).
#define GPIO_SIDE_BUTTON GPIO_NUM_15

#define TASK_WAIT_PERIOD_MS 60000

static const char TAG[] = "BTN";

static QueueHandle_t *main_queue = NULL;

static bool error_detected = false;

void button_task_set_main_queue(QueueHandle_t *m_queue) {

    main_queue = m_queue;

}

// GPIO interrupt handler.
static void gpio_interrupt_handler(void *arg) {

    // This handler is called in a interrupt context by the GPIO
    // service. So we don't generate any log messages here.
    if (main_queue == NULL) {
        error_detected = true;
        return;
    }
    // Inform main task.
    BaseType_t higher_priority_task_woken = pdFALSE;
    main_msg_t msg = {MAIN_MSG_BUTTON_PRESSED};
    BaseType_t fr_rs = xQueueSendFromISR(*main_queue, &msg, &higher_priority_task_woken);
    if (fr_rs != pdTRUE) {
        error_detected = true;
        return;
    }
    if (higher_priority_task_woken != pdFALSE) {
        // It's good to allow the higher priority task to be scheduled.
        portYIELD_FROM_ISR();
    }

}

void button_task(void *taskParameters) {

    // Install GPIO ISR service. According to Espressif's
    // gpio_example_main.c example, 0 seems to be a good value for the
    // argument to gpio_install_isr_service.
    // Note: ESP-WHO camera component calls gpio_install_isr_service
    // as well. This will generate an error message, which can be ignored.
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

    // Keep the task alive, but do (almost) nothing.
    while(true) {
        vTaskDelay(pdMS_TO_TICKS(TASK_WAIT_PERIOD_MS));
    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting from button_task");

}
