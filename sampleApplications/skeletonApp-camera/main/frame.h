#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Must be called before the frame task is started.
void frame_task_set_queue(QueueHandle_t *i_queue);

// Must be called before the frame task is started.
void frame_task_set_led_queue(QueueHandle_t *l_queue);

void frame_task(void *taskParameters);

#endif /* MAIN_FRAME_H_ */
