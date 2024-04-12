#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "esp_log.h"

// Maximum time to wait for a semaphore to become available, in ms.
#define SEM_MAX_WAIT_PERIOD_MS 600000

#define TASK_WORKING_TIME_MS 10000

#define NUMBER_OF_ACTIVE_TASKS 2

static const char TAG[] = "014";

typedef struct {
    uint32_t id;
    uint32_t wait_time_ms;
    SemaphoreHandle_t semaphore;
} params_t;

// Task parameters. They must stay accessible until the end
// of the tasks. Thus, they are created in static storage.
static params_t params1;
static params_t params2;
static params_t params3;
static params_t params4;

void vTaskCode(void *pvParameters) {

    params_t *paramsPtr = (params_t *)pvParameters;
    uint32_t id = paramsPtr->id;

    ESP_LOGI(TAG, "Task %lu started", id);

    // TODO:
    // - Wait for wait_time_ms
    // - Try to take the semaphore
    // - Wait for TASK_WORKING_TIME_MS
    // - Give the semaphore
    // - Write log messages allowing to understand what's going on
    // - In case of error, commit suicide.

    ESP_LOGI(TAG, "%lu - Exiting", id);

    exit_on_fatal_error:
    vTaskDelete(NULL);

}

void app_main(void) {

    // TODO - Create a semaphore allowing two tasks to work and assign
    // it to the semaphore variable.
    SemaphoreHandle_t semaphore;

    // Prepare task parameters.
    params1.id = 1;
    params1.semaphore = semaphore;
    params1.wait_time_ms = 5000;
    //
    params2.id = 2;
    params2.semaphore = semaphore;
    params2.wait_time_ms = 7000;
    //
    params3.id = 3;
    params3.semaphore = semaphore;
    params3.wait_time_ms = 10000;
    //
    params4.id = 4;
    params4.semaphore = semaphore;
    params4.wait_time_ms = 14000;

    BaseType_t os_rs = xTaskCreatePinnedToCore(
            vTaskCode,              // Task code.
            "Task1",                // Task name.
            2048,                   // Task stack depth (in bytes).
            &params1,               // Task parameters.
            5,                      // Priority.
            NULL,                   // Task handle (output). Not used here.
            APP_CPU_NUM             // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
        goto exit_on_fatal_error;
    }

    os_rs = xTaskCreatePinnedToCore(
            vTaskCode,              // Task code.
            "Task2",                // Task name.
            2048,                   // Task stack depth (in bytes).
            &params2,               // Task parameters.
            5,                      // Priority.
            NULL,                   // Task handle (output). Not used here.
            APP_CPU_NUM             // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
        goto exit_on_fatal_error;
    }

    os_rs = xTaskCreatePinnedToCore(
            vTaskCode,              // Task code.
            "Task3",                // Task name.
            2048,                   // Task stack depth (in bytes).
            &params3,               // Task parameters.
            5,                      // Priority.
            NULL,                   // Task handle (output). Not used here.
            APP_CPU_NUM             // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
        goto exit_on_fatal_error;
    }

    os_rs = xTaskCreatePinnedToCore(
            vTaskCode,              // Task code.
            "Task4",                // Task name.
            2048,                   // Task stack depth (in bytes).
            &params4,               // Task parameters.
            5,                      // Priority.
            NULL,                   // Task handle (output). Not used here.
            APP_CPU_NUM             // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
        goto exit_on_fatal_error;
    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting");

}
