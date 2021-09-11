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
#include "communication_driver.h"
#include "user_agreement.h"
#include "data_business.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
#define COM_LONG 60
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
FLAMILY_INS_FRAME	 user_instructions;
u8 	user_add=0;
extern application_template_t communication_bsp_operation;
extern u8 communication_add; 
extern u8 *analysis_pointer ;
volatile u8 analysis_flag = 0;
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
void factory_fixed_agreement(u8* receive_buff , u8 receive_length );
void single_channel_lamp_control_instructions(u8 channel , u8 value);
void multi_channel_lamp_control_instructions(u8 all_state);
void multi_channel_lamp_query_instructions(void);
void single_channel_output_control_instructions(u8 channel , u8 value);
void multi_channel_output_control_instructions(u8 all_state);
void output_control_handle(u8* receive_buff , u8 receive_length );
void lamp_control_handle(u8* receive_buff , u8 receive_length );

/***********************************************************************************************************
* @名称	: 
* @描述	: 用户协议解析
receive_buff 	为接收到的一包数据(自动获取无需用户操作)
receive_length  为接收到的一包数据长度(自动获取无需用户操作)
用户可以根据解析的数据 执行灯和输出的控制
控制操作函数只能在本函数内部执行，控制操作函数如下：
single_channel_lamp_control_instructions 单通道灯控制
multi_channel_lamp_control_instructions  多通道灯控制
multi_channel_lamp_query_instructions    多通道查询
single_channel_output_control_instructions 单通道输出控制 
multi_channel_output_control_instructions  多通道输出控制
***********************************************************************************************************/
u8 user_agreement_analysis(u8* receive_buff , u8 receive_length )
{	
	analysis_flag = 0;/*不可改*/
	/*灯控制 用户可以自由修改*/
	lamp_control_handle(receive_buff ,  receive_length );
	/*输出控制 用户可以自由修改*/
	output_control_handle(receive_buff ,  receive_length );
	/**厂家内部协议 用户不能修改 **/
	factory_fixed_agreement(receive_buff , receive_length );
	return analysis_flag;/*不可改*/
}
/***********************************************************************************************************
* @名称	: 
* @描述	:输出控制 
***********************************************************************************************************/
void output_control_handle(u8* receive_buff , u8 receive_length )
{
	//判断帧头 数据类型   地址
	if((receive_buff[0] == 0x7e)
	&&((receive_buff[2] == COM_INQUIRY)||(receive_buff[2] == COM_DOWNSTREAM))
	&&(communication_add == receive_buff[1]))
	{
		//判断CRC8校验      万能校验AA
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			if(receive_buff[2] == COM_INQUIRY)/*设置类型 */
			{
				//判断灯类型   
				if(receive_buff[4] == COM_OUTPUT_TYPE)
				{
					//判断单灯类型
					if(receive_buff[INS_START] == COM_LAMP_SINGLE_INS)	
						single_channel_output_control_instructions(receive_buff[INS_CHANNEL], receive_buff[INS_START+1]); /* 单通道输出控制 */
					//判断多灯类型
					else if(receive_buff[INS_START] == COM_MULTIPLE_LAMP_INS)	
						multi_channel_output_control_instructions(receive_buff[INS_START+2]);   /* 多通道输出控制 */
				}		
			}	
		}	
	}		
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 灯控制 
***********************************************************************************************************/
void lamp_control_handle(u8* receive_buff , u8 receive_length )
{
	//判断帧头 数据类型   地址
	if((receive_buff[0] == 0x7e)
	&&((receive_buff[2] == COM_INQUIRY)||(receive_buff[2] == COM_DOWNSTREAM))
	&&(communication_add == receive_buff[1]))
	{
		//判断CRC8校验      万能校验AA
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			if(receive_buff[2] == COM_INQUIRY)/*设置类型 */
			{
				//判断灯类型   
				if(receive_buff[4] == COM_LAMP_TYPE)
				{
					//判断单灯类型
					if(receive_buff[INS_START] == COM_LAMP_SINGLE_INS)	
						single_channel_lamp_control_instructions(receive_buff[INS_CHANNEL], receive_buff[INS_START+1]);/*单通道灯控制 */
					//判断多灯类型
					else if(receive_buff[INS_START] == COM_MULTIPLE_LAMP_INS)	
						multi_channel_lamp_control_instructions(receive_buff[INS_START+2]);/*多通道灯控制 */
				}		
			}
			else if(receive_buff[2] == COM_DOWNSTREAM) /* 查询类型 */
			{	
				//判断灯类型
				if(receive_buff[4] == COM_LAMP_TYPE)
					multi_channel_lamp_query_instructions();		/*多通道查询*/					
			}	
		}	
	}
}	
/***********************************************END*****************************************************/



