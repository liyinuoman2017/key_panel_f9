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
#include "gpio_manage_bsp_hc32l130.h"
#include "gpio_manage_driver.h"
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
typedef struct key_frame_def
{
	u8 flag;		
	u8 num;
	u8 time;
	u16 on;
	u16 off;
}key_frame_t;
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/

/*��尴����*/
#define KEN_NUM 8

#define MAX_NUM 3000
#define OFF_NUM 10
#define DELAY_NUM 5
key_frame_t key_manage[KEN_NUM];
u8 key_return=0;

static  run_data_t button_data;
static  run_data_t* current_run_data =&button_data;

/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
u8 input_read(u8 channel);
void gpio_initialization(void);
u8 dial_read(void);

static u8 read_single(u8* buff);
void output_write(u8 channel , u8 data);

static u8 read(data_stream_t* stream);
static u8 write(data_stream_t* stream);
static u8 initialization(data_stream_t* stream);
static u8 interrput(data_stream_t* stream);
static u8 run(data_stream_t* stream);

application_template_t button_driver_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,
};

/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 read(data_stream_t* stream)
{
	read_single(stream->buff);
	stream->buff[2] = dial_read();
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 read_single(u8* buff)
{
	u8 i;
	for(i = 0;i < KEN_NUM;i++)
	{
		if((key_manage[i].flag == 0)&&(key_manage[i].num > 0))
		{
			buff[0]=i;
			buff[1]=key_manage[i].num;
			/*�����������*/
			key_manage[i].num=0;
			break;
		}
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 write(data_stream_t* stream)
{
	/*��ͨ������*/
	if(stream->hex[0] == SINGLE_CHANNEL)
	{
		if(stream->hex[1] < OUTPUT_NUM)
		{
			output_write(stream->hex[1] , stream->hex[2]);
		}
		
	}
	/*��ͨ������*/
	else if(stream->hex[0] == MULTI_CHANNEL)
	{
		for(u8 i = 0; i < OUTPUT_NUM; i ++)
		{
			if((stream->hex[1]& (0x01<<i)) ==0)
				output_write(i, 0);	
			else
				output_write(i, 1);	
		}
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{

	gpio_initialization();
	
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_run_data->ms_clk++;
	if((current_run_data->ms_clk % 10) == 0)
	{
		current_run_data->flag =1;
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/	
void button_scanf(void)
{
	u8 ram_i;
	/*����ɨ��*/
	for(ram_i=0;ram_i<KEN_NUM;ram_i++)
	{
		if(input_read(ram_i))
		{
			/*û�а�  ���ж�*/
			if(key_manage[ram_i].off<MAX_NUM)
			{
				if(key_manage[ram_i].flag == 1)
					key_manage[ram_i].off++;
				else
					key_manage[ram_i].off = 0;
			}
			 /*�����ɿ�ʱ�䳬�����ֵ �����Ͱ����ж�*/
			if(key_manage[ram_i].off>OFF_NUM)
			{
				/*FLAG=1 ���ذ���ֵ�Ͱ�����*/
				if(key_manage[ram_i].flag == 1)
				{
					key_return=ram_i+1;
					key_manage[ram_i].flag=0;
					key_manage[ram_i].on=0;
					key_manage[ram_i].off=0;
				
				}
			}
			/*�����ɿ���������*/
			if(key_manage[ram_i].off>DELAY_NUM)
			{
				/*����ON����ֵ  ����NUM���� �жϳ���*/		
				if(key_manage[ram_i].on>0)
				{
					/*������ʱ���������*/
					if(key_manage[ram_i].on==MAX_NUM)
					{
						key_manage[ram_i].num=0x0c;
					}
					else
					{	
						key_manage[ram_i].num++;
						if(key_manage[ram_i].num>5)
							key_manage[ram_i].num=5;
					}				
					key_manage[ram_i].on=0;
					
				}
			}		
		}
		else
		{
			/*���º��ԼӼ���*/
			if(key_manage[ram_i].on<MAX_NUM)
				key_manage[ram_i].on++;
			/*������ʱ��������*/
			if(key_manage[ram_i].on>DELAY_NUM)
			{
			     /*�������º� ��OFF���� ����FLAG*/
				if(key_manage[ram_i].flag!=1)
				{
					key_manage[ram_i].flag=1;
					key_manage[ram_i].off=0;			
				}
								
			}		
		}
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	if(current_run_data->flag ==1)
	{
		current_run_data->flag = 0;
		button_scanf();	
	}
	return 0;
}

/***********************************************END*****************************************************/

