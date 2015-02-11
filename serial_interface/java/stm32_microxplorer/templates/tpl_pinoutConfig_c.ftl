[#ftl]
/**
  ******************************************************************************
  * File Name          : mx_gpio.c
  * Date               : [#list date as dt]${dt}[/#list]
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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

  [#compress]
/* Includes ------------------------------------------------------------------*/
#include "mx_gpio.h"

[/#compress]

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

[#compress]
void mx_pinout_config(void) {
#t/* Private typedef ---------------------------------------------------------*/
[#assign v = ""]
[#list datas as data]
	[#list data.variables as variable]				
			[#if v?contains(variable.name)]
				[#-- no matches--]
			[#else]
				[#list variable.value as val]#t${val}[/#list] ${variable.name};
				[#assign v = v + " "+ variable.name/]	
			[/#if]	
	[/#list]
[/#list]

[#list datas as data]
[#if data.comments??]
#n
#t/** ${data.comments}[/#if]
#t*/#n
		[#list data.methods as method][#assign args = ""]			
			[#if method.status=="OK"]	
[#if method.comment??]#n#t/*[#compress]${method.comment} */[/#compress][/#if]			
				[#if method.arguments??]
					[#list method.arguments as fargument][#compress]
						[#if fargument.addressOf] [#assign adr = "&"][#else ][#assign adr = ""][/#if][/#compress]
						[#if fargument.genericType == "struct"][#assign arg = "" + adr + fargument.name]											
							[#list fargument.argument as argument]	
							
[#if argument.mandatory]							
#t${fargument.name}.${argument.name} = ${argument.value};
[/#if]
[/#list][#else][#assign arg = "" + adr + fargument.value][/#if]
					[#if args == ""][#assign args = args + arg ][#else][#assign args = args + ', ' + arg][/#if]
[/#list]
#t${method.name}(${args});

				[#else]
#t//${method.comment}[/#if]			
		[/#if]
		[#if method.status=="KO"]
		#n
#t//!!! ${method.name} is commented because some parameters are missing
			[#if method.arguments??]			
				[#list method.arguments as fargument]
					[#if fargument.addressOf] [#assign adr = "&"][#else ] [#assign adr = ""][/#if]
					[#if fargument.genericType == "struct"][#assign arg = "" + adr + fargument.name]
						[#list fargument.argument as argument]	
[#if argument.mandatory]						
#t//${fargument.name}.${argument.name} = ${argument.value}; 
[/#if]
		 				[/#list][#else][#assign arg = "" + adr + fargument.value][/#if]
					[#if args == ""][#assign args = args + arg ]
					[#else][#assign args = args + ', ' + arg][/#if][/#list]
#t//${method.name}(${args});[/#if]
[/#if][/#list][/#list]
}
[/#compress]


/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
