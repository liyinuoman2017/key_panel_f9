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
#include "stdio.h"  
#include "user_type.h"
#include "communication_business.h"
#include "communication_driver.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
#define COMMUNICATION_BUSINESS_START  9

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
static application_template_t* current_operation = &communication_driver_operation;
const char communication_business_char[30]="{\"lamp\":\"   \"}";
//const char communication_business_char[30]="{\"lamp\":\"   \"}";
u8 communication_add=1; 


u8  lamp_buff[8]={0,0,0,0,0,0,0,0};
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

application_template_t communication_business_operation=
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
HEX0 1��ͨ�� 2 ��ͨ�� 3����������
HEX1   ͨ��\���״̬
HEX2  ��ͨ����ֵ  0�� 1�� 2 ��   ��ģʽ�������� 
***********************************************************************************************************/
static u8 read( data_stream_t* stream)
{
	/*��ȡͨѶ*/
	if(current_operation->read(&current_stream))
	{
		memcpy(stream->hex , current_stream.hex , 3 );	
		memcpy(stream->buff,"communication",sizeof("communication"));		
		return 1;	
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	/*�߼�����ҵ��*/
	if(strstr((char*)(stream->buff),"lgc_comm")!=NULL)
	{
		/*����HEXָ������*/
		memcpy(&current_stream.hex, stream->hex, sizeof(stream->hex));
		/*ִ��д����*/
		current_operation->write(&current_stream);			
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	current_operation->initialization(stream);
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_operation->interrput(stream);
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	current_operation->run(stream);	
	return 0;
}

/***********************************************END*****************************************************/
