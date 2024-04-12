#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "esp_log.h"

#define TASK1_PERIOD_MS  1000
#define TASK2_PERIOD_MS  1050

#define TASK_PROC_TIME_SIMU_MS 100

#define MUTEX_WAIT_PERIOD_MS    60000

static const char TAG[] = "016";

typedef struct {
    uint32_t i;
    uint32_t j;
} sharedStruct_t;

static sharedStruct_t sharedStruct1 = {0, 0};
static sharedStruct_t sharedStruct2 = {65535, 65535};

// Mutex used to protect access to sharedStruct1.
static SemaphoreHandle_t mutex1;
// Same for sharedStruct2.
static SemaphoreHandle_t mutex2;

void vTaskCode1(void *pvParameters) {

    BaseType_t os_rs;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(TASK1_PERIOD_MS));
        // Request exclusive access to the first structure.
        ESP_LOGI(TAG, "Task 1 - requesting to acquire mutex1");
        os_rs = xSemaphoreTake(mutex1, pdMS_TO_TICKS(MUTEX_WAIT_PERIOD_MS));
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't acquire mutex1");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
        ESP_LOGI(TAG, "Task 1 - mutex1 acquired");
        // Simulate some processing.
        vTaskDelay(pdMS_TO_TICKS(TASK_PROC_TIME_SIMU_MS));
        sharedStruct1.i += 1;
        sharedStruct1.j += 2;
        // Now request exclusive access to the second structure.
        ESP_LOGI(TAG, "Task 1 - requesting to acquire mutex2");
        os_rs = xSemaphoreTake(mutex2, pdMS_TO_TICKS(MUTEX_WAIT_PERIOD_MS));
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't acquire mutex2");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
        ESP_LOGI(TAG, "Task 1 - mutex2 acquired");
        // Simulate some processing.
        vTaskDelay(pdMS_TO_TICKS(TASK_PROC_TIME_SIMU_MS));
        sharedStruct2.i -= 1;
        sharedStruct2.j -= 2;
        // Release access.
        ESP_LOGI(TAG, "Task 1 - releasing mutex2");
        os_rs = xSemaphoreGive(mutex2);
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't release mutex2");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
        ESP_LOGI(TAG, "Task 1 - releasing mutex1");
        os_rs = xSemaphoreGive(mutex1);
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't release mutex1");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Task 1 committing suicide");
    vTaskDelete(NULL);

}

void vTaskCode2(void *pvParameters) {

    BaseType_t os_rs;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(TASK2_PERIOD_MS));
        // Request exclusive access to the second structure.
        ESP_LOGI(TAG, "Task 2 - requesting to acquire mutex2");
        os_rs = xSemaphoreTake(mutex2, pdMS_TO_TICKS(MUTEX_WAIT_PERIOD_MS));
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't acquire mutex2");
            // DWe consider we are in trouble.
            goto exit_on_fatal_error;
        }
        ESP_LOGI(TAG, "Task 2 - mutex2 acquired");
        // Simulate some processing.
        vTaskDelay(pdMS_TO_TICKS(TASK_PROC_TIME_SIMU_MS));
        sharedStruct2.i += 1;
        sharedStruct2.j += 2;
        // Now request exclusive access to the first structure.
        ESP_LOGI(TAG, "Task 2 - requesting to acquire mutex1");
        os_rs = xSemaphoreTake(mutex1, pdMS_TO_TICKS(MUTEX_WAIT_PERIOD_MS));
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't acquire mutex1");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
        ESP_LOGI(TAG, "Task 2 - mutex1 acquired");
        // Simulate some processing.
        vTaskDelay(pdMS_TO_TICKS(TASK_PROC_TIME_SIMU_MS));
        sharedStruct1.i -= 1;
        sharedStruct1.j -= 2;
        // Release access.
        ESP_LOGI(TAG, "Task 2 - releasing mutex1");
        os_rs = xSemaphoreGive(mutex1);
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't release mutex1");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
        ESP_LOGI(TAG, "Task 2 - releasing mutex2");
        os_rs = xSemaphoreGive(mutex2);
        if (os_rs != pdTRUE) {
            ESP_LOGW(TAG,
                     "Couldn't release mutex 2");
            // We consider we are in trouble.
            goto exit_on_fatal_error;
        }
    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Task 2 committing suicide");
    vTaskDelete(NULL);

}

void app_main(void) {

    mutex1 = xSemaphoreCreateMutex();
    if (mutex1 == NULL) {
        ESP_LOGE(TAG,
                 "Error from xSemaphoreCreateMutex");
        goto exit_on_fatal_error;
    }
    mutex2 = xSemaphoreCreateMutex();
    if (mutex2 == NULL) {
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
