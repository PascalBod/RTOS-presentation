#ifndef MAIN_BUTTON_H_
#define MAIN_BUTTON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Must be called before the button task is started.
void button_task_set_main_queue(QueueHandle_t *m_queue);

void button_task(void *taskParameters);

#endif /* MAIN_BUTTON_H_ */
