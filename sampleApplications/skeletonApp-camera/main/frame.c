#include "esp_log.h"

#include "who_camera.h"

#include "frame.h"
#include "messages.h"
#include "userFunction.hpp"

static const char TAG[] = "FRM";

// Input message queue.
static QueueHandle_t *queue = NULL;

// Message queue for controlling the LED.
static QueueHandle_t *led_queue = NULL;

void frame_task_set_queue(QueueHandle_t *i_queue) {

    queue = i_queue;

}

void frame_task_set_led_queue(QueueHandle_t *l_queue) {

    led_queue = l_queue;

}

void frame_task(void *taskParameters) {

    if (queue == NULL) {
        ESP_LOGE(TAG, "Frame queue is NULL");
        goto exit_on_fatal_error;
    }

    if (led_queue == NULL) {
        ESP_LOGE(TAG, "Led queue is NULL");
        goto exit_on_fatal_error;
    }

    camera_fb_t *frame = NULL;

    while (true) {

        if (xQueueReceive(*queue, &frame, portMAX_DELAY)) {
            ESP_LOGI(TAG, "%u", frame->len);
            // Call C++ user function.
            bool bool_rs = userFunction(frame->buf, frame->len);
            ESP_LOGI(TAG, "End of user function execution");
            if (bool_rs) {
                // Set the LED on for a short period of time.
                led_msg_t msg_to_led = {LED_MSG_START};
                BaseType_t rs = xQueueSend(*led_queue, &msg_to_led, 0);
                if (rs != pdTRUE) {
                    ESP_LOGE(TAG, "Error from xQueueSend: %d", rs);
                    goto exit_on_fatal_error;
                }
            }
            //
            esp_camera_fb_return(frame);
        }

    }

    exit_on_fatal_error:
    ESP_LOGI(TAG, "Exiting");

}
