[#ftl]
/**
  ****************************************************************************
  * File Name          : main.h
  * Date               : [#--list date as dt]${dt}[/#list]--]
  * Description        : 
  ******************************************************************************
  *
  * COPYRIGHT [#list year as y]${y}[/#list] STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#include "stm32f2xx.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/* Mcu Initialization --------------------------------------------------------*/
[#list datas as void]
${void}();
[/#list]

/* Pinout Configure */
  mx_pinout_config();
  
/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
  /* Infinite loop */
  while (1)
  {

  }
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */  
}
/**
  * @}
  */ 
/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/