#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"

#define TASK_SEND_PERIOD_MS  5000
#define QUEUE_ITEM_WAIT_PERIOD_MS   10000

#define QUEUE_LENGTH    5

static const char TAG[] = "015";

static QueueHandle_t queue;

void producerCode(void *pvParameters) {

    ESP_LOGI(TAG, "Producer started");

    uint32_t counter = 0;
    BaseType_t os_rs;

    while (true) {
        ESP_LOGI(TAG, "Producer - Writing %lu into queue", counter);
        os_rs = xQueueSendToBack(queue, &counter, 0);
        if (os_rs == errQUEUE_FULL) {
            ESP_LOGW(TAG, "Producer - Queue full, counter lost");
        }
        counter++;
        vTaskDelay(pdMS_TO_TICKS(TASK_SEND_PERIOD_MS));
    }

}

void consumerCode(void *pvParameters) {

    ESP_LOGI(TAG, "Consumer started");

    uint32_t counter;
    BaseType_t os_rs;

    while (true) {
        os_rs = xQueueReceive(queue, &counter, QUEUE_ITEM_WAIT_PERIOD_MS);
        if (os_rs == pdFALSE) {
            ESP_LOGW(TAG, "Consumer - Queue empty");
            continue;
        }
        ESP_LOGI(TAG, "Consumer - Received %lu", counter);
    }

}

void app_main(void) {

    // Create the queue. Important: must be created before creating
    // the tasks.
    queue = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));
    if (queue == NULL) {
        ESP_LOGE(TAG, "Error from xQueueCreate");
        goto exit_on_fatal_error;
    }

    // Start the two tasks.
    BaseType_t os_rs = xTaskCreatePinnedToCore(
            producerCode,       // Task code.
            "producer",         // Task name.
            2048,               // Task stack depth (in bytes).
            NULL,               // Task parameters. Not used here.
            5,                  // Priority.
            NULL,               // Task handle (output). Not used here.
            APP_CPU_NUM         // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
        goto exit_on_fatal_error;
    }
    os_rs = xTaskCreatePinnedToCore(
            consumerCode,       // Task code.
            "consumer",         // Task name.
            2048,               // Task stack depth (in bytes).
            NULL,               // Task parameters. Not used here.
            5,                  // Priority.
            NULL,               // Task handle (output). Not used here.
            APP_CPU_NUM         // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
        goto exit_on_fatal_error;
    }
    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting");

}
