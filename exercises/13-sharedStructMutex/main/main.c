#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "esp_log.h"

#define TASK1_PERIOD_MS  20
#define TASK2_PERIOD_MS  27

#define MUTEX_WAIT_PERIOD_MS    1000

#define MULTIPLIER1 2
#define MULTIPLIER2 3

static const char TAG[] = "013";

typedef struct {
    uint32_t i;
    uint32_t j;
} sharedStruct_t;

static sharedStruct_t sharedStruct = {0, 0};

static SemaphoreHandle_t mutex;

// TODO - Use mutex to implement atomic access to the shared structure.

void vTaskCode1(void *pvParameters) {

    BaseType_t os_rs;

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

    BaseType_t os_rs;

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

    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        ESP_LOGE(TAG,
                 "Error from xSemaphoreCreateMutex");
        goto exit_on_fatal_error;
    }

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
