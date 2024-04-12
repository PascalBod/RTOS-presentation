#ifndef MAIN_CAMERA_H_
#define MAIN_CAMERA_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Must be called before the camera task is started.
void camera_task_set_queue(QueueHandle_t *i_queue);

// Must be called before the camera task is started.
void camera_task_set_frame_queue(QueueHandle_t *f_queue);

void camera_task(void *taskParameters);

#endif /* MAIN_CAMERA_H_ */
