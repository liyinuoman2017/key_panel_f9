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
#include "business_scheduler.h"
#include "data_business.h"
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

data_stream_cache_t logic_read_cache;
static data_stream_t current_stream;
static  run_data_t logic_data;
static  run_data_t* current_run_data =&logic_data;
logic_data_frame_t user_logic_data;
extern u8 communication_add;
extern data_frame_t system_data;
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
static void button_mapping_handle( void);


application_template_t logic_business_operation=
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
static u8 read( data_stream_t* stream)
{
	/*���߼������ж�ȡ����*/
	stream_cache_read(&logic_read_cache, stream);
	return 0;
}

/***********************************************************************************************************
* @����	: 
* @����	: ��ָ����ҵ��������
***********************************************************************************************************/
void directional_release_data_to_cache(u8* data, u8 data_length , u8 *target_name , u8 target_name_length)
{	
	memcpy(current_stream.hex, data, data_length); 					/*����ָ��*/
	memcpy(current_stream.buff, target_name, target_name_length); 	/*��������*/
	stream_cache_write(&logic_read_cache, &current_stream );		/*��������*/
	data_stream_clear(&current_stream); 							/*��ջ�������*/	
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static void button_handle( data_stream_t* stream)
{
	/*��������  ִ�еƲ�����ͨѶ����*/ 
	if(strstr((char*)(stream->buff),"button")!=NULL)
	{
		/*��¼����ͨ��*/
		user_logic_data.button_channel = stream->hex[0];
		/*������˸����  ���ı�״̬��Ϣ*/
		if(user_logic_data.button_type == BUTTON_TWIKLE )
		{
			/*RGB�Ʋ���*/
			u8 cmd_buff[5];		
			cmd_buff[0] = SINGLE_CHANNEL;									/*ͨ��ģʽ*/ 
			cmd_buff[1] = user_logic_data.button_channel;		/*ͨ��*/
			cmd_buff[2] = RGB_TWINKLE;							/*ģʽ*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_rgb" , sizeof("lgc_rgb"));		
			/*ͨѶ����*/
			cmd_buff[0] = SINGLE_CHANNEL;												/*ͨ��ģʽ*/ 
			cmd_buff[1] = user_logic_data.button_channel;								/*ͨ��*/
			cmd_buff[2] = !user_logic_data.led_state[user_logic_data.button_channel];	/*ģʽ*/
			directional_release_data_to_cache(cmd_buff , 3 , (u8*)"lgc_comm", sizeof("lgc_comm"));
		}
		/*����ȡ������  �ı�״̬��Ϣ*/
		else if(user_logic_data.button_type == BUTTON_NEGATION )
		{		
			user_logic_data.led_state[user_logic_data.button_channel] = !user_logic_data.led_state[user_logic_data.button_channel];
			/*RGB�Ʋ���*/
			u8 cmd_buff[5];		
			cmd_buff[0] = SINGLE_CHANNEL;									/*ͨ��ģʽ*/ 
			cmd_buff[1] = user_logic_data.button_channel;		/*ͨ��*/
			cmd_buff[2] = user_logic_data.led_state[stream->hex[0]] ;							/*ģʽ*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_rgb" , sizeof("lgc_rgb"));
			
			/*ͨѶ����*/
			cmd_buff[0] = SINGLE_CHANNEL;												/*ͨ��ģʽ*/ 
			cmd_buff[1] = user_logic_data.button_channel;								/*ͨ��*/
			cmd_buff[2] = user_logic_data.led_state[user_logic_data.button_channel];	/*ģʽ*/
			directional_release_data_to_cache(cmd_buff , 3 , (u8*)"lgc_comm", sizeof("lgc_comm"));	
			/*����ӳ��OUTPUT*/
			button_mapping_handle();			
		}
		
	}	
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static void dial_handle( data_stream_t* stream)
{
	if(strstr((char*)(stream->buff),"dial")!=NULL)
	{
		user_logic_data.add = stream->hex[0];
		communication_add =  user_logic_data.add;		
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static void infrared_handle( data_stream_t* stream)
{
	if(strstr((char*)(stream->buff),"infrared")!=NULL)
	{
		if(stream->hex[0] == 1)
		{
			u8 cmd_buff[5];		
			cmd_buff[0] = STANDBY_CHANNEL;									/*������ָ��**/ 
			directional_release_data_to_cache(cmd_buff , 1 ,(u8*) "lgc_rgb", sizeof("lgc_rgb"));		
		}				
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static void button_mapping_handle( void)
{
	u8 cmd_buff[5];	
	u8 all_state = 0;
	/*�ж�ӳ��*/
	for(u8 i = 0 ; i <BUTTON_NUM ; i++)
	{
		if(user_logic_data.output_mapping[i] < OUTPUT_NUM)
		{
			user_logic_data.output_state[ user_logic_data.output_mapping[i] ] = user_logic_data.led_state[i]; 
		}
	}
	/*������״̬*/
	for(u8 i = 0 ; i < OUTPUT_NUM ; i++)
	{
		if(user_logic_data.output_state[i] == 1)
			all_state |= 0x01<<i;
	}	
	/*OUTPUT����*/
	cmd_buff[0] = MULTI_CHANNEL;							/*��ģʽ*/ 
	cmd_buff[1] = all_state;								/*ͨ��*/	
	directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_out", sizeof("lgc_out"));	
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static void communication_handle( data_stream_t* stream)
{
	u8 cmd_buff[5];		
	if(strstr((char*)(stream->buff),"communication")!=NULL)
	{
		/*����ָ��*/
		if(stream->hex[0] == SINGLE_CHANNEL)
		{
			user_logic_data.led_state[stream->hex[1]] = stream->hex[2];
			
			cmd_buff[0] = SINGLE_CHANNEL;								/*��ģʽ*/ 
			cmd_buff[1] = stream->hex[1];								/*ͨ��*/
			cmd_buff[2] = user_logic_data.led_state[stream->hex[1]];	/*��ֵ*/
			/*RGB*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_rgb", sizeof("lgc_rgb"));
			/*����ӳ��OUTPUT*/
			button_mapping_handle();		
		}
		/*���ָ��*/
		else if(stream->hex[0] == MULTI_CHANNEL)
		{
			user_logic_data.led_state[stream->hex[1]] = stream->hex[2];
			for(u8 i = 0 ; i < BUTTON_NUM ; i++)
			{
				if((stream->hex[1]&(0x01<<i)) == 0)
					user_logic_data.led_state[i] = 0;
				else
					user_logic_data.led_state[i] = 1;
			}			
			/*RGB*/
			cmd_buff[0] = MULTI_CHANNEL;								/*��ģʽ*/ 
			cmd_buff[1] = stream->hex[1];								/*ͨ��*/	
			directional_release_data_to_cache(cmd_buff , 2 ,(u8*) "lgc_rgb", sizeof("lgc_rgb"));
			/*����ӳ��OUTPUT*/
			button_mapping_handle();	
		}
		/*��Ʋ�ѯָ��*/
		else if(stream->hex[0] == MULTI_ANSWER)
		{
			u8 all_state =0;	
			for(u8 i = 0 ; i < BUTTON_NUM ; i++)
			{
				if(user_logic_data.led_state[i] == 1)
					all_state |= 0x01<<i;
			}
			/*ͨѶ*/
			cmd_buff[0] = MULTI_ANSWER;								/*��ģʽ*/ 
			cmd_buff[1] = all_state;								/*ͨ��*/
			directional_release_data_to_cache(cmd_buff , 2 ,(u8*) "lgc_comm", sizeof("lgc_comm"));			
		}
		/*��ͨ�����ָ��*/
		else if(stream->hex[0] == SINGLE_OUT)
		{
			if(stream->hex[1] < OUTPUT_NUM)
			{
				user_logic_data.output_state[stream->hex[1]] =  stream->hex[2];
				cmd_buff[0] = SINGLE_CHANNEL;								/*��ģʽ*/ 
				cmd_buff[1] = stream->hex[1];								/*ͨ��*/
				cmd_buff[2] = stream->hex[2];								/*��ֵ*/
				/*OUTPUT*/
				directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_out", sizeof("lgc_out"));
			}				
		}		
		/*��ͨ�����ָ��*/
		else if(stream->hex[0] == MULTI_OUT)
		{
			for(u8 i = 0 ; i < OUTPUT_NUM ; i++)
			{
				if((stream->hex[1]&(0x01<<i)) == 0)
					user_logic_data.output_state[i] = 0;
				else
					user_logic_data.output_state[i] = 1;
			}			
			
			cmd_buff[0] = MULTI_CHANNEL;								/*��ģʽ*/ 
			cmd_buff[1] = stream->hex[1];								/*ͨ��*/	
			/*OUTPUT*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_out", sizeof("lgc_out"));			
		}		
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: ��������ҵ������� �������� �洢״̬ ��������ҵ��֮����߼�
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	/*�����͵��������ν�����Ӧ��ģ������ж�ִ��*/
	/*ͨѶ����*/
	communication_handle(stream);
	/*��������*/
	button_handle(stream);
	/*��������*/
	dial_handle(stream);
	/*�����Ӧ����*/
	infrared_handle(stream);
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	user_logic_data.button_channel = 0xff;
	if(user_logic_data.button_type > 1)
		user_logic_data.button_type = 0;
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_run_data->ms_clk++;
	if((current_run_data->ms_clk%10) == 0)
	{
		current_run_data->flag=1;/*���б�־λ*/	
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	if(current_run_data->flag == 1)
	{
		current_run_data->flag = 0;
		/*�첽ָ���*/			
	}
	return 0;
}
/*************************************************END*******************************************************/

