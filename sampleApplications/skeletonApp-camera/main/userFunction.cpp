#include <chrono>
#include <iostream>
#include <thread>

#include "esp_log.h"

#include "userFunction.hpp"

static uint32_t counter = 0;

static const char TAG[] = "USR";

// Return true to set the LED on for a short period of time (see led task).
extern "C" bool userFunction(uint8_t *picture, uint32_t size) {

    using namespace std::this_thread;
    using namespace std::chrono;

    std::cout << "This is the user function - picture size: " << size << std::endl;

    // ESP-IDF log macros can be used.
    ESP_LOGI(TAG, "test");

    // Check the READMe file for information about how to configure the
    // task timeout period, in case userFunction takes too long to execute.

    // Note: the user function must be ready to cope with inconsistent arguments,
    // because of a race condition: check comment in stop_who_camera_task, in
    // camera.c.

    // Here, we simulate an execution time of 1.5 s.
    sleep_for(milliseconds(1500));

    // We simulate one success every ten executions.
    counter++;
    if (counter % 10 == 0) {
        return true;
    } else {
        return false;
    }

}
