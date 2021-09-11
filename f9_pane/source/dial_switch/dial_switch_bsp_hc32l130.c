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
/* BUFF[0]Ϊָ���еĲ����� */
static u8 read(data_stream_t* stream);
static u8 write(data_stream_t* stream);
static u8 initialization(data_stream_t* stream);
static u8 interrput(data_stream_t* stream);

/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
application_template_t dial_switch_bsp_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
};

/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 read(data_stream_t* stream)
{
	u8 dial_value;

	/*��岦����������*/
	dial_value = 32 * PB_IDR_IDR5 + 16 * PB_IDR_IDR4 + 8 * PB_IDR_IDR3 + 4 * PB_IDR_IDR2 + 2 * PB_IDR_IDR1 +  PB_IDR_IDR0;
	dial_value = (~dial_value) & 0x3f;
	
	stream->buff[0] = dial_value;
	return 0;
}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
******************************************/
static u8 write(data_stream_t* stream)
{
	
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
    ///< �˿ڷ�������->����
    stcGpioCfg.enDir = GpioDirIn;
    ///< �˿�������������->����������
    stcGpioCfg.enDrv = GpioDrvL;
    ///< �˿�����������->��	
    stcGpioCfg.enPu = GpioPuEnable;
    stcGpioCfg.enPd = GpioPdDisable;	


    ///< �˿ڿ�©�������->��©����ر�
    stcGpioCfg.enOD = GpioOdDisable;
    ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER �����ʼ�� 
	for(u8 i = 0 ; i < 6 ; i++)
		Gpio_Init(uesr_port[i], uesr_pin[i], &stcGpioCfg);
	
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

/***********************************************END*****************************************************/

