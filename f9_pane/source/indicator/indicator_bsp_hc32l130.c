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
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
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
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 read(data_stream_t* stream)
{

	return 0;
}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
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
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 initialization(data_stream_t* stream)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< ��GPIO����ʱ���ſ�
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    
    ///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    ///< GPIO IO USER LED��ʼ�� 
    Gpio_Init(GpioPortB, GpioPin12, &stcGpioCfg); // LED1
    Gpio_Init(GpioPortB, GpioPin13, &stcGpioCfg); // LED2
    Gpio_Init(GpioPortB, GpioPin14, &stcGpioCfg); // LED3
    Gpio_Init(GpioPortB, GpioPin15, &stcGpioCfg); // LED4
    Gpio_Init(GpioPortA, GpioPin11, &stcGpioCfg); // LED5
    Gpio_Init(GpioPortA, GpioPin12, &stcGpioCfg); // LED6	
	
	/*�ر�LED*/
    Gpio_SetIO(GpioPortB, GpioPin12); //LED4
    Gpio_SetIO(GpioPortB, GpioPin13); //LED3
    Gpio_SetIO(GpioPortB, GpioPin14); //LED2
    Gpio_SetIO(GpioPortB, GpioPin15); //LED1
    Gpio_SetIO(GpioPortA, GpioPin11); //LED2
    Gpio_SetIO(GpioPortA, GpioPin12); //LED1	
	
	return 0;
}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 interrput(data_stream_t* stream)
{
	
return 0;
}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 run(data_stream_t* stream)
{	
	return 0;
}
/***********************************************END*****************************************************/

