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
�������ƣ�
��    �ܣ�
��    ����
����ֵ  ��
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
	/*��������ҵ��*/
	if(strstr((char*)(stream->buff),"{\"button\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		if(location_buff[2]>=0x30)
		{
			/*ͨ��*/
			current_stream.buff[0]=location_buff[2]-0x30;
			/*���� ȡ��*/
			current_stream.buff[1]=0xff;
			current_operation->write(&current_stream);
					
		}
	}
	/*����ͨѶҵ��*/
	else if(strstr((char*)(stream->buff),"{\"lamp\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		/*����ָ��*/
		if(location_buff[2]>=0x30)
		{
			/*ͨ��*/
			current_stream.buff[0]=location_buff[3]-0x30;
			/*���� ����*/
			current_stream.buff[1]=location_buff[4]-0x30;
			current_operation->write(&current_stream);				
		}
	}
	/*����ͨѶ���ҵ��*/
	else if(strstr((char*)(stream->buff),"{\"lamp_mul\":\"")!=NULL)	
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		/*���ָ��*/
		if(location_buff[2]>=0x30)
		{
			/*���� ���*/
			current_stream.buff[1]=0xfe;
			/*���� ��ʱ*/
			current_stream.buff[2]=location_buff[3]-0x30;
			current_stream.buff[3]=location_buff[4]-0x30;
			/*8����*/
			current_stream.buff[4]=location_buff[5]-0x30;
			
			current_operation->write(&current_stream);				
		}
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
	current_operation->initialization(stream);
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
	current_run_data->flag = 1;
	current_operation->interrput(stream);
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
	if(current_run_data->flag ==1)
	{
		current_run_data->flag = 0;
		current_operation->run(stream);
	}	
	return 0;
}

/***********************************************END*****************************************************/

