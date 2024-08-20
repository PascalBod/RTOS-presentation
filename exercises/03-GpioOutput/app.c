/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "app_log.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "sl_sleeptimer.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{

  // To let us know which application we are running.
  app_log("03-GpioOutput - v0.1.0\n");

  // Enable GPIO-block clock.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // TODO: configure the GPIO pin for controlling the red LED.

  // Display tick/ms ratio.
  uint64_t ms;
  uint64_t ticks = 32768;
  sl_status_t rs = sl_sleeptimer_tick64_to_ms(ticks, &ms);
  if (rs != SL_STATUS_OK) {
      app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lu\n", rs);
      return;
  }
  // Displaying uint64_t values requires %lld. But %lld is only supported
  // by the Base C Library, while the default library is Nano C Library.
  // To stay with it, we consider that the two values are using less
  // than 32 bits (%ld).
  app_log("ms for %ld ticks: %ld\n", (uint32_t)ticks, (uint32_t)ms);

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  // We may ignore time value wrapping, as it will occur after around
  // 17.8 millions of years (with 32768 ticks for 1 s). The number of
  // ticks per second is provided by the code in app_init above.
  uint64_t ticks = sl_sleeptimer_get_tick_count64();
  uint64_t ms;
  sl_status_t rs = sl_sleeptimer_tick64_to_ms(ticks, &ms);
  if (rs != SL_STATUS_OK) {
      app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lu\n", rs);
      return;
  }

  // TODO: make the LED blink every 250 ms by inserting here some code.

}
