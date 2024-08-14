# Exercise 01 - Hello World

## Purpose of the exercise

Display a message on the virtual serial link created over the USB connection.

## A few words about EFR32BG24 libraries

Many libraries are provided by Silicon Labs for helping the developer in writing code. The libraries contain abstraction code implementing a "high-level" API (Application Programming Interface) to the various microcontroller blocks: peripherals, interfaces, etc. (more information on this topic later in the presentation).

For Simplicity Studio, these libraries are provided as "software components" which can be added to a project in a simple way.

There are pros and cons to this architecture. It's easy to add, configure and remove software support to a given interface or peripheral. On the other hand, it can be sometimes difficult to understand exactly how a library works. 

## The exercise

1. Ensure you completed the [prerequisites](https://github.com/PascalBod/RTOS-presentation?tab=readme-ov-file#prerequisites-for-the-practical-exercises).
2. Create a new project, still based on the **Empty C project**, naming it `01-HelloWorld`.
3. Check that you can build it and that you can program the board.
4. Replace the content of the `readme.md` file (right-click and **Open With > Text Editor**) by:
    ```
    # HelloWorld
    
    This example project shows how to display a message on the
    virtual serial link created over the USB connection.          
    ```
5. If the content of the `01-HelloWorld.slcp` file is not displayed in an editor tab yet, double-click the filename in the Project Explorer view.
6. Click on the **SOFTWARE COMPONENTS** tab.
7. Using the search box, install the following components, in this order:
   * **Services > IO Stream > IO Stream: STDLIB Configuration**
   * **Services > IO Stream > Driver > IO Stream: USART**
   * **Services > IO Stream > IO Stream: Retarget STDIO**
   * **Application > Utility > Log**

Notes:
* Only the part of the string after the last **>** character must be used to search.
* When the search result is displayed, click the small triangles on the left-hand side until the result is fully unfolded. Then, click the **Install** button.
* For the USART component, keep the default name for the instance.

8. Double-click the `app.c` file in order to display its content.
9. Add the following include directive:
    ```
    #include "app_log.h"
    ```
10. Add the following function call in the `app_init` function:
    ```
        app_log("Hello World!\n");
    ```
12. Save the file.
13. Build the application, and program the board.
14. In a terminal window, start a serial terminal connected to the board:
    ```
    $ pyserial-miniterm /dev/ttyACM0 115200
    ```
15. Click the board reset button. Each click should display the `Hello World!` message.
