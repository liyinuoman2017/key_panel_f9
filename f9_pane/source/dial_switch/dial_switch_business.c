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
#include "user_type.h"
#include "dial_switch_business.h"
#include "dial_switch_driver.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
#define DIAL_SWITCH_BUSINESS_START  13

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
//static data_stream_t current_stream;
static application_template_t* current_operation = &dial_switch_driver_operation;
const char dial_switch_business_char[30]="{\"d_switch\":\"   \"}";

static  run_data_t dial_switch_data;
static  run_data_t* current_run_data =&dial_switch_data;
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
application_template_t dial_switch_business_operation=
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
	if(current_run_data->flag == 1)
	{
		u8 buff[2];
		stream->cmd = 1;
		stream->buff[0]=0xff;
		/*读取*/
		current_operation->read(stream);

		buff[0] = stream->buff[0];
		memcpy(stream->buff,dial_switch_business_char,sizeof(stream->buff));
		stream->buff[DIAL_SWITCH_BUSINESS_START] 	= 0x30 + buff[0]/100;
		stream->buff[DIAL_SWITCH_BUSINESS_START+1] 	= 0x30 + buff[0]/10%10;
		stream->buff[DIAL_SWITCH_BUSINESS_START+2] 	= 0x30 + buff[0]%10;
		current_run_data->flag =0;
	}
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
	
	current_run_data->ms_clk++;
	if((current_run_data->ms_clk%1000) == 0)
	{
		current_run_data->flag=1;
	}
	return 0;
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 run(data_stream_t* stream)
{	
	return 0;
}


/***********************************************END*****************************************************/

