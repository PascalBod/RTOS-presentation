#include <stdbool.h>
#include <unistd.h>

#include "btn_task.h"
#include "common.h"
#include "led_task.h"

#define SLEEP_PERIOD_US 11000

void app_main(void)
{

	// Initialize communication flags.
	push_event = false;

	btn_task_init();
	led_task_init();

	while (true) {

		// If we don't sleep a little, ESP-IDF considers there is a problem:
		// a watchdog resets the microcontroller. Minimum period is 10 ms.
		usleep(SLEEP_PERIOD_US);

		btn_task_run();
		led_task_run();

	}

}
