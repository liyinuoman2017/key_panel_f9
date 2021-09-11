/**
*********************************************************************************************************
*                                        multi_switch_control
*                                      (c) Copyright 2021-2031
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* BSP层只处理简单的IO操作，不负责逻辑控制
*********************************************************************************************************
**/

/*
*********************************************************************************************************
Includes 
*********************************************************************************************************
*/
#include "user_type.h"
#include "gpio.h"
#include "indicator_bsp_hc32l130.h"


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
static u8 read(data_stream_t* stream);
static u8 write(data_stream_t* stream);
static u8 initialization(data_stream_t* stream);
static u8 interrput(data_stream_t* stream);
static u8 run(data_stream_t* stream);
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
application_template_t indicator_bsp_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,	
};

/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 read(data_stream_t* stream)
{

	return 0;
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 write(data_stream_t* stream)
{
	switch(stream->buff[0])
	{
		case LED_1:
			LED_OUT1=!stream->buff[1];
        break;
		case LED_2:
			LED_OUT2=!stream->buff[1];
        break;
		case LED_3:
			LED_OUT3=!stream->buff[1];
        break;
		case LED_4:
			LED_OUT4=!stream->buff[1];
        break;	
		case LED_5:
			LED_OUT5=!stream->buff[1];
        break;	
		case LED_6:
			LED_OUT6=!stream->buff[1];
        break;			
		default:
		break;
	}	
	return 0;
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 initialization(data_stream_t* stream)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    ///< GPIO IO USER LED初始化 
    Gpio_Init(GpioPortB, GpioPin12, &stcGpioCfg); // LED1
    Gpio_Init(GpioPortB, GpioPin13, &stcGpioCfg); // LED2
    Gpio_Init(GpioPortB, GpioPin14, &stcGpioCfg); // LED3
    Gpio_Init(GpioPortB, GpioPin15, &stcGpioCfg); // LED4
    Gpio_Init(GpioPortA, GpioPin11, &stcGpioCfg); // LED5
    Gpio_Init(GpioPortA, GpioPin12, &stcGpioCfg); // LED6	
	
	/*关闭LED*/
    Gpio_SetIO(GpioPortB, GpioPin12); //LED4
    Gpio_SetIO(GpioPortB, GpioPin13); //LED3
    Gpio_SetIO(GpioPortB, GpioPin14); //LED2
    Gpio_SetIO(GpioPortB, GpioPin15); //LED1
    Gpio_SetIO(GpioPortA, GpioPin11); //LED2
    Gpio_SetIO(GpioPortA, GpioPin12); //LED1	
	
	return 0;
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 interrput(data_stream_t* stream)
{
	
return 0;
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 run(data_stream_t* stream)
{	
	return 0;
}
/***********************************************END*****************************************************/

