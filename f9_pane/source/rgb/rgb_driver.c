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
#include "rgb_bsp_hc32l130.h"
#include "rgb_driver.h"
#include "string.h"
#include "stdlib.h"
#include "data_business.h"

/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/

#define RGB_DELAY_NUM  5000

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
u8 standby_time;
static data_stream_t current_stream;
static application_template_t* current_operation = &rgb_bsp_operation;
static  run_data_t rgb_data=
{
	.delay=5000,
};
static  run_data_t* current_run_data =&rgb_data;
/*  灯颜色    G  R  B*/
// u8  work_on[3]={200,200,200};
// u8  work_off[3]={165,250,0};
// u8  standby_on[3]={20,20,20};
// u8  standby_off[3]={16,25,0}; 

static u8 rgb_lamp_buff[RGB_LAMP_NUM]={0,0,0,0,0,0,0,0};
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


application_template_t rgb_driver_operation=
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

	current_operation->read(stream);		
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 写RGB灯状态
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{

	/*单通道*/
	if(stream->hex[0] == SINGLE_CHANNEL)
	{
		if(stream->hex[1] < 8)
		{
			if(stream->hex[2] == RGB_OFF)
				rgb_lamp_buff[ stream->hex[1] ] = 0;	
			else if(stream->hex[2] == RGB_ON)
				rgb_lamp_buff[ stream->hex[1] ] = 1;
			else if(stream->hex[2] == RGB_TWINKLE)
			{
				rgb_lamp_buff[8+ stream->hex[1] ] = 1;
				current_run_data->delay = standby_time*1000;
			}	
		}			
	}
	else if(stream->hex[0] == MULTI_CHANNEL)
	{
		/*按键指令 打开延时*/
		for(u8 i = 0 ; i < 8 ; i++)
		{
			if((stream->hex[1] & (0x01<<i)) == 0)
			{
				rgb_lamp_buff[ i ] = 0;
			}
			else
				rgb_lamp_buff[ i ] = 1;	
				
		}

	}
	else if(stream->hex[0] == STANDBY_CHANNEL)
	{
		current_run_data->delay = standby_time*1000;
	}		
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
extern data_frame_t cpy_flash;
static u8 initialization(data_stream_t* stream)
{
	if(standby_time < 5)
		standby_time = 5;
	if(standby_time > 60)
		standby_time = 60;
	
	current_operation->initialization(stream);
	
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void led_all_on(void)
{
	for(u8 i=0; i<8; i++)
		rgb_lamp_buff[i] =1;				
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void led_all_off(void)
{
	for(u8 i=0; i<8; i++)
		rgb_lamp_buff[i] =0;				
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void led_one_on(u8 num)
{
	led_all_off();
	rgb_lamp_buff[num] = 1;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 开机效果
***********************************************************************************************************/
void indicator_start_show(void)
{
	static u8 start_flag =0;
	static u8 start_time =0;

	if(start_time<45)
		start_time++;
	/*1秒开机动作*/
	if(start_time<45)
	{
		/*0.1秒为一个节拍*/
		switch(start_time)
		{
			case 1:
			{
				//黑白
				system_data.work_off[0]=0;
				system_data.work_off[1]=0;
				system_data.work_off[2]=0;
				
				system_data.work_on[0]=255;
				system_data.work_on[1]=255;
				system_data.work_on[2]=255;				
				led_one_on(0);
			}
			break;
			case 2:
			{
				led_one_on(1);
			}
			break;
			case 3:
			{
				led_one_on(2);
			}
			break;
			case 4:
			{
				led_one_on(3);
			}
			break;
			case 5:
			{
				led_one_on(4);
			}
			break;
			case 6:
			{
				led_one_on(5);
			}
			break;
			case 7:
			{
				led_one_on(6);
			}
			break;
			case 8:
			{
				led_one_on(7);
			}
			break;			
			case 9:
			{
				led_all_off();
			}
			break;
			case 10:
			{
				//黑白
				system_data.work_off[0]=0;
				system_data.work_off[1]=0;
				system_data.work_off[2]=0;
				
				system_data.work_on[0]=255;
				system_data.work_on[1]=0;
				system_data.work_on[2]=0;				
				led_one_on(0);
			}
			break;
			case 11:
			{
				led_one_on(1);
			}
			break;
			case 12:
			{
				led_one_on(2);
			}
			break;
			case 13:
			{
				led_one_on(3);
			}
			break;
			case 14:
			{
				led_one_on(4);
			}
			break;
			case 15:
			{
				led_one_on(5);
			}
			break;
			case 16:
			{
				led_one_on(6);
			}
			break;
			case 17:
			{
				led_one_on(7);
			}
			break;
			case 18:
			{
				led_all_off();
			}
			break;			
			////红
			case 19:
			{
				
				system_data.work_on[0]=255;
				system_data.work_on[1]=0;
				system_data.work_on[2]=0;				
				led_all_on();
			}
			break;
			case 22:
			{
				
				system_data.work_on[0]=100;
				system_data.work_on[1]=0;
				system_data.work_on[2]=0;				
				led_all_on();
			}
			break;
			case 25:
			{
				
				system_data.work_on[0]=20;
				system_data.work_on[1]=0;
				system_data.work_on[2]=0;				
				led_all_on();
			}
			break;
			////绿
			case 28:
			{
				
				system_data.work_on[0]=0;
				system_data.work_on[1]=255;
				system_data.work_on[2]=0;				
				led_all_on();
			}
			break;
			case 31:
			{
				
				system_data.work_on[0]=0;
				system_data.work_on[1]=100;
				system_data.work_on[2]=0;				
				led_all_on();
			}
			break;
			case 34:
			{
				
				system_data.work_on[0]=0;
				system_data.work_on[1]=20;
				system_data.work_on[2]=0;				
				led_all_on();
			}
			break;
			////蓝
			case 37:
			{
				
				system_data.work_on[0]=0;
				system_data.work_on[1]=0;
				system_data.work_on[2]=255;				
				led_all_on();
			}
			break;
			case 40:
			{
				
				system_data.work_on[0]=0;
				system_data.work_on[1]=0;
				system_data.work_on[2]=100;				
				led_all_on();
			}
			break;
			case 43:
			{
				
				system_data.work_on[0]=0;
				system_data.work_on[1]=0;
				system_data.work_on[2]=20;				
				led_all_on();
			}
			break;					
			default:
			break;
		}
	}
	else
	{
		if(start_flag == 0)
		{
			start_flag =1;
			led_all_off();
			memcpy(&system_data ,&cpy_flash,sizeof(data_frame_t));			
		}
	
	}			
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_run_data->ms_clk++;
	/*100ms 置标志位*/
	if((current_run_data->ms_clk%100) == 0)
	{
		current_run_data->flag = 1;
		indicator_start_show();
	}
	if(current_run_data->delay > 0)	
	{
		current_run_data->delay--;
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
		/*将8个灯状态转换成8组RGB值*/
		for(u8 i = 0; i< RGB_LAMP_NUM ;i++)
		{
			/*点亮*/
			if(rgb_lamp_buff[i] !=0)
			{
				/*判断是否待机*/
				if(current_run_data->delay >0)
				{
					/*按键按下后亮1秒*/
					if(rgb_lamp_buff[8+i]>0)
					{
						rgb_lamp_buff[8+i]--;
						/*灯转态取反*/
						current_stream.buff[i*3] 		= system_data.work_off[0];
						current_stream.buff[i*3 +1] 	= system_data.work_off[1];
						current_stream.buff[i*3 +2] 	= system_data.work_off[2];							
					}
					else
					{						
						current_stream.buff[i*3] 		= system_data.work_on[0];
						current_stream.buff[i*3 +1] 	= system_data.work_on[1];
						current_stream.buff[i*3 +2] 	= system_data.work_on[2];
					}
				}
				else
				{
					/*开待机*/
					current_stream.buff[i*3] 		= system_data.standby_on[0];
					current_stream.buff[i*3 +1] 	= system_data.standby_on[1];
					current_stream.buff[i*3 +2] 	= system_data.standby_on[2];				
				}
			}
			else
			{
				/*判断是否待机*/
				if(current_run_data->delay >0)
				{
					/*按键按下后亮1秒*/
					if(rgb_lamp_buff[8+i]>0)
					{
						rgb_lamp_buff[8+i]--;
						/*灯转态取反*/
						current_stream.buff[i*3] 		= system_data.work_on[0];
						current_stream.buff[i*3 +1] 	= system_data.work_on[1];
						current_stream.buff[i*3 +2] 	= system_data.work_on[2];							
					}
					else
					{
						current_stream.buff[i*3] 		= system_data.work_off[0];
						current_stream.buff[i*3 +1] 	= system_data.work_off[1];
						current_stream.buff[i*3 +2] 	= system_data.work_off[2];
					}
				}
				else
				{
					/*关待机*/
					current_stream.buff[i*3] 		= system_data.standby_off[0];
					current_stream.buff[i*3 +1] 	= system_data.standby_off[1];
					current_stream.buff[i*3 +2] 	= system_data.standby_off[2];				
				}							
			}
		}
		/*执行写操作*/
		current_operation->write(&current_stream);	
	}		
	return 0;
}

/***********************************************END*****************************************************/

