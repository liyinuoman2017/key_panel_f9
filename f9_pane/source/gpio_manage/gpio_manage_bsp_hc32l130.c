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
#include "gpio_manage_bsp_hc32l130.h"

/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/

static en_gpio_pin_t button_pin[BUTTON_NUM]=
{
	GpioPin8,
	GpioPin9,
	GpioPin12,
	GpioPin13,
	GpioPin14,
	GpioPin15,
	GpioPin8,
	GpioPin11	
};

static en_gpio_port_t button_port[BUTTON_NUM] =
{
	GpioPortB,
	GpioPortB,
	GpioPortB,
	GpioPortB,	
	GpioPortB,
	GpioPortB,
	GpioPortA,
	GpioPortA,	
};


static en_gpio_pin_t output_pin[OUTPUT_NUM]=
{
	GpioPin7,
	GpioPin4,
	GpioPin5,
	GpioPin6,
	
};

static en_gpio_port_t output_port[OUTPUT_NUM] =
{
	GpioPortD,
	GpioPortA,
	GpioPortA,
	GpioPortA,	
	
};


static en_gpio_pin_t dial_pin[DIAL_SWITCH_NUM]=
{
	GpioPin0,
	GpioPin1,
	GpioPin2,
	GpioPin3,
	GpioPin4,
	GpioPin5,	
};

static en_gpio_port_t dial_port[DIAL_SWITCH_NUM] =
{
	GpioPortB,
	GpioPortB,
	GpioPortB,
	GpioPortB,	
	GpioPortB,
	GpioPortB,
};
#define PB_IDR_IDR5 (M0P_GPIO->PBIN_f.PB00)
#define PB_IDR_IDR4 (M0P_GPIO->PBIN_f.PB01)
#define PB_IDR_IDR3 (M0P_GPIO->PBIN_f.PB02)
#define PB_IDR_IDR2 (M0P_GPIO->PBIN_f.PB03)
#define PB_IDR_IDR1 (M0P_GPIO->PBIN_f.PB04)
#define PB_IDR_IDR0 (M0P_GPIO->PBIN_f.PB05)
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
* @名称	: 
* @描述	: 
***********************************************************************************************************/
u8 input_read(u8 channel)
{
	u8 ret = 0;
	if(channel <BUTTON_NUM)
	{
		ret = Gpio_GetInputIO(button_port[channel], button_pin[channel]);
	}
	return ret;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
u8 dial_read(void)
{
	u8 dial_value = 0;

	/*面板拨码引脚乱序*/
	dial_value = 32 * PB_IDR_IDR5 + 16 * PB_IDR_IDR4 + 8 * PB_IDR_IDR3 + 4 * PB_IDR_IDR2 + 2 * PB_IDR_IDR1 +  PB_IDR_IDR0;
	dial_value = (~dial_value) & 0x3f;
	
	return dial_value;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void output_write(u8 channel , u8 data)
{
	if(channel <OUTPUT_NUM)
		Gpio_WriteOutputIO(output_port[channel], output_pin[channel] ,data);
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void input_init(void)
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
    ///< GPIO IO USER KEY初始化 
	for(u8 i = 0 ; i < BUTTON_NUM ; i++)
		Gpio_Init(button_port[i], button_pin[i], &stcGpioCfg);
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void output_init(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuEnable;
    stcGpioCfg.enPd = GpioPdEnable;
    ///< GPIO IO USER KEY初始化 
	for(u8 i = 0 ; i < OUTPUT_NUM ; i++)
		Gpio_Init(output_port[i], output_pin[i], &stcGpioCfg);
    ///< OUPUT 0	
	for(u8 i = 0 ; i < OUTPUT_NUM ; i++)
		Gpio_WriteOutputIO(output_port[i], output_pin[i], 0);	
	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void dial_switch_init(void)
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
	for(u8 i = 0 ; i < DIAL_SWITCH_NUM ; i++)
		Gpio_Init( dial_port[i], dial_pin[i], &stcGpioCfg);	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void gpio_initialization(void)
{
	/*按键输入初始化*/
	input_init();
	/*控制输出初始化*/	
	output_init();
	/*拨码输入*/
	dial_switch_init();
}
/***********************************************END*****************************************************/

