# Skeleton app for insertion of inference code

## Environment

The target board is the ESP-EYE board.

The development environment is ESP-IDF version 4.4.6. With the ESP-EYE board as target, do not use ESP-IDF 5.0.

## Functional point of view

This sample code provides the following skeleton application, at a functional point of view:
* When the user clicks on the side button, the camera starts returning a stream of pictures
* Every picture is sent to a function provided by the user. This function may perform some processing on the picture
* When the user function is done, it can call a function provided by the skeleton, which will set the detection LED on for a short period of time

## Technical point of view

The architecture of the skeleton application relies on the following elements:
* Several tasks are running
* A first task, named the *button task*, is in charge of detecting a press on the side button
* A second task, named the *led task*, is in charge of controlling the detection LED
* A third task, named the *camera task*, is in charge of starting and stopping the camera component provided by ESP-WHO
* A fourth task, named the *frame task*, receives the camera frames provided by the camera component, and calls the user function
* A fifth task, named the *main task*:
  * Waits for a message from the button task, which tells it that the button has been pressed
  * Tells the camera task to start the camera
  * Waits for a second press on the button, which would indicate that the camera has to be stopped
* The led task waits for a message from the frame task, which tells it to set the LED on for a short period of time

Commnunication between tasks is implemented with message queues.

The following picture depicts inter-task communication:

```
+--------+                                                        +------------+
| button |                                          . . . . . . . | esp-camera |
+--------+                                          .             +------------+
    |                                               .                    |
    |  BUTTON_PRESSED    +------+  START/STOP  +--------+                | FRAME
    +------------------->| main |------------->| camera |                |
                         +------+              +--------+                v
                                                                     +-------+
                                                                     | frame |
                                                                     +-------+
                                                                         |
                                                                         | ON
                                                                         |
                                                                         v
                                                                      +-----+
                                                                      | led |
                                                                      +-----+
```

If the user function preempts a core for too long a period, ESP-IDF core watchdog will reset the application. The current watchog timeout period is set to 5 seconds. It can be increased with in *sdkconfig*: **Component Config > ESP System Settings > Task Watchdog imeout period**.

## Project configuration

The project reuses components provided by [ESP-WHO](https://github.com/espressif/esp-who). If ESP-WHO is not installed yet, download it:

```shell
$ git clone --recursive https://github.com/espressif/esp-who.git
```

Then, configure *skeletonApp-camera* by modifying the top `CMakeLists.txt` file in order to adapt the definition of `EXTRA_COMPONENT_DIRS` to your ESP-WHO installation:

```
# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS /home/developer/DevTools/esp-who/components)  # <== adapt directory path
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})
```

## How the project was created

The project was created using `esp_who/examples/human_face_detection/web` as a model, with the following steps:
* Create an empty ESP-IDF project
* In `CMakeLists.txt` file, add the following line (adapt as required):

```
set(EXTRA_COMPONENT_DIRS /home/developer/DevTools/esp-who/components)
```

* Copy the `/home/developer/DevTools/esp-who/examples/human_face_detection/web/sdkconfig.defaults.esp32` file to `skeletonApp-camera` directory
* Add the following lines to this file, taken from the `sdkconfig.defaults` file of the above human face detection sample project:

```
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y

CONFIG_ESPTOOLPY_FLASHFREQ_80M=y
CONFIG_ESPTOOLPY_FLASHMODE_QIO=y

CONFIG_SPIRAM_SPEED_80M=y

CONFIG_ESP_TASK_WDT=n

# Wait for esp-idf stable
CONFIG_BOOTLOADER_COMPILER_OPTIMIZATION_PERF=y
CONFIG_BOOTLOADER_LOG_LEVEL_NONE=y
CONFIG_SPIRAM_MEMTEST=n
```

`sdkconfig` is augmented by some ESP-WHO menus which do not come from the above import. 

The type of the board must be selected (**Component Config > ESP-WHO Configuration**).

SPI RAM (PSRAM) must be enabled, with **Component Config > ESP32-specific > Support for external, SPI-connected RAM**
