#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#define TASK1_PERIOD_MS  20
#define TASK2_PERIOD_MS  27

#define MULTIPLIER1 2
#define MULTIPLIER2 3

static const char TAG[] = "010";

typedef struct {
    uint32_t i;
    uint32_t j;
} sharedStruct_t;

static volatile sharedStruct_t sharedStruct = {0, 0};

void vTaskCode1(void *pvParameters) {

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(TASK1_PERIOD_MS));
        sharedStruct.i = 111;
        vTaskDelay(pdMS_TO_TICKS(TASK1_PERIOD_MS));
        sharedStruct.j = 111;
        ESP_LOGI(TAG, "1 - %lu %lu",
                 sharedStruct.i,
                 sharedStruct.j);
    }

}

void vTaskCode2(void *pvParameters) {

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(TASK2_PERIOD_MS));
        sharedStruct.i = 222;
        vTaskDelay(pdMS_TO_TICKS(TASK2_PERIOD_MS));
        sharedStruct.j = 222;
        ESP_LOGI(TAG, "2 - %lu %lu",
                 sharedStruct.i,
                 sharedStruct.j);
    }

}

void app_main(void) {

    BaseType_t os_rs = xTaskCreatePinnedToCore(
            vTaskCode1,         // Task code.
            "OurFirstTask",     // Task name.
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
    // The app_main function may return.

    os_rs = xTaskCreatePinnedToCore(
                vTaskCode2,         // Task code.
                "OurSecondTask",    // Task name.
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
