#ifndef MAIN_LED_H_
#define MAIN_LED_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Must be called before the led task is started.
void led_task_set_queue(QueueHandle_t *i_queue);

void led_task(void *taskParameters);

#endif /* MAIN_LED_H_ */
