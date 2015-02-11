/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"
#include "memstreams.h"
#include "evtimer.h"

icucnt_t last_width, last_period;

static void icuwidthcb(ICUDriver *icup) {

  last_width = icuGetWidth(icup);
}

static void icuperiodcb(ICUDriver *icup) {

  last_period = icuGetPeriod(icup);
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  10000,                                    /* 10kHz ICU clock frequency.   */
  icuwidthcb,
  icuperiodcb,
  NULL,
  ICU_CHANNEL_1,
  0
};

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }

  chprintf(chp, "serial ok!\n");
}

static void cmd_data(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: data\r\n");
    return;
  }

  chprintf(chp, "last_width = %i last_period = %i \n",last_width,last_period);
}

static const ShellCommand commands[] = {
  {"test", cmd_test},
  {"data", cmd_data},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands
};

static WORKING_AREA(waThread1, THREAD_WA_SIZE);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palTogglePad(GPIOA,13);
    chThdSleepMilliseconds(500);
  }
}


int main(void) {
  Thread *shelltp = NULL;

  halInit();
  chSysInit();


  palSetPadMode(GPIOA,9,PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  palSetPadMode(GPIOA,10,PAL_MODE_INPUT);
  sdStart(&SD1, NULL);

  shellInit();

  palSetPadMode(GPIOA,13,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(GPIOA,13);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  
  icuStart(&ICUD1, &icucfg);
  icuEnable(&ICUD1);

  while (TRUE) {
    if (!shelltp){
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);}
    else if (chThdTerminated(shelltp)) {
      chThdRelease(shelltp);
      shelltp = NULL;
    }
    chThdSleepMilliseconds(500);
  }
}
