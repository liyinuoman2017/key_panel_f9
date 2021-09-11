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
#include "string.h"
#include "stdlib.h"
#include "user_type.h"
#include "rgb_business.h"
#include "rgb_driver.h"
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
static data_stream_t current_stream;
static application_template_t* current_operation = &rgb_driver_operation;

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


application_template_t rgb_business_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,
};

/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 read( data_stream_t* stream)
{

	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
HEX0 1单通道 2 多通道 3开待机背光
HEX1   通道\多灯状态
HEX2  单通道数值  0关 1开 2 闪   闪模式代表按键 
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	/*RGB逻辑控制业务*/
	if(strstr((char*)(stream->buff),"lgc_rgb")!=NULL)
	{
		/*复制HEX指令数据*/
		memcpy(&current_stream.hex, stream->hex, sizeof(stream->hex));
		/*执行写操作*/
		current_operation->write(&current_stream);			
	}		
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	current_operation->initialization(stream);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 中断操作中只能包涵计数，计时间，设置标志位等不耗时操作，
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_operation->interrput(stream);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	current_operation->run(stream);
	return 0;
}


/***********************************************END*****************************************************/

