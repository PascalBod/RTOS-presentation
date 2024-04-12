/**
 * See 01-detectionLed sample application for information about
 * this code.
 */
#include "esp_log.h"

#include "driver/gpio.h"

#include "led.h"
#include "messages.h"

// Detection LED is controlled by GPIO 21 (check the schematic).
#define GPIO_DETECT_LED GPIO_NUM_21
// LED on period, in ms.
#define ON_PERIOD_MS 200

// Input message queue.
static QueueHandle_t *queue = NULL;

static const char TAG[] = "LED";

void led_task_set_queue(QueueHandle_t *i_queue) {

    queue = i_queue;

}

void led_task(void *taskParameters) {

    if (queue == NULL) {
        ESP_LOGE(TAG, "Frame queue is NULL");
        goto exit_on_fatal_error;
    }

    esp_err_t rs;

    // Select GPIO function for the pin, enable pull-up, disable I/O.
    // No need to check returned status: it is always ESP_OK.
    gpio_reset_pin(GPIO_DETECT_LED);
    // Disable pull-up, as there is an external pull-down.
    rs = gpio_set_pull_mode(GPIO_DETECT_LED, GPIO_FLOATING);
    if (rs != ESP_OK) {
        ESP_LOGE(TAG, "Error from gpio_set_pull_mode: %s", esp_err_to_name(rs));
        goto exit_on_fatal_error;
    }
    rs = gpio_set_direction(GPIO_DETECT_LED, GPIO_MODE_OUTPUT);
    if (rs != ESP_OK) {
        ESP_LOGE(TAG, "Error from gpio_set_direction: %s", esp_err_to_name(rs));
        goto exit_on_fatal_error;
    }
    // Set LED off.
    rs = gpio_set_level(GPIO_DETECT_LED, 0);
    if (rs != ESP_OK) {
        ESP_LOGE(TAG, "Error from gpio_set_level: %s", esp_err_to_name(rs));
        goto exit_on_fatal_error;
    }

    led_msg_t msg;

    while (true) {

        if (xQueueReceive(*queue, &msg, portMAX_DELAY)) {
            if (msg.type != LED_MSG_START) {
                ESP_LOGE(TAG, "Unknown message: %d", msg.type);
                goto exit_on_fatal_error;
            }
            // Set LED on.
            ESP_LOGI(TAG, "Set LED on for some time");
            rs = gpio_set_level(GPIO_DETECT_LED, 1);
            if (rs != ESP_OK) {
                ESP_LOGE(TAG, "Error from gpio_set_level: %s", esp_err_to_name(rs));
                goto exit_on_fatal_error;
            }
            vTaskDelay(pdMS_TO_TICKS(ON_PERIOD_MS));
            // Set LED off.
            rs = gpio_set_level(GPIO_DETECT_LED, 0);
            if (rs != ESP_OK) {
                ESP_LOGE(TAG, "Error from gpio_set_level: %s", esp_err_to_name(rs));
                goto exit_on_fatal_error;
            }
        }

    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting");

}
