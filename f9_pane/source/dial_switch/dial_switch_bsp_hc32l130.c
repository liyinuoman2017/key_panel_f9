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
#include "dial_switch_bsp_hc32l130.h"


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
#define INPUT_NUM 6
static en_gpio_pin_t uesr_pin[INPUT_NUM]=
{
	GpioPin0,
	GpioPin1,
	GpioPin2,
	GpioPin3,
	GpioPin4,
	GpioPin5,	
};

static en_gpio_port_t uesr_port[INPUT_NUM] =
{
	GpioPortB,
	GpioPortB,
	GpioPortB,
	GpioPortB,	
	GpioPortB,
	GpioPortB,
};
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
/* BUFF[0]为指令中的操作码 */
static u8 read(data_stream_t* stream);
static u8 write(data_stream_t* stream);
static u8 initialization(data_stream_t* stream);
static u8 interrput(data_stream_t* stream);

/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
application_template_t dial_switch_bsp_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
};

/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 read(data_stream_t* stream)
{
	u8 dial_value;

	/*面板拨码引脚乱序*/
	dial_value = 32 * PB_IDR_IDR5 + 16 * PB_IDR_IDR4 + 8 * PB_IDR_IDR3 + 4 * PB_IDR_IDR2 + 2 * PB_IDR_IDR1 +  PB_IDR_IDR0;
	dial_value = (~dial_value) & 0x3f;
	
	stream->buff[0] = dial_value;
	return 0;
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
******************************************/
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
    stcGpioCfg.enPu = GpioPuEnable;
    stcGpioCfg.enPd = GpioPdDisable;	


    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER 拨码初始化 
	for(u8 i = 0 ; i < 6 ; i++)
		Gpio_Init(uesr_port[i], uesr_pin[i], &stcGpioCfg);
	
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

/***********************************************END*****************************************************/

