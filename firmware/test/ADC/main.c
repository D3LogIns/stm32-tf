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

#define ADC_GRP1_NUM_CHANNELS   3
#define ADC_GRP1_BUF_DEPTH      10

static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

adcsample_t adc0,adc1,adc2;
uint32_t sum_adc0,sum_adc1,sum_adc2;

void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n){
  (void) buffer; (void) n;
  int i;
  if (adcp->state == ADC_COMPLETE) {
	  
    sum_adc0=0;
    sum_adc1=0;
    sum_adc2=0;
	
    for(i=0;i<ADC_GRP1_BUF_DEPTH;i++){
		sum_adc0=sum_adc0+samples[0+(i*ADC_GRP1_NUM_CHANNELS)];
		sum_adc1=sum_adc1+samples[1+(i*ADC_GRP1_NUM_CHANNELS)];
		sum_adc2=sum_adc2+samples[2+(i*ADC_GRP1_NUM_CHANNELS)];
    }
    
    adc0=sum_adc0/ADC_GRP1_BUF_DEPTH;
	adc1=sum_adc1/ADC_GRP1_BUF_DEPTH;
	adc2=sum_adc2/ADC_GRP1_BUF_DEPTH;
   }
 }
 
static const ADCConversionGroup adcgrpcfg = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  adccb,
  NULL,
  /* HW dependent part.*/
  0,
  0,
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_239P5) | ADC_SMPR2_SMP_AN1(ADC_SAMPLE_239P5) | ADC_SMPR2_SMP_AN2(ADC_SAMPLE_239P5),
  0,
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN1) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN2)
};

static WORKING_AREA(wa_adcThread, THREAD_WA_SIZE);
static msg_t adcThread(void *arg) {
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(100);
    adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);
  }
  return 0;
}

static void cmd_adc(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: adc\r\n");
    return;
  }
  
  chprintf(chp,"%4d,%4d,%4d\n",adc0,adc1,adc2);
}

static const ShellCommand commands[] = {
  {"adc",cmd_adc},
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

    palClearPad(GPIOA,8);
    chThdSleepMilliseconds(250);
    palSetPad(GPIOA,8);
    chThdSleepMilliseconds(250);
  }
  return 0;
}

int main(void) {

  halInit();
  chSysInit();
  
  Thread *shelltp = NULL;

  palSetPadMode(GPIOA,9,PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  palSetPadMode(GPIOA,10,PAL_MODE_INPUT);
  sdStart(&SD1, NULL);

  palSetPadMode(GPIOA,8,PAL_MODE_OUTPUT_PUSHPULL);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  
  palSetPadMode(GPIOA,0,PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA,1,PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA,2,PAL_MODE_INPUT_ANALOG);
  adcStart(&ADCD1, NULL);
  chThdCreateStatic(wa_adcThread, sizeof(wa_adcThread), NORMALPRIO, adcThread, NULL);

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
