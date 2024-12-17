# Exercise 12 - Shared structure bug

## Purpose of the exercise

Demonstrate a "strange" behavior when three tasks share some global variables.

## Instructions

Import the `12-Shared-structure-bug` project. Look at the source code. In a few words:
* There are three tasks
* An initial call to `vTaskDelay` makes each task loop start in the following order: task3 starts immediately, then task2 starts, then task1 starts
* The three tasks share the `a` variable
* Task1 performs the following tests:

```
if (a == 1) {
  // Some processing here...
  b = a;
}
if (b != 1) {
  app_log_warning("b value: %lu\n", b);
}
```

* `a` is initialized to `1` by task3. Then, task3 periodically sets `a` to `1`
* Task2 periodically sets `a` to `2`

As `a` is initialized to `1` and `b` is set to the value of `a` only when `a` contains `1`, it seems that `b` should always contain `1`.

Build and run the application, and check the output. What do you get? Can you find a reason for the exhibited behavior?


