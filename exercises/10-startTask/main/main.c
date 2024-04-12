#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#define TASK_MESSAGE_PERIOD_MS	500

#define MAIN_LOOP_PERIOD_MS 5000

static const char TAG[] = "010";

void vTaskCode(void *pvParameters) {

    while (true) {
        ESP_LOGI(TAG, "Hello from task!");
        vTaskDelay(pdMS_TO_TICKS(TASK_MESSAGE_PERIOD_MS));
    }
    // A task function MUST NOT return.

}

void app_main(void) {

    BaseType_t os_rs = xTaskCreatePinnedToCore(
            vTaskCode,          // Task code.
            "OurFirstTask",	    // Task name.
            2048,               // Task stack depth (in bytes).
            NULL,               // Task parameters. Not used here.
            5,                  // Priority.
            NULL,               // Task handle (output). Not used here.
            APP_CPU_NUM         // Run task on APP_CPU.
            );
    if (os_rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreatePinnedToCore: %d", os_rs);
    }
    // The app_main function may return.

}
