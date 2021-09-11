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
#include "indicator_bsp_hc32l130.h"
#include "indicator_driver.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
#define INDICATOR_NUM 6
#define SACNF_NUM 4

/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/

typedef struct indicator_frame_def
{
	u8  status;	  		/*控制状态  开关  延时  闪烁  延时闪烁*/	
	u16 delay;			/*延时时间  延时模式下 时间为0灭灯  延时闪烁模式下 时间为0根据delay_status控制灯 */
	u16 period;     	/*周期时间 */
	u16 time;			/*周期中亮灯时间 */
   	u8  delay_status;	/*延时闪烁时间为0后灯状态 0为灭灯 */
}indicator_frame_t;

indicator_frame_t  indicator_led_buff[INDICATOR_NUM];

typedef struct sacnf_frame_def
{
	
	u16 delay;			/*延时时间   */
	u16 count;     		/*计时 */
	u16 flag;			/*标志 */
    u8 status_all;	   /*所有灯状态 */
}sacnf_frame_t;
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
static sacnf_frame_t user_control=
{
	.flag=0,
	.delay=10,
};
static data_stream_t current_stream;
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
application_template_t indicator_driver_operation=
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
	/*取反  按键操作*/
	if(stream->buff[1] == 0xff)
	{
		indicator_led_buff[stream->buff[0]].status = !indicator_led_buff[stream->buff[0]].status;
		/*反馈灯状态*/
		stream->buff[2] = indicator_led_buff[stream->buff[0]].status;
	}
	/*延时闪烁 通讯操作*/
	else if(stream->buff[1] == 0xfe)
	{
		
		indicator_led_buff[stream->buff[0]].status = INDICATOR_TWINKLE;
		indicator_led_buff[stream->buff[0]].period = 20;
		indicator_led_buff[stream->buff[0]].time   = 10;
		indicator_led_buff[stream->buff[0]].delay  = stream->buff[2]*20;
		indicator_led_buff[stream->buff[0]].delay_status = stream->buff[3];
		/*通讯指示灯点亮500ms*/
		indicator_led_buff[5].status = INDICATOR_DELAY;
		indicator_led_buff[5].delay = 50;		
	} 
	/*延时扫描操作*/
	else if(stream->buff[1] == 0xfd)
	{
		/*延时时间*/
		user_control.delay = stream->buff[2]*256 + stream->buff[3];
		user_control.flag = 1;
		user_control.count =0;
		user_control.status_all = stream->buff[4];	
		/*通讯指示灯点亮500ms*/
		indicator_led_buff[5].status = INDICATOR_DELAY;
		indicator_led_buff[5].delay = 50;		
		
	}	
	/*写入数值  通讯操作*/
	else
	{
		/*直接操作通道和数值*/
		indicator_led_buff[stream->buff[0]].status = stream->buff[1];
		/*通讯指示灯点亮500ms*/
		indicator_led_buff[5].status = INDICATOR_DELAY;
		indicator_led_buff[5].delay = 50;
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
	indicator_bsp_operation.initialization(stream);
	/*LED4 设置为1HZ闪烁模式*/
	indicator_led_buff[4].status=INDICATOR_TWINKLE;
	indicator_led_buff[4].period=100;
	indicator_led_buff[4].time=50;
	indicator_led_buff[4].delay=0xffff;
	return 0;
}
/******************************************
函数名称：
功    能：开灯
参    数：
返回值  ：
*******************************************/
static void indicator_on(u8 channel)
{
	current_stream.buff[0]=channel;
	current_stream.buff[1]=1;
	indicator_bsp_operation.write(&current_stream);
}
/******************************************
函数名称：
功    能：关灯
参    数：
返回值  ：
*******************************************/
static void indicator_off(u8 channel)
{
	current_stream.buff[0]=channel;
	current_stream.buff[1]=0;
	indicator_bsp_operation.write(&current_stream);
}
/******************************************
函数名称：
功    能：开关操作
参    数：
返回值  ：
*******************************************/
static void indicator_onoff(void)
{
	u8 ram_i;
	/*开关操作*/
	for(ram_i = 0; ram_i < INDICATOR_NUM; ram_i++)
	{
		/*开操作*/
		if(indicator_led_buff[ram_i].status == INDICATOR_ON)
		{
			indicator_on(ram_i);
		}
		/*关操作*/
		else if(indicator_led_buff[ram_i].status == INDICATOR_OFF)
		{
			indicator_off(ram_i);			
		}
	}
}
/******************************************
函数名称：
功    能：闪烁操作
参    数：
返回值  ：
*******************************************/
static void indicator_twinkle(void)
{
	static u16 clk=0;
	u8 ram_i;
	clk++;
	/*闪烁操作*/
	for(ram_i = 0; ram_i < INDICATOR_NUM; ram_i++)
	{
		/*判断类型*/
		if(indicator_led_buff[ram_i].status == INDICATOR_TWINKLE)
		{
			/*正常工作模式*/
			if(indicator_led_buff[ram_i].delay ==0xffff)
			{
				/*判断周期 于 时间大小 控制亮灭*/
				if((clk%indicator_led_buff[ram_i].period) < indicator_led_buff[ram_i].time)
				{
					indicator_on(ram_i);
				}
				else
				{
					indicator_off(ram_i);					
				}
			}
			/*延时模式*/
			else
			{
				if(indicator_led_buff[ram_i].delay>0)
				{
					indicator_led_buff[ram_i].delay--;
					/*判断周期 于 时间大小 控制亮灭*/
					if((clk%indicator_led_buff[ram_i].period) < indicator_led_buff[ram_i].time)
					{
						indicator_on(ram_i);
					}
					else
					{
						indicator_off(ram_i);							
					}					
				}
				/*延时完状态控制*/
				else
				{
					if(indicator_led_buff[ram_i].delay_status == 0)
						indicator_off(ram_i);
					else
						indicator_on(ram_i);
				}
			}
		}
	}
}
/******************************************
函数名称：
功    能：延时操作
参    数：
返回值  ：
*******************************************/
static void indicator_delay(void)
{
	static u16 clk=0;
	u8 ram_i;
	clk++;
	/*闪烁操作*/
	for(ram_i = 0; ram_i < INDICATOR_NUM; ram_i++)
	{
		/*判断类型*/
		if(indicator_led_buff[ram_i].status == INDICATOR_DELAY)
		{
			/*时间大于0 开灯*/
			if(indicator_led_buff[ram_i].delay > 0 )
			{
				indicator_led_buff[ram_i].delay--;
				indicator_on(ram_i);
			}
			else
			{
				indicator_led_buff[ram_i].status = 0;
				indicator_off(ram_i);				
			}		
		}
	}
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 interrput(data_stream_t* stream)
{
	if(user_control.flag == 1)
	{
		/*10ms 加10*/
		user_control.count=user_control.count+10;
	}
	return 0;
}
/******************************************
函数名称：
功    能：延时操作
参    数：
返回值  ：
*******************************************/
static void indicator_sacnf(void)
{
	static u8 i=0;
	if(user_control.flag == 1)
	{
		if(user_control.count > user_control.delay)
		{		
			/*1为执行 非1为不执行*/ 
			 if((user_control.status_all & (0x01 << i))!=0)
				indicator_led_buff[i].status = 1;
			else
				indicator_led_buff[i].status = 0;
			/*更新扫描状态*/
			user_control.count = 0;
			i++;
			if(i >= SACNF_NUM)
			{
				user_control.flag =0;
				i=0;
			}						
		}
	}	
}
/******************************************
函数名称：
功    能：
参    数：
返回值  ：
*******************************************/
static u8 run(data_stream_t* stream)
{
	/*开关操作*/
	indicator_onoff();
	/*闪烁操作*/	
	indicator_twinkle();
	/*延时操作*/
	indicator_delay();
	/*输出扫描*/
	indicator_sacnf();		
	return 0;
}
/***********************************************END*****************************************************/

