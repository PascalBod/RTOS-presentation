#include "esp_log.h"

#include "who_camera.h"

#include "camera.h"
#include "messages.h"

#define WAIT_QUEUE_REC_PERIOD_MS 30000

static const char TAG[] = "CAM";

// Input message queue.
static QueueHandle_t *queue = NULL;

// Queue receiving frames from the camera.
static QueueHandle_t *frame_queue = NULL;

// Returns true if success, false otherwise.
static bool stop_who_camera_task(void) {

    // Get the handle of the task.
    TaskHandle_t handle = xTaskGetHandle("who_camera");
    if (handle == NULL) {
        ESP_LOGE(TAG, "who_camera task not found");
        return false;
    }
    // Delete the task.
    vTaskDelete(handle);
    // We have a race condition here: a call to esp_camera_deinit may
    // lead to the insertion of an inconsistent frame into the frame
    // queue. So we reset the queue here, to decrease the probability
    // of occurrence of this problem.
    xQueueReset(*frame_queue);
    //
    return true;

}

void camera_task_set_queue(QueueHandle_t *i_queue) {

    queue = i_queue;

}

void camera_task_set_frame_queue(QueueHandle_t *f_queue) {

    frame_queue = f_queue;

}

void camera_task(void *taskParameters) {

    // Delay used for xTicksToWait when calling xQueueReceive().
    const TickType_t delay_queue_rec = pdMS_TO_TICKS(WAIT_QUEUE_REC_PERIOD_MS);

    // Ensure that queues have been defined.
    if (queue == NULL) {
        ESP_LOGE(TAG, "Input queue not defined");
        goto exit_on_fatal_error;
    }
    if (frame_queue == NULL) {
        ESP_LOGE(TAG, "Frame queue not defined");
        goto exit_on_fatal_error;
    }

    esp_err_t es_rs;
    cam_msg_t msg;

    while (true) {

        BaseType_t fr_rs = xQueueReceive(*queue, &msg, delay_queue_rec);
        if (fr_rs != pdTRUE) {
            ESP_LOGI(TAG, "End of wait for xQueueReceive");
            continue;
        }

        // Camera task works under the control of the main task. The main task is
        // in charge of remembering the current state of the camera (started or
        // not). Consequently, the camera task can be stateless.
        switch (msg.type) {
        case CAM_MSG_START:
            // Register_camera initializes the camera, and starts a task which gets
            // frames from the camera and sends them to frame_queue.
            ESP_LOGI(TAG, "Starting camera");
            register_camera(PIXFORMAT_RGB565, FRAMESIZE_QVGA, 2, *frame_queue);
            break;
        case CAM_MSG_STOP:
            ESP_LOGI(TAG, "Stopping camera");
            bool bool_rs = stop_who_camera_task();
            if (!bool_rs) {
                ESP_LOGE(TAG, "Couldn't stop who_camera task");
                goto exit_on_fatal_error;
            }
            es_rs = esp_camera_deinit();
            if (es_rs != ESP_OK) {
                ESP_LOGE(TAG, "Error on camera deinit: %d", es_rs);
                goto exit_on_fatal_error;
            }
            break;
        default:
            ESP_LOGE(TAG, "Unknown message type: %d", msg.type);
            goto exit_on_fatal_error;
        }

    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting");

}
