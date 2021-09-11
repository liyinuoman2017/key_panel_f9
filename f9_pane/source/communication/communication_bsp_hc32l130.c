/**
*********************************************************************************************************
*                                        multi_switch_control
*                                      (c) Copyright 2021-2031
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* BSP��ֻ����򵥵�IO�������������߼�����
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
/* BUFF[0]Ϊָ���еĲ����� */
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
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 read(data_stream_t* stream)
{

	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: �͵�ƽ���գ��ߵ�ƽ���ͣ�Ĭ��Ϊ����ģʽ
***********************************************************************************************************/
void rs485_direction(u8 direction)
{
    //���DIR_OUT��Ϊ���ģʽ��PA1Ϊ�ߵ�ƽ
    if (direction == DIR_OUT)
    {
        Gpio_SetIO(TR_PORT, TR_PIN);
    }
    //DIR_IN=0����Ϊ����ģʽ��PA1Ϊ�͵�ƽ
    else
    {
        Gpio_ClrIO(TR_PORT, TR_PIN);
    }
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 write(data_stream_t* stream)
{
    u8 k = 0;
	Uart_DisableIrq(USART_CH, UartRxIrq);
    rs485_direction(DIR_OUT);
	delay(10);
    for (k = 0; k < stream->cmd; k++) //ѭ����������
    {
        Uart_SendDataPoll(USART_CH, stream->buff[k]); //ѭ������һ���ֽ�һ���ֽڵķ�
    }
	rs485_direction(DIR_IN);
	delay(10);
	Uart_ClrStatus(USART_CH, UartRC);       
	Uart_EnableIrq(USART_CH, UartRxIrq); //ʹ�ܽ����ж�
	
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
void uart_gpio_init(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //ʹ��GPIOģ��ʱ��
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(USART_TX_PORT, USART_TX_PIN, &stcGpioCfg);
    Gpio_SetAfMode(USART_TX_PORT, USART_TX_PIN, GpioAf1);          //����PA2 �˿�ΪURART1_TX
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(USART_RX_PORT, USART_RX_PIN, &stcGpioCfg);
    Gpio_SetAfMode(USART_RX_PORT, USART_RX_PIN, GpioAf1);          //����PA10 �˿�ΪURART1_RX
    ///< �˿ڷ�������->���
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    Gpio_Init(TR_PORT, TR_PIN, &stcGpioCfg); 
 	Gpio_ClrIO(TR_PORT, TR_PIN);                 //485 EN
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
void uart_port_init(void)
{
    stc_uart_cfg_t    stcCfg;
    DDL_ZERO_STRUCT(stcCfg);
    ///< ��������ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);///<ʹ��uart1ģ��ʱ��
    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;          ///<ģʽ3
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bitֹͣλ
    stcCfg.enMmdorCk        = UartMskEven;           ///<ż����
    stcCfg.stcBaud.u32Baud  = USART_BAUDRATE;        ///<������9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<ͨ��������Ƶ����
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<�������ʱ�ӣ�PCLK��Ƶ��ֵ		
    Uart_Init(USART_CH, &stcCfg);                    ///<���ڳ�ʼ��
    ///<UART�ж�ʹ��
    Uart_ClrStatus(USART_CH,UartRC);                 ///<���������
    Uart_EnableIrq(USART_CH,UartRxIrq);              ///<ʹ�ܴ��ڽ����ж�
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);         ///<ϵͳ�ж�ʹ��
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	uart_gpio_init();
	uart_port_init();	
	return 0;
}

/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	
return 0;
}

/***********************************************END*****************************************************/

