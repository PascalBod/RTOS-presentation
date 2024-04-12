# Button and LED tasks

This application uses two tasks: the *btn_task* and the *led_task*.

The btn_task detects button pushes. The led_task is in charge of making the LED blink or stop blinking.

When the btn_task detects two successive button pushes in less than one second, it informs the led_task. If the LED was blinking, the led_task stops it. Otherwise, it makes it blink.