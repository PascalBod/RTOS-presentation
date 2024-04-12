#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "esp_log.h"

static const char TAG[] = "FPN";

// Structure used to access a four-byte piece of data either as a floating point
// number or as an integer.
typedef union {
	float f;
	uint32_t i;
} float_to_num_t;

void app_main(void)
{

	float_to_num_t ftn;

	ftn.f = 1.0;
	ESP_LOGI(TAG, "Size of float: %u", sizeof(float));
	ESP_LOGI(TAG, "Float value in hexadecimal: 0x%04lx", ftn.i);

	ftn.f = 2.0;
	ESP_LOGI(TAG, "Float value in hexadecimal: 0x%04lx", ftn.i);

	ftn.f = pow(2, 10);
	ESP_LOGI(TAG, "Float value in hexadecimal: 0x%04lx", ftn.i);

	ftn.f += 5.1;
	ESP_LOGI(TAG, "Float value in hexadecimal: 0x%04lx", ftn.i);

}
