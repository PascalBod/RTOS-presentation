#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char TAG[] = "011";

typedef struct {
    uint32_t task_id;
    uint32_t task_period;
} params_t;

void vTaskCode(void *pvParameters) {

    params_t *paramsPtr = (params_t *)pvParameters;

    while (true) {
        ESP_LOGI(TAG, "Hello from task %lu!", paramsPtr->task_id);
        vTaskDelay(pdMS_TO_TICKS(paramsPtr->task_period));
    }

}

void app_main(void) {

    params_t params;

    // Start first task with task ID 0 and 500 ms for message period.
    params.task_id = 0;
    params.task_period = 500;

    BaseType_t os_rs = xTaskCreatePinnedToCore(
            vTaskCode,              // Task code.
            "OurFirstTask",         // Task name.
            2048,                   // Task stack depth (in bytes).
            &params,                // Task parameters.
            5,                      // Priority.
            NULL,                   // Task handle (output). Not used here.
            APP_CPU_NUM             // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
    }

    // Start second task with task ID 1 and 1 s for message period.
    params.task_id = 1;
    params.task_period = 1000;

    os_rs = xTaskCreatePinnedToCore(
            vTaskCode,              // Task code.
            "OurSecondTask",        // Task name.
            2048,                   // Task stack depth (in bytes).
            &params,                // Task parameters.
            5,                      // Priority.
            NULL,                   // Task handle (output). Not used here.
            APP_CPU_NUM             // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
    }

}
