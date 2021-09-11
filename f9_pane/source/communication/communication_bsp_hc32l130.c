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
#include "communication_bsp_hc32l130.h"
#include "uart.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/	
#define USART_CH  						(M0P_UART1)
#define USART_BAUDRATE                  (9600)

#define USART_RX_PORT                   (GpioPortA)
#define USART_RX_PIN                    (GpioPin3)

#define USART_TX_PORT                   (GpioPortA)
#define USART_TX_PIN                    (GpioPin2)


#define DIR_IN    0
#define DIR_OUT   1

#define TR_PORT GpioPortA
#define TR_PIN 	GpioPin0

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
/* BUFF[0]为指令中的操作码 */
static u8 read(data_stream_t* stream);
static u8 write(data_stream_t* stream);
static u8 initialization(data_stream_t* stream);
static u8 interrput(data_stream_t* stream);


application_template_t communication_bsp_operation=
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

	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 低电平接收，高电平发送，默认为接收模式
***********************************************************************************************************/
void rs485_direction(u8 direction)
{
    //如果DIR_OUT则为输出模式，PA1为高电平
    if (direction == DIR_OUT)
    {
        Gpio_SetIO(TR_PORT, TR_PIN);
    }
    //DIR_IN=0，则为输入模式，PA1为低电平
    else
    {
        Gpio_ClrIO(TR_PORT, TR_PIN);
    }
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 write(data_stream_t* stream)
{
    u8 k = 0;
	Uart_DisableIrq(USART_CH, UartRxIrq);
    rs485_direction(DIR_OUT);
	delay(10);
    for (k = 0; k < stream->cmd; k++) //循环发送数据
    {
        Uart_SendDataPoll(USART_CH, stream->buff[k]); //循环发送一个字节一个字节的发
    }
	rs485_direction(DIR_IN);
	delay(10);
	Uart_ClrStatus(USART_CH, UartRC);       
	Uart_EnableIrq(USART_CH, UartRxIrq); //使能接收中断
	
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void uart_gpio_init(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //使能GPIO模块时钟
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(USART_TX_PORT, USART_TX_PIN, &stcGpioCfg);
    Gpio_SetAfMode(USART_TX_PORT, USART_TX_PIN, GpioAf1);          //配置PA2 端口为URART1_TX
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(USART_RX_PORT, USART_RX_PIN, &stcGpioCfg);
    Gpio_SetAfMode(USART_RX_PORT, USART_RX_PIN, GpioAf1);          //配置PA10 端口为URART1_RX
    ///< 端口方向配置->输出
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    Gpio_Init(TR_PORT, TR_PIN, &stcGpioCfg); 
 	Gpio_ClrIO(TR_PORT, TR_PIN);                 //485 EN
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void uart_port_init(void)
{
    stc_uart_cfg_t    stcCfg;
    DDL_ZERO_STRUCT(stcCfg);
    ///< 开启外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);///<使能uart1模块时钟
    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;          ///<模式3
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bit停止位
    stcCfg.enMmdorCk        = UartMskEven;           ///<偶检验
    stcCfg.stcBaud.u32Baud  = USART_BAUDRATE;        ///<波特率9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<通道采样分频配置
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<获得外设时钟（PCLK）频率值		
    Uart_Init(USART_CH, &stcCfg);                    ///<串口初始化
    ///<UART中断使能
    Uart_ClrStatus(USART_CH,UartRC);                 ///<清接收请求
    Uart_EnableIrq(USART_CH,UartRxIrq);              ///<使能串口接收中断
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);         ///<系统中断使能
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	uart_gpio_init();
	uart_port_init();	
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

