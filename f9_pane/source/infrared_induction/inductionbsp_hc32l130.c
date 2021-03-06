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


application_template_t induction_bsp_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
};

/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 read(data_stream_t* stream)
{
	stream->hex[0] = Gpio_GetInputIO(GpioPortB,GpioPin7);
	return stream->hex[0];
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 write(data_stream_t* stream)
{
	
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
    ///< 端口方向配置->输入
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY初始化 
	
	Gpio_Init(GpioPortB, GpioPin7, &stcGpioCfg);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	
	return 0;
}

/***********************************************END*****************************************************/

