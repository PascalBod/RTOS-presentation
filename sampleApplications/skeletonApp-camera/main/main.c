#include <button.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "esp_log.h"

#include "who_camera.h"

#include "button.h"
#include "camera.h"
#include "frame.h"
#include "led.h"
#include "messages.h"

#define BUTTON_TASK_STACK_DEPTH 3000
#define CAMERA_TASK_STACK_DEPTH 3000
#define FRAME_TASK_STACK_DEPTH 3000
#define LED_TASK_STACK_DEPTH 3000

#define BUTTON_TASK_PRIORITY 5
#define CAMERA_TASK_PRIORITY 5
#define FRAME_TASK_PRIORITY 5
#define LED_TASK_PRIORITY 5

#define WAIT_QUEUE_REC_PERIOD_MS 30000

static const char TAG[] = "AMN";

// Input message queue.
static QueueHandle_t queue;

// Queues of other tasks.
static QueueHandle_t camera_queue;
static QueueHandle_t frame_queue;
static QueueHandle_t led_queue;

// Possible states of the FSM implementing the application.
typedef enum {
    ST_WAIT_START_CAMERA,
    ST_WAIT_STOP_CAMERA,
} state_t;

void app_main(void) {

    // Delay used for xTicksToWait when calling xQueueReceive().
    const TickType_t delay_queue_rec = pdMS_TO_TICKS(WAIT_QUEUE_REC_PERIOD_MS);

    // Create all task queues.
    queue = xQueueCreate(2, sizeof(main_msg_t));
    camera_queue = xQueueCreate(2, sizeof(cam_msg_t));
    // Set queue length to one, so that the queue does not buffer more than one
    // frame, when the user function takes time to execute.
    frame_queue = xQueueCreate(1, sizeof(camera_fb_t *));
    led_queue = xQueueCreate(2, sizeof(led_msg_t));

    // Configure tasks queues.
    button_task_set_main_queue(&queue);
    camera_task_set_queue(&camera_queue);
    camera_task_set_frame_queue(&frame_queue);
    frame_task_set_queue(&frame_queue);
    frame_task_set_led_queue(&led_queue);
    led_task_set_queue(&led_queue);

    // Start the tasks.
    BaseType_t rs = xTaskCreate(button_task, "buttonTask", BUTTON_TASK_STACK_DEPTH,
                                NULL, BUTTON_TASK_PRIORITY, NULL);
    if (rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreate");
        goto exit_on_fatal_error;
    }
    rs = xTaskCreate(camera_task, "cameraTask", CAMERA_TASK_STACK_DEPTH,
                     NULL, CAMERA_TASK_PRIORITY, NULL);
    if (rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreate");
        goto exit_on_fatal_error;
    }
    rs = xTaskCreate(frame_task, "frameTask", FRAME_TASK_STACK_DEPTH,
                     NULL, FRAME_TASK_PRIORITY, NULL);
    if (rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreate");
        goto exit_on_fatal_error;
    }
    rs = xTaskCreate(led_task, "ledTask", LED_TASK_STACK_DEPTH,
                     NULL, LED_TASK_PRIORITY, NULL);
    if (rs != pdPASS) {
        ESP_LOGE(TAG, "Error from xTaskCreate");
        goto exit_on_fatal_error;
    }

    state_t state = ST_WAIT_START_CAMERA;

    main_msg_t msg;
    cam_msg_t msg_to_camera;

    // Wait for incoming messages.
    while (true) {

        BaseType_t fr_rs = xQueueReceive(queue, &msg, delay_queue_rec);
        if (fr_rs != pdTRUE) {
            ESP_LOGI(TAG, "End of wait for xQueueReceive");
            continue;
        }

        switch (state) {

        case ST_WAIT_START_CAMERA:
            if (msg.type != MAIN_MSG_BUTTON_PRESSED) {
                ESP_LOGE(TAG, "Wrong message type: %d", msg.type);
                goto exit_on_fatal_error;
            }
            ESP_LOGI(TAG, "Button pressed => start camera");
            // We have to start the camera.
            msg_to_camera.type = CAM_MSG_START;
            rs = xQueueSend(camera_queue, &msg_to_camera, 0);
            if (rs != pdTRUE) {
                goto exit_on_fatal_error;
            }
            // Set next state.
            state = ST_WAIT_STOP_CAMERA;
            break;

        case ST_WAIT_STOP_CAMERA:
            if (msg.type != MAIN_MSG_BUTTON_PRESSED) {
                ESP_LOGE(TAG, "Wrong message type: %d", msg.type);
                goto exit_on_fatal_error;
            }
            ESP_LOGI(TAG, "Button pressed => stop camera");
            // We have to stop the camera.
            msg_to_camera.type = CAM_MSG_STOP;
            rs = xQueueSend(camera_queue, &msg_to_camera, 0);
            if (rs != pdTRUE) {
                goto exit_on_fatal_error;
            }
            // Set next state.
            state = ST_WAIT_START_CAMERA;
            break;

        default:
            // Error!
            ESP_LOGE(TAG, "Unknown state: %d", state);
            goto exit_on_fatal_error;

        }

    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting from main");

}
