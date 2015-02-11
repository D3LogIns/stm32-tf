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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32VL-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_PELATIHAN_JOMBLO
#define BOARD_NAME              "PELATIHAN JOMBLO"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F10X_LD_VL

/*
 * IO pins assignments.
 */

#define VAL_GPIOACRL            0x88888888    
#define VAL_GPIOACRH            0x88888888     
#define VAL_GPIOAODR            0xFFFFFFFF

#define VAL_GPIOBCRL            0x88888888    
#define VAL_GPIOBCRH            0x88888888     
#define VAL_GPIOBODR            0xFFFFFFFF

#define VAL_GPIOCCRL            0x88888888    
#define VAL_GPIOCCRH            0x88888888     
#define VAL_GPIOCODR            0xFFFFFFFF

#define VAL_GPIODCRL            0x88888888    
#define VAL_GPIODCRH            0x88888888     
#define VAL_GPIODODR            0xFFFFFFFF

#define SHELL_WA_SIZE   THD_WA_SIZE(256)
#define THREAD_WA_SIZE  64

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
