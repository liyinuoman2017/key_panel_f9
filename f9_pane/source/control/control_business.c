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
#include "control_business.h"
#include "control_driver.h"
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
static application_template_t* current_operation = &control_driver_operation;
static  run_data_t contorl_data;
static  run_data_t* current_run_data =&contorl_data;
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
application_template_t control_business_operation=
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
	/*按键处理业务*/
	if(strstr((char*)(stream->buff),"{\"button\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		if(location_buff[2]>=0x30)
		{
			/*通道*/
			current_stream.buff[0]=location_buff[2]-0x30;
			/*类型 取反*/
			current_stream.buff[1]=0xff;
			current_operation->write(&current_stream);
					
		}
	}
	/*处理通讯业务*/
	else if(strstr((char*)(stream->buff),"{\"lamp\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		/*单灯指令*/
		if(location_buff[2]>=0x30)
		{
			/*通道*/
			current_stream.buff[0]=location_buff[3]-0x30;
			/*类型 数据*/
			current_stream.buff[1]=location_buff[4]-0x30;
			current_operation->write(&current_stream);				
		}
	}
	/*处理通讯多灯业务*/
	else if(strstr((char*)(stream->buff),"{\"lamp_mul\":\"")!=NULL)	
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		/*多灯指令*/
		if(location_buff[2]>=0x30)
		{
			/*类型 多灯*/
			current_stream.buff[1]=0xfe;
			/*类型 延时*/
			current_stream.buff[2]=location_buff[3]-0x30;
			current_stream.buff[3]=location_buff[4]-0x30;
			/*8个灯*/
			current_stream.buff[4]=location_buff[5]-0x30;
			
			current_operation->write(&current_stream);				
		}
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
	current_run_data->flag = 1;
	current_operation->interrput(stream);
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
	if(current_run_data->flag ==1)
	{
		current_run_data->flag = 0;
		current_operation->run(stream);
	}	
	return 0;
}

/***********************************************END*****************************************************/

