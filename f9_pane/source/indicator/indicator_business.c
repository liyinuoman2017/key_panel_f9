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
#include "indicator_business.h"
#include "indicator_driver.h"
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
//static data_stream_t current_stream;
static application_template_t* current_operation = &indicator_driver_operation;
//static u8 user_button_buff[4]={0,0,0,0};

static  run_data_t indicator_data;
static  run_data_t* current_run_data =&indicator_data;
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
??????????
??    ????
??    ????
??????  ??
*******************************************/
application_template_t indicator_business_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,
};

/******************************************
??????????
??    ????
??    ????
??????  ??
*******************************************/
static u8 read( data_stream_t* stream)
{
	return 0;
}
#define INDICATOR_CHANNEL   		0
#define INDICATOR_TYPE   			1
#define INDICATOR_DATA   			2
#define INDICATOR_DELAY_STATUS  	3
/******************************************
??????????
??    ????
??    ????
??????  ??
*******************************************/
static u8 button_write_business( data_stream_t* stream)
{
#if ( USER_MODE == SWITCH_MODE )
	/*????????????*/
	if(strstr((char*)(stream->buff),"{\"button\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		if(location_buff[2]>=0x30)
		{
			current_stream.buff[INDICATOR_CHANNEL]=location_buff[2]-0x30;
			/*??????????????????*/
			current_stream.buff[INDICATOR_TYPE]=0XFF;
			/*??????????*/
			current_operation->write(&current_stream);
			/*??????????*/
			stream->buff[13] = current_stream.buff[2] +0x30;					
		}
	}
#elif (( USER_MODE == ADJUST_MODE ) || ( USER_MODE == THYRISTOR_MODE ))
	/*????????????????*/
	if(strstr((char*)(stream->buff),"{\"button\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		if(location_buff[2] >= 0x30)
		{
			/*????*/
			current_stream.buff[INDICATOR_CHANNEL]=location_buff[2] - 0x30;		
			/*????????????*/
			current_stream.buff[INDICATOR_TYPE] = 0XFe;
			/*???????????? 0~4*/
			if(user_button_buff[current_stream.buff[INDICATOR_CHANNEL]] < 4)
				user_button_buff[current_stream.buff[INDICATOR_CHANNEL]]++;
			else
				user_button_buff[current_stream.buff[INDICATOR_CHANNEL]] = 0;
			
			current_stream.buff[INDICATOR_DATA] = user_button_buff[current_stream.buff[INDICATOR_CHANNEL]];
			/*??????????????*/
			if(user_button_buff[current_stream.buff[0]] == 0)
				current_stream.buff[INDICATOR_DELAY_STATUS] = 0;
     		else
				current_stream.buff[INDICATOR_DELAY_STATUS] = 1;
			
			/*??????????*/
			current_operation->write(&current_stream);
			/*??????????*/
			//stream->buff[13] = current_stream.buff[2] +0x30;				
		}
	}
#endif	
	return 0;
}
/******************************************
??????????
??    ????
??    ????
??????  ??
*******************************************/
static u8 communication_write_business( data_stream_t* stream)
{
#if ( USER_MODE == SWITCH_MODE )
	/*????????????*/
	if(strstr((char*)(stream->buff),"{\"lamp\":\"")!=NULL)
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		/*????????*/
		if(location_buff[2]>=0x30)
		{
			current_stream.buff[INDICATOR_CHANNEL]=location_buff[3]-0x30;
			current_stream.buff[INDICATOR_TYPE]=location_buff[4]-0x30;;
			current_operation->write(&current_stream);				
		}
	}
	/*????????????????*/
	else if(strstr((char*)(stream->buff),"{\"lamp_mul\":\"")!=NULL)	
	{
		char *location_buff;
		location_buff=strstr((char*)(stream->buff),":\"");
		/*????????*/
		if(location_buff[2]>=0x30)
		{
			/*???? ????*/
			current_stream.buff[1]=0xfd;
			/*???? ????*/
			current_stream.buff[2]=location_buff[3]-0x30;
			current_stream.buff[3]=location_buff[4]-0x30;
			/*8????*/
			current_stream.buff[4]=location_buff[5]-0x30;
			
			current_operation->write(&current_stream);				
		}
	}	
#elif (( USER_MODE == ADJUST_MODE ) || ( USER_MODE == THYRISTOR_MODE ))
	/*????????????????*/
	if(strstr((char*)(stream->buff),"{\"com_adj\":\"")!=NULL)
	{
		char *location_buff;
		u8 adjust_data=0;
		location_buff=strstr((char*)(stream->buff),":\"");		
		/*????*/
		current_stream.buff[INDICATOR_CHANNEL]=location_buff[2] - 0x30;		
		/*????????????*/
		current_stream.buff[INDICATOR_TYPE] = 0XFe;
		/*????????1??*/		
		current_stream.buff[INDICATOR_DATA] = 1;
		/*??????????????*/
		adjust_data=atoi(&location_buff[3]);
		if(adjust_data == 0)
			current_stream.buff[INDICATOR_DELAY_STATUS] = 0;
		else
			current_stream.buff[INDICATOR_DELAY_STATUS] = 1;
		/*??????????*/
		current_operation->write(&current_stream);
		/*??????????*/
		//stream->buff[13] = current_stream.buff[2] +0x30;		
	}
#endif		
	return 0;	
}
/******************************************
??????????
??    ???????????? current_stream.buff ???????? ??????????
??    ????
??????  ??
*******************************************/
static u8 write( data_stream_t* stream)
{
	/*????????????*/
	button_write_business(stream);
	/*????????????*/
	communication_write_business(stream);	
	return 0;
}
/******************************************
??????????
??    ????
??    ????
??????  ??
*******************************************/
static u8 initialization(data_stream_t* stream)
{
	current_operation->initialization(stream);
	return 0;
}
/******************************************
??????????
??    ????
??    ????
??????  ??
*******************************************/
static u8 interrput(data_stream_t* stream)
{
	/*10ms ????????*/
	current_run_data->ms_clk++;
	if((current_run_data->ms_clk % 10) == 0)
	{
		current_run_data->flag = 1;
		current_operation->interrput(stream);
	}
	return 0;
}
/******************************************
??????????
??    ????
??    ????
??????  ??
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

