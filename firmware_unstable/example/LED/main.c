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

unsigned char led_run=1;

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }

  chprintf(chp, "serial ok!\n");
}

static void cmd_led(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: led\r\n");
    return;
  }
  
  if(led_run==0){led_run=1;}
  else if(led_run==1){led_run=0;}
  chprintf(chp, "led toggled\r\n");
  
}

static const ShellCommand commands[] = {
  {"test", cmd_test},
  {"led",cmd_led},
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
	  if(led_run==1){
		  palTogglePad(GPIOA,13);
			palTogglePad(GPIOA,14);
			palTogglePad(GPIOA,15);
			palTogglePad(GPIOB,3);
			palTogglePad(GPIOB,4);
			palTogglePad(GPIOB,5);
			palTogglePad(GPIOB,6);
			palTogglePad(GPIOB,7);
		}
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
  palSetPadMode(GPIOA,14,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA,15,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,3,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,4,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,5,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,6,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,7,PAL_MODE_OUTPUT_PUSHPULL);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

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
