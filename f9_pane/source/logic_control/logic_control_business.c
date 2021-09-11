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
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 read( data_stream_t* stream)
{
	/*从逻辑缓存中读取数据*/
	stream_cache_read(&logic_read_cache, stream);
	return 0;
}

/***********************************************************************************************************
* @名称	: 
* @描述	: 给指定的业务发送数据
***********************************************************************************************************/
void directional_release_data_to_cache(u8* data, u8 data_length , u8 *target_name , u8 target_name_length)
{	
	memcpy(current_stream.hex, data, data_length); 					/*复制指令*/
	memcpy(current_stream.buff, target_name, target_name_length); 	/*复制名称*/
	stream_cache_write(&logic_read_cache, &current_stream );		/*缓存数据*/
	data_stream_clear(&current_stream); 							/*清空缓存数据*/	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void button_handle( data_stream_t* stream)
{
	/*按键处理  执行灯操作和通讯操作*/ 
	if(strstr((char*)(stream->buff),"button")!=NULL)
	{
		/*记录按键通道*/
		user_logic_data.button_channel = stream->hex[0];
		/*按键闪烁类型  不改变状态信息*/
		if(user_logic_data.button_type == BUTTON_TWIKLE )
		{
			/*RGB灯操作*/
			u8 cmd_buff[5];		
			cmd_buff[0] = SINGLE_CHANNEL;									/*通道模式*/ 
			cmd_buff[1] = user_logic_data.button_channel;		/*通道*/
			cmd_buff[2] = RGB_TWINKLE;							/*模式*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_rgb" , sizeof("lgc_rgb"));		
			/*通讯操作*/
			cmd_buff[0] = SINGLE_CHANNEL;												/*通道模式*/ 
			cmd_buff[1] = user_logic_data.button_channel;								/*通道*/
			cmd_buff[2] = !user_logic_data.led_state[user_logic_data.button_channel];	/*模式*/
			directional_release_data_to_cache(cmd_buff , 3 , (u8*)"lgc_comm", sizeof("lgc_comm"));
		}
		/*按键取反类型  改变状态信息*/
		else if(user_logic_data.button_type == BUTTON_NEGATION )
		{		
			user_logic_data.led_state[user_logic_data.button_channel] = !user_logic_data.led_state[user_logic_data.button_channel];
			/*RGB灯操作*/
			u8 cmd_buff[5];		
			cmd_buff[0] = SINGLE_CHANNEL;									/*通道模式*/ 
			cmd_buff[1] = user_logic_data.button_channel;		/*通道*/
			cmd_buff[2] = user_logic_data.led_state[stream->hex[0]] ;							/*模式*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_rgb" , sizeof("lgc_rgb"));
			
			/*通讯操作*/
			cmd_buff[0] = SINGLE_CHANNEL;												/*通道模式*/ 
			cmd_buff[1] = user_logic_data.button_channel;								/*通道*/
			cmd_buff[2] = user_logic_data.led_state[user_logic_data.button_channel];	/*模式*/
			directional_release_data_to_cache(cmd_buff , 3 , (u8*)"lgc_comm", sizeof("lgc_comm"));	
			/*按键映射OUTPUT*/
			button_mapping_handle();			
		}
		
	}	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
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
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void infrared_handle( data_stream_t* stream)
{
	if(strstr((char*)(stream->buff),"infrared")!=NULL)
	{
		if(stream->hex[0] == 1)
		{
			u8 cmd_buff[5];		
			cmd_buff[0] = STANDBY_CHANNEL;									/*开背光指令**/ 
			directional_release_data_to_cache(cmd_buff , 1 ,(u8*) "lgc_rgb", sizeof("lgc_rgb"));		
		}				
	}
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void button_mapping_handle( void)
{
	u8 cmd_buff[5];	
	u8 all_state = 0;
	/*判断映射*/
	for(u8 i = 0 ; i <BUTTON_NUM ; i++)
	{
		if(user_logic_data.output_mapping[i] < OUTPUT_NUM)
		{
			user_logic_data.output_state[ user_logic_data.output_mapping[i] ] = user_logic_data.led_state[i]; 
		}
	}
	/*更新总状态*/
	for(u8 i = 0 ; i < OUTPUT_NUM ; i++)
	{
		if(user_logic_data.output_state[i] == 1)
			all_state |= 0x01<<i;
	}	
	/*OUTPUT控制*/
	cmd_buff[0] = MULTI_CHANNEL;							/*道模式*/ 
	cmd_buff[1] = all_state;								/*通道*/	
	directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_out", sizeof("lgc_out"));	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static void communication_handle( data_stream_t* stream)
{
	u8 cmd_buff[5];		
	if(strstr((char*)(stream->buff),"communication")!=NULL)
	{
		/*单灯指令*/
		if(stream->hex[0] == SINGLE_CHANNEL)
		{
			user_logic_data.led_state[stream->hex[1]] = stream->hex[2];
			
			cmd_buff[0] = SINGLE_CHANNEL;								/*道模式*/ 
			cmd_buff[1] = stream->hex[1];								/*通道*/
			cmd_buff[2] = user_logic_data.led_state[stream->hex[1]];	/*数值*/
			/*RGB*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_rgb", sizeof("lgc_rgb"));
			/*按键映射OUTPUT*/
			button_mapping_handle();		
		}
		/*多灯指令*/
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
			cmd_buff[0] = MULTI_CHANNEL;								/*道模式*/ 
			cmd_buff[1] = stream->hex[1];								/*通道*/	
			directional_release_data_to_cache(cmd_buff , 2 ,(u8*) "lgc_rgb", sizeof("lgc_rgb"));
			/*按键映射OUTPUT*/
			button_mapping_handle();	
		}
		/*多灯查询指令*/
		else if(stream->hex[0] == MULTI_ANSWER)
		{
			u8 all_state =0;	
			for(u8 i = 0 ; i < BUTTON_NUM ; i++)
			{
				if(user_logic_data.led_state[i] == 1)
					all_state |= 0x01<<i;
			}
			/*通讯*/
			cmd_buff[0] = MULTI_ANSWER;								/*道模式*/ 
			cmd_buff[1] = all_state;								/*通道*/
			directional_release_data_to_cache(cmd_buff , 2 ,(u8*) "lgc_comm", sizeof("lgc_comm"));			
		}
		/*单通道输出指令*/
		else if(stream->hex[0] == SINGLE_OUT)
		{
			if(stream->hex[1] < OUTPUT_NUM)
			{
				user_logic_data.output_state[stream->hex[1]] =  stream->hex[2];
				cmd_buff[0] = SINGLE_CHANNEL;								/*道模式*/ 
				cmd_buff[1] = stream->hex[1];								/*通道*/
				cmd_buff[2] = stream->hex[2];								/*数值*/
				/*OUTPUT*/
				directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_out", sizeof("lgc_out"));
			}				
		}		
		/*单通道输出指令*/
		else if(stream->hex[0] == MULTI_OUT)
		{
			for(u8 i = 0 ; i < OUTPUT_NUM ; i++)
			{
				if((stream->hex[1]&(0x01<<i)) == 0)
					user_logic_data.output_state[i] = 0;
				else
					user_logic_data.output_state[i] = 1;
			}			
			
			cmd_buff[0] = MULTI_CHANNEL;								/*道模式*/ 
			cmd_buff[1] = stream->hex[1];								/*通道*/	
			/*OUTPUT*/
			directional_release_data_to_cache(cmd_buff , 3 ,(u8*) "lgc_out", sizeof("lgc_out"));			
		}		
	}
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 接收所有业务的数据 解析数据 存储状态 控制所有业务之间的逻辑
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	/*将推送的数据依次交给相应的模块进行判断执行*/
	/*通讯数据*/
	communication_handle(stream);
	/*按键数据*/
	button_handle(stream);
	/*拨码数据*/
	dial_handle(stream);
	/*红外感应数据*/
	infrared_handle(stream);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	user_logic_data.button_channel = 0xff;
	if(user_logic_data.button_type > 1)
		user_logic_data.button_type = 0;
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_run_data->ms_clk++;
	if((current_run_data->ms_clk%10) == 0)
	{
		current_run_data->flag=1;/*运行标志位*/	
	}
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	if(current_run_data->flag == 1)
	{
		current_run_data->flag = 0;
		/*异步指令处理*/			
	}
	return 0;
}
/*************************************************END*******************************************************/

