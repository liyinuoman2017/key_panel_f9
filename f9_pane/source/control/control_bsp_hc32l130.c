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
#include "control_bsp_hc32l130.h"


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
application_template_t control_bsp_operation=
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

	return 0;
}
/******************************************
�������ƣ�
��    �ܣ���������ص�����
��    ����
����ֵ  ��
*******************************************/
void out1_on_handle(u8 data)
{
	OUT1_ON=data;
}
void out1_off_handle(u8 data)
{
	OUT1_OFF=data;
}
void out2_on_handle(u8 data)
{
	OUT2_ON=data;
}
void out2_off_handle(u8 data)
{
	OUT2_OFF=data;
}
void out3_on_handle(u8 data)
{
	OUT3_ON=data;
}
void out3_off_handle(u8 data)
{
	OUT3_OFF=data;
}
void out4_on_handle(u8 data)
{
	OUT4_ON=data;
}
void out4_off_handle(u8 data)
{
	OUT4_OFF=data;
}
typedef void (*callback_t)(u8 data);
const callback_t callbacks_on[4] = 
{
	out1_on_handle,
	out2_on_handle,
	out3_on_handle,
	out4_on_handle,
};
const callback_t callbacks_off[4] = 
{
	out1_off_handle,
	out2_off_handle,
	out3_off_handle,
	out4_off_handle,
};
/******************************************
�������ƣ�
��    �ܣ� ���ƴű������
��    ����
����ֵ  ��
*******************************************/
#define DELAY_NUM 1000
void control_output(u8 channel,u8 data)
{
#if ( USER_MODE == THYRISTOR_MODE )

#else
 	if(data != 0)
	{
		callbacks_on[channel](1);
		delay(DELAY_NUM);
		callbacks_on[channel](0);
	}
	else
	{
		callbacks_off[channel](1);
		delay(DELAY_NUM);
		callbacks_off[channel](0);			
	}	
#endif

}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
******************************************/

static u8 write(data_stream_t* stream)
{
	if(stream->buff[0]<4)
	{
		control_output(stream->buff[0],stream->buff[1]);
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
#if ( USER_MODE == THYRISTOR_MODE )

#else
    ///< GPIO IO USER output��ʼ�� 
    Gpio_Init(GpioPortA, GpioPin4, &stcGpioCfg); // OUT1
    Gpio_Init(GpioPortA, GpioPin5, &stcGpioCfg); // OUT1
    Gpio_Init(GpioPortA, GpioPin6, &stcGpioCfg); // OUT2
    Gpio_Init(GpioPortA, GpioPin7, &stcGpioCfg); // OUT2
    Gpio_Init(GpioPortA, GpioPin8, &stcGpioCfg); // OUT3
    Gpio_Init(GpioPortA, GpioPin15, &stcGpioCfg); // OUT3
    Gpio_Init(GpioPortB, GpioPin11, &stcGpioCfg); // OUT4
    Gpio_Init(GpioPortB, GpioPin10, &stcGpioCfg); // OUT4	
     // �ű��̵ּ��� ����Ĭ������
    Gpio_ClrIO(GpioPortA, GpioPin4);
    Gpio_ClrIO(GpioPortA, GpioPin5);
    Gpio_ClrIO(GpioPortA, GpioPin6);
    Gpio_ClrIO(GpioPortA, GpioPin7);
    Gpio_ClrIO(GpioPortA, GpioPin8);
    Gpio_ClrIO(GpioPortA, GpioPin15);
    Gpio_ClrIO(GpioPortB, GpioPin10); 
    Gpio_ClrIO(GpioPortB, GpioPin11); 	
#endif	

	

	control_output(0,0);
	control_output(1,0);
	control_output(2,0);
	control_output(3,0);
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

