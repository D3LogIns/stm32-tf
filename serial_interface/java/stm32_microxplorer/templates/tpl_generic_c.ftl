[#ftl]
/*--------------------------------------------------------------*/
/* 						                                        */
/*--------------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
${mcu}
/* USER CODE BEGIN 1 */
hello world
/* USER CODE END 1 */
/*--------------------------------------------------------------*/
/* Configure GPIO                                               */
/*--------------------------------------------------------------*/
/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
void mx_pinout_config(void) {
[#list datas as ip]	

// Configure ${ip.targetName} 
	[#list ip.methods as method] 	
[#if method.valid]
[#if method.comment??]
[/#if]	 
	 [#assign args = ""]
	[#if method.arguments??]
			[#list method.arguments as fargument]
				[#if fargument.addressOf] [#assign adr = "&"]
				[#else ] [#assign adr = ""]
				[/#if]
				[#if fargument.genericType == "struct"]
					[#assign arg = "" + adr + fargument.name]
					[#list fargument.argument as argument][#compress]
${fargument.name}.${argument.name} = ${argument.value};[/#compress] 
[#-- //${(argument.comments)!}--][/#list]		 
				[#else][#assign arg = "" + adr + fargument.value][/#if]
				[#if args == ""][#assign args = args + arg ]
				[#else][#assign args = args + ', ' + arg][/#if]
			[/#list]
${method.name}(${args}); // ${method.comment}
	[/#if]
	[#else]	
//	The ${method.name} function is not generated; some parameters are missing!! 
	[/#if]
	[/#list]
[/#list]
}
/* USER CODE BEGIN 3 */
/* USER CODE END 3 */

