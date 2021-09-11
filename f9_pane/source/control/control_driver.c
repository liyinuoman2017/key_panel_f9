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
#include "control_bsp_hc32l130.h"
#include "control_driver.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
#define INDICATOR_NUM 4
#define SACNF_NUM 4

/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/

typedef struct control_frame_def
{
	u8  status[INDICATOR_NUM];
	u16 delay;
	u8  flag;
	u16 count;
	u8 status_all;
}control_frame_t;



/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
static data_stream_t current_stream;
static application_template_t* current_operation = &control_bsp_operation;
control_frame_t  user_control=
{
	.flag=0,
	.delay=10,
};
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
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
application_template_t control_driver_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,	
};

/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 read( data_stream_t* stream)
{
	return 0;
}

/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 write( data_stream_t* stream)
{
	/*取反*/
	if(stream->buff[1] == 0xff)
	{
		user_control.status[stream->buff[0]] = !user_control.status[stream->buff[0]];
		current_stream.buff[0]=stream->buff[0];
		current_stream.buff[1]=user_control.status[stream->buff[0]];
		/*反馈灯状态*/
		stream->buff[3] = user_control.status[stream->buff[0]];
		/*写操作*/
		current_operation->write(&current_stream);			
	}
	/*多灯延时扫描*/
	else if(stream->buff[1] == 0xfe)
	{
		/*延时时间*/
		user_control.delay = stream->buff[2]*256 + stream->buff[3];
		user_control.flag = 1;
		user_control.count =0;
		user_control.status_all = stream->buff[4];
	}	
	else
	{
		/*1为执行 非1为关*/  
		if(stream->buff[1] == 1)
			user_control.status[stream->buff[0]] = 1;
		else
			user_control.status[stream->buff[0]] = 0;
		/*直接操作通道和数值*/
		current_stream.buff[0]=stream->buff[0];
		current_stream.buff[1]=user_control.status[stream->buff[0]];
		/*写操作*/
		current_operation->write(&current_stream);		
	}		
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
	current_operation->initialization(stream);
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
	if(user_control.flag == 1)
	user_control.count++;
	return 0;
}
/******************************************
函数名称：
功    能：输出扫描
参    数：
返回值  ：
*******************************************/
void output_sacnf(void)
{
	static u8 i=0;
	if(user_control.flag == 1)
	{
		if(user_control.count > user_control.delay)
		{		
			/*1为执行 非1为不执行*/ 
			 if((user_control.status_all & (0x01 << i))!=0)
				user_control.status[i] = 1;
			else
				user_control.status[i] = 0;
			/*直接操作通道和数值*/
			current_stream.buff[0]=i;
			current_stream.buff[1]=user_control.status[i];
			/*写操作*/
			current_operation->write(&current_stream);
			/*更新扫描状态*/
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
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 run(data_stream_t* stream)
{
	/*输出扫描*/
	output_sacnf();		
	return 0;
}

/***********************************************END*****************************************************/

