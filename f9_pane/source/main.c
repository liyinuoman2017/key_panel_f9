/**
*********************************************************************************************************
*                                        multi_switch_control
*                                      (c) Copyright 2021-2031
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* 
*********************************************************************************************************
**/

/*
*********************************************************************************************************
Includes 
*********************************************************************************************************
*/
#include "string.h"
#include "user_type.h"
#include "business_scheduler.h"
#include "system_clock.h"
#include "data_business.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/

/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/

/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/


/***********************************************************************************************************
* @名称	: main
* @描述	: 配置MCU  运行关联业务   48MHZ  每秒轮询 约3000次
***********************************************************************************************************/
int32_t main(void)
{	
	/*MCU  48MHZ配置  看门狗  */
	system_clock_config();
	/*读取FLASH*/
	flash_data_read();
	/*业务初始化*/
	business_initialization();
	while(1)
	{
		/*业务调度*/
		business_scheduler();
		/*喂看门狗*/
		watchdog_feed();
	}	
}
/*************************************************END*******************************************************/



