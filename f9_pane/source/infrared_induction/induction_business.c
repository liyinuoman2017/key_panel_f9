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
typedef struct induction_frame_def
{
	u8 flag;
	u8 channge;
	u16 on;
	u16 off;
}induction_frame_t;

/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
induction_frame_t user_induction;
//static data_stream_t current_stream;
extern application_template_t induction_bsp_operation ;
static application_template_t* current_operation = &induction_bsp_operation;
const char induction_business_char[30]="{\"induction\":\"   \"}";

static  run_data_t inductionh_data;
static  run_data_t* current_run_data =&inductionh_data;
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

application_template_t induction_business_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,
};

/***********************************************************************************************************
* @Ãû³Æ	: 
* @ÃèÊö	: 
***********************************************************************************************************/
static u8 read( data_stream_t* stream)
{
	if(((current_run_data->ms_clk%100) == 0)&& (user_induction.channge == 1))
	{
		user_induction.channge = 0;
		stream->hex[0] = user_induction.flag;
		memcpy(stream->buff,"infrared",sizeof("infrared"));	
	}
	return 0;
}
/***********************************************************************************************************
* @Ãû³Æ	: 
* @ÃèÊö	: 
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	
	return 0;
}
/***********************************************************************************************************
* @Ãû³Æ	: 
* @ÃèÊö	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	current_operation->initialization(stream);
	return 0;
}
/***********************************************************************************************************
* @Ãû³Æ	: 
* @ÃèÊö	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	
	current_run_data->ms_clk++;
	if((current_run_data->ms_clk%30) == 0)
	{
		current_run_data->flag=1;
	}
	return 0;
}
/***********************************************************************************************************
* @Ãû³Æ	: 
* @ÃèÊö	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	if(current_run_data->flag == 1)
	{
		current_run_data->flag = 0;
		/*¶ÁÈ¡*/
		if(current_operation->read(stream) == 0)
		{
			if(user_induction.off < 100)
				user_induction.off++;
			if(user_induction.off > 5)
			{
				if(user_induction.on != 0)
				{
					user_induction.flag = 0;	
					user_induction.channge = 1;					
				}
				user_induction.on = 0;			
			}			
		}
		else
		{		
			if(user_induction.on < 100)
			user_induction.on++;
			if(user_induction.on > 5)
			{
				if(user_induction.off != 0)
				{
					user_induction.flag = 1;
					user_induction.channge = 1;
		
				}
				user_induction.off = 0;		
			}			
		}	
	}
	return 0;
}

/***********************************************END*****************************************************/

