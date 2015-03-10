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

#define _lcd_stream_data _base_sequential_stream_data

#define LCD_PIN_RS 10
#define LCD_PIN_RW 11
#define LCD_PIN_EN 12
#define LCD_PORT_CRTL GPIOC

#define LCD_PIN_D4 4
#define LCD_PIN_D5 5
#define LCD_PIN_D6 6
#define LCD_PIN_D7 7
#define LCD_PORT_DATA GPIOB

#define LCD_PORT_MODE PAL_MODE_OUTPUT_PUSHPULL

#define LCD_2X16_CGRAM_ADDR 0x40
#define LCD_2X16_DDRAM_ADDR 0x80
#define LCD_2X16_NEXT_LINE  0x40

struct LcdStreamVMT {
    _base_sequential_stream_methods
};

typedef struct {
    const struct LcdStreamVMT *vmt;
    _base_sequential_stream_data
} LcdStream;

#ifdef __cplusplus
extern "C" {
#endif
void lsObjectInit(LcdStream *msp);
#ifdef __cplusplus
}
#endif

LcdStream myLCD;

void Lcd_Write_Data(uint8_t chr){
    palWritePort(LCD_PORT_DATA,(chr & 0xf0));
    palSetPad(LCD_PORT_CRTL,LCD_PIN_RS);
    palSetPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMicroseconds(40);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_EN);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_RS);
    chThdSleepMicroseconds(40);

    palWritePort(LCD_PORT_DATA,((chr & 0x0f)<<4));
    palSetPad(LCD_PORT_CRTL,LCD_PIN_RS);
    palSetPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMicroseconds(40);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_EN);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_RS);
    chThdSleepMicroseconds(40);
}

static void Lcd_Write_Command(uint8_t cmd){
    palWritePort(LCD_PORT_DATA,(cmd & 0xf0));
    palSetPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMicroseconds(40);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMicroseconds(40);

    palWritePort(LCD_PORT_DATA,((cmd & 0x0f)<<4));
    palSetPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMicroseconds(40);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMicroseconds(40);
}

static msg_t put(void *ip, uint8_t chr) {
    (void)ip;
    Lcd_Write_Data(chr);
    return RDY_OK;
}

static const struct LcdStreamVMT vmt = {NULL, NULL, put, NULL};
void lsObjectInit(LcdStream *msp) {
    msp->vmt = &vmt;
}

static void Lcd_Pin_Dir(void){
    palSetPadMode(LCD_PORT_CRTL,LCD_PIN_RS,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_CRTL,LCD_PIN_RW,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_CRTL,LCD_PIN_EN,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D4,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D5,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D6,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D7,LCD_PORT_MODE);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_RW);
}

static void Lcd_Cursor(uint8_t column, uint8_t line){
    uint8_t position = LCD_2X16_DDRAM_ADDR;
    if (line > 0)
    {
    position |= LCD_2X16_NEXT_LINE;
    }
    Lcd_Write_Command(position | column);
}

static void Lcd_Init(void){
    lsObjectInit(&myLCD);
    Lcd_Pin_Dir();

    chThdSleepMilliseconds(500);
    palWritePort(LCD_PORT_CRTL,0x00);
    palWritePort(LCD_PORT_DATA,0x00);

    palSetPad(LCD_PORT_DATA,LCD_PIN_D5);
    palSetPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMilliseconds(40);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_EN);
    chThdSleepMilliseconds(40);

    Lcd_Write_Command(0x28);
    chThdSleepMicroseconds(40);
    Lcd_Write_Command(0x0c);
    chThdSleepMicroseconds(40);
}

static void Lcd_Clear (void){
    Lcd_Write_Command(0x01);
    chThdSleepMicroseconds(40);
}

static void Lcd_Example(void){
    Lcd_Clear();
    Lcd_Cursor(0,0);
    chprintf((BaseSequentialStream *)&myLCD,"STM32");
    Lcd_Cursor(0,1);
    chprintf((BaseSequentialStream *)&myLCD,"TF ITS");
}


static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }

  chprintf(chp, "serial ok!\n");
}

static void cmd_lcd(BaseSequentialStream *chp, int argc, char *argv[]) {

  if (argc != 2) {
    chprintf(chp, "Usage: lcd [baris] [kata_max_16] \r\n");
    return;
  }

  unsigned int baris;
  baris = atoi(argv[0]);
  Lcd_Cursor(0,baris);
  chprintf((BaseSequentialStream *)&myLCD,"                ");
  Lcd_Cursor(0,baris);
  chprintf((BaseSequentialStream *)&myLCD,argv[1]);

}


static const ShellCommand commands[] = {
  {"test", cmd_test},
  {"lcd",cmd_lcd},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands
};

static WORKING_AREA(waThread2, THREAD_WA_SIZE);
static msg_t Thread2(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker2");
  while (TRUE) {

    palClearPad(GPIOC,0);
    palClearPad(GPIOC,1);
    palClearPad(GPIOC,2);
    palClearPad(GPIOC,3);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOC,0);
    palSetPad(GPIOC,1);
    palSetPad(GPIOC,2);
    palSetPad(GPIOC,3);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

static WORKING_AREA(waThread1, THREAD_WA_SIZE);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {

    palClearPad(GPIOA,8);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOA,8);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

int main(void) {
  Thread *shelltp = NULL;

  halInit();
  chSysInit();

  palSetPadMode(GPIOC,0,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,1,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,2,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,3,PAL_MODE_OUTPUT_PUSHPULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  palSetPadMode(GPIOA,9,PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  palSetPadMode(GPIOA,10,PAL_MODE_INPUT);
  sdStart(&SD1, NULL);

  shellInit();

  palSetPadMode(GPIOA,8,PAL_MODE_OUTPUT_PUSHPULL);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  Lcd_Init();
  Lcd_Clear();
  Lcd_Example();

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
