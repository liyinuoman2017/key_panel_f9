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
#include "user_type.h"
#include "indicator_bsp_hc32l130.h"
#include "indicator_driver.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
#define INDICATOR_NUM 6
#define SACNF_NUM 4

/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/

typedef struct indicator_frame_def
{
	u8  status;	  		/*����״̬  ����  ��ʱ  ��˸  ��ʱ��˸*/	
	u16 delay;			/*��ʱʱ��  ��ʱģʽ�� ʱ��Ϊ0���  ��ʱ��˸ģʽ�� ʱ��Ϊ0����delay_status���Ƶ� */
	u16 period;     	/*����ʱ�� */
	u16 time;			/*����������ʱ�� */
   	u8  delay_status;	/*��ʱ��˸ʱ��Ϊ0���״̬ 0Ϊ��� */
}indicator_frame_t;

indicator_frame_t  indicator_led_buff[INDICATOR_NUM];

typedef struct sacnf_frame_def
{
	
	u16 delay;			/*��ʱʱ��   */
	u16 count;     		/*��ʱ */
	u16 flag;			/*��־ */
    u8 status_all;	   /*���е�״̬ */
}sacnf_frame_t;
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
static sacnf_frame_t user_control=
{
	.flag=0,
	.delay=10,
};
static data_stream_t current_stream;
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
application_template_t indicator_driver_operation=
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
static u8 read( data_stream_t* stream)
{
	return 0;
}

/******************************************
�������ƣ�
��    �ܣ� 
��    ����
����ֵ  ��
*******************************************/
static u8 write( data_stream_t* stream)
{
	/*ȡ��  ��������*/
	if(stream->buff[1] == 0xff)
	{
		indicator_led_buff[stream->buff[0]].status = !indicator_led_buff[stream->buff[0]].status;
		/*������״̬*/
		stream->buff[2] = indicator_led_buff[stream->buff[0]].status;
	}
	/*��ʱ��˸ ͨѶ����*/
	else if(stream->buff[1] == 0xfe)
	{
		
		indicator_led_buff[stream->buff[0]].status = INDICATOR_TWINKLE;
		indicator_led_buff[stream->buff[0]].period = 20;
		indicator_led_buff[stream->buff[0]].time   = 10;
		indicator_led_buff[stream->buff[0]].delay  = stream->buff[2]*20;
		indicator_led_buff[stream->buff[0]].delay_status = stream->buff[3];
		/*ͨѶָʾ�Ƶ���500ms*/
		indicator_led_buff[5].status = INDICATOR_DELAY;
		indicator_led_buff[5].delay = 50;		
	} 
	/*��ʱɨ�����*/
	else if(stream->buff[1] == 0xfd)
	{
		/*��ʱʱ��*/
		user_control.delay = stream->buff[2]*256 + stream->buff[3];
		user_control.flag = 1;
		user_control.count =0;
		user_control.status_all = stream->buff[4];	
		/*ͨѶָʾ�Ƶ���500ms*/
		indicator_led_buff[5].status = INDICATOR_DELAY;
		indicator_led_buff[5].delay = 50;		
		
	}	
	/*д����ֵ  ͨѶ����*/
	else
	{
		/*ֱ�Ӳ���ͨ������ֵ*/
		indicator_led_buff[stream->buff[0]].status = stream->buff[1];
		/*ͨѶָʾ�Ƶ���500ms*/
		indicator_led_buff[5].status = INDICATOR_DELAY;
		indicator_led_buff[5].delay = 50;
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
	indicator_bsp_operation.initialization(stream);
	/*LED4 ����Ϊ1HZ��˸ģʽ*/
	indicator_led_buff[4].status=INDICATOR_TWINKLE;
	indicator_led_buff[4].period=100;
	indicator_led_buff[4].time=50;
	indicator_led_buff[4].delay=0xffff;
	return 0;
}
/******************************************
�������ƣ�
��    �ܣ�����
��    ����
����ֵ  ��
*******************************************/
static void indicator_on(u8 channel)
{
	current_stream.buff[0]=channel;
	current_stream.buff[1]=1;
	indicator_bsp_operation.write(&current_stream);
}
/******************************************
�������ƣ�
��    �ܣ��ص�
��    ����
����ֵ  ��
*******************************************/
static void indicator_off(u8 channel)
{
	current_stream.buff[0]=channel;
	current_stream.buff[1]=0;
	indicator_bsp_operation.write(&current_stream);
}
/******************************************
�������ƣ�
��    �ܣ����ز���
��    ����
����ֵ  ��
*******************************************/
static void indicator_onoff(void)
{
	u8 ram_i;
	/*���ز���*/
	for(ram_i = 0; ram_i < INDICATOR_NUM; ram_i++)
	{
		/*������*/
		if(indicator_led_buff[ram_i].status == INDICATOR_ON)
		{
			indicator_on(ram_i);
		}
		/*�ز���*/
		else if(indicator_led_buff[ram_i].status == INDICATOR_OFF)
		{
			indicator_off(ram_i);			
		}
	}
}
/******************************************
�������ƣ�
��    �ܣ���˸����
��    ����
����ֵ  ��
*******************************************/
static void indicator_twinkle(void)
{
	static u16 clk=0;
	u8 ram_i;
	clk++;
	/*��˸����*/
	for(ram_i = 0; ram_i < INDICATOR_NUM; ram_i++)
	{
		/*�ж�����*/
		if(indicator_led_buff[ram_i].status == INDICATOR_TWINKLE)
		{
			/*��������ģʽ*/
			if(indicator_led_buff[ram_i].delay ==0xffff)
			{
				/*�ж����� �� ʱ���С ��������*/
				if((clk%indicator_led_buff[ram_i].period) < indicator_led_buff[ram_i].time)
				{
					indicator_on(ram_i);
				}
				else
				{
					indicator_off(ram_i);					
				}
			}
			/*��ʱģʽ*/
			else
			{
				if(indicator_led_buff[ram_i].delay>0)
				{
					indicator_led_buff[ram_i].delay--;
					/*�ж����� �� ʱ���С ��������*/
					if((clk%indicator_led_buff[ram_i].period) < indicator_led_buff[ram_i].time)
					{
						indicator_on(ram_i);
					}
					else
					{
						indicator_off(ram_i);							
					}					
				}
				/*��ʱ��״̬����*/
				else
				{
					if(indicator_led_buff[ram_i].delay_status == 0)
						indicator_off(ram_i);
					else
						indicator_on(ram_i);
				}
			}
		}
	}
}
/******************************************
�������ƣ�
��    �ܣ���ʱ����
��    ����
����ֵ  ��
*******************************************/
static void indicator_delay(void)
{
	static u16 clk=0;
	u8 ram_i;
	clk++;
	/*��˸����*/
	for(ram_i = 0; ram_i < INDICATOR_NUM; ram_i++)
	{
		/*�ж�����*/
		if(indicator_led_buff[ram_i].status == INDICATOR_DELAY)
		{
			/*ʱ�����0 ����*/
			if(indicator_led_buff[ram_i].delay > 0 )
			{
				indicator_led_buff[ram_i].delay--;
				indicator_on(ram_i);
			}
			else
			{
				indicator_led_buff[ram_i].status = 0;
				indicator_off(ram_i);				
			}		
		}
	}
}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 interrput(data_stream_t* stream)
{
	if(user_control.flag == 1)
	{
		/*10ms ��10*/
		user_control.count=user_control.count+10;
	}
	return 0;
}
/******************************************
�������ƣ�
��    �ܣ���ʱ����
��    ����
����ֵ  ��
*******************************************/
static void indicator_sacnf(void)
{
	static u8 i=0;
	if(user_control.flag == 1)
	{
		if(user_control.count > user_control.delay)
		{		
			/*1Ϊִ�� ��1Ϊ��ִ��*/ 
			 if((user_control.status_all & (0x01 << i))!=0)
				indicator_led_buff[i].status = 1;
			else
				indicator_led_buff[i].status = 0;
			/*����ɨ��״̬*/
			user_control.count = 0;
			i++;
			if(i >= SACNF_NUM)
			{
				user_control.flag =0;
				i=0;
			}						
		}
	}	
}
/******************************************
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
*******************************************/
static u8 run(data_stream_t* stream)
{
	/*���ز���*/
	indicator_onoff();
	/*��˸����*/	
	indicator_twinkle();
	/*��ʱ����*/
	indicator_delay();
	/*���ɨ��*/
	indicator_sacnf();		
	return 0;
}
/***********************************************END*****************************************************/

