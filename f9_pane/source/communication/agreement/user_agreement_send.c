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
//typedef u8 (*callback_t)(data_stream_t* stream_in,data_stream_t* stream_out);
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
extern FLAMILY_INS_FRAME	 user_instructions;
extern application_template_t communication_bsp_operation;
extern u8 communication_add; 
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
void communication_485_data_send(u8 *send_buff, u8 send_length);

/***********************************************************************************************************
* @名称	: 
* @描述	: 单按键数据发送
程序检测到按键按下后执行本函数
用户只需要根据channel按键通道0~7，value按键数值1为开  0为关 打包数据
使用communication_485_data_send 函数发送数据既可以
用户可以根据协议修改
***********************************************************************************************************/
void single_button_send_data(u8 channel , u8 value)
{
	u8 length, send_buff[30];
	/*按键数据打包 用户可以根据协议修改*/	
	length = single_lamp_switch_package(
		communication_add,     							// 设备地址
		COM_SET, 										
		0,												// 品牌
		0x01, 											// 地址
		channel+1,										// 通道
		value,     										// 数值
		send_buff);
	/*数据发送*/	
	communication_485_data_send(send_buff , length);
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 指示灯状态查询 
程序检测到串口查询指令后执行本函数
用户只需要根据all_state指示状态，本字节为8个灯总状态每个BIT代表一个灯BIT0代表灯1 BIT7代表灯8 1为开  0为关
使用communication_485_data_send 函数发送数据既可以
用户可以根据协议修改
***********************************************************************************************************/
void multiple_button_answer_send_data(u8 all_state)
{	
	u8 length, send_buff[30];	
	/*多灯数据打包  用户可以根据协议修改*/
	length = multiple_lamp_switch(
		communication_add,
		COM_INQUIRY,
		0,			// 品牌
		0, 	// 地址 屏蔽高2位
		0,			// 通道
		all_state,
		send_buff);
	/*数据发送*/
	communication_485_data_send(send_buff , length);	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 校验
***********************************************************************************************************/
unsigned char CRC8_Generate(unsigned char *ptr,unsigned char len)
{
    unsigned char crc;
    unsigned char i;
    crc = 0;
    while(len--)
    {
       crc ^= *ptr++;
       for(i = 0;i < 8;i++)
       {
           if(crc & 0x80)
           {
               crc = (crc << 1) ^ 0x07;
           }
           else crc <<= 1;
       }
    }
    return crc;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 校验
***********************************************************************************************************/
unsigned char CRC8_Check(unsigned char *ptr,unsigned char len)
{
    unsigned char crc;
	crc=CRC8_Generate(ptr,len-2);
	if(ptr[len-2]==crc)
		return 1;
	else
		return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 协议数据打包 
***********************************************************************************************************/
u8 communication_send_instructions(
unsigned char device_address,unsigned char data_type,unsigned char device_type,
unsigned char device_brand,unsigned char device_add,unsigned char device_channel,
unsigned char null_1,unsigned char null_2,unsigned char null_3,
unsigned char *instructions_data ,unsigned char instructions_long,
u8* send_buff)
{
	uint8_t ram_i;
	uint8_t ins_long=0;
	//判断指令数据总长度
	if(instructions_long<(CACHE_BUFF_NUM-2))
	{
		//填充帧头 ID 类型 数据 长度
		send_buff[ins_long++]=0x7E;// 帧头
		send_buff[ins_long++]=device_address;   //本机地址
		send_buff[ins_long++]=data_type;		 //数据类型
		send_buff[ins_long++]=0;                //最后填入总长度
		send_buff[ins_long++]=device_type;		 //设备类型  灯  空调
		send_buff[ins_long++]=device_brand;	 //品牌
		send_buff[ins_long++]=device_add;		 //设备地址 
		send_buff[ins_long++]=device_channel;	 //设备通道 
		//扩展信息
		send_buff[ins_long++]=null_1;	         //备用
		send_buff[ins_long++]=null_2;	         //备用
		send_buff[ins_long++]=null_3;	         //备用 
		//填充指令数据
		for(ram_i=0;ram_i<instructions_long;ram_i++)
		{
			send_buff[ins_long+ram_i]=instructions_data[ram_i];
		}
		ins_long=ins_long+instructions_long; //
		//指令总长
		send_buff[3]=ins_long+2; // 报文长度 
		//CRC校验
		send_buff[ins_long]=CRC8_Generate(send_buff,ins_long);  // 效验值
		//帧尾
		send_buff[ins_long+1]=0x7d;  //报尾
	}
	return (ins_long+2);
}  
/***********************************************************************************************************
* @名称	: 
* @描述	:单通道 
***********************************************************************************************************/
u8 single_lamp_switch_package(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_state,u8* send_buff)
{
	/*面板和其他类型不一致*/	
	#define COM_LAMP_SWITCH 0X01
	u8 send_long ,ret;
	send_long=0;
	user_instructions.data_buff[send_long++]=COM_LAMP_SINGLE_INS;
	user_instructions.data_buff[send_long++]=device_state;// 设备 状态
	ret = communication_send_instructions(local_add,comm_state,COM_LAMP_SWITCH,device_brand,device_add,device_channel,0,0,0,(u8*)&user_instructions.data_buff,send_long,send_buff);	
	return ret;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 多通道
***********************************************************************************************************/
u8 multiple_lamp_switch(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_data,u8* send_buff)
{
	u8 send_long,ret;
	send_long=0;
	user_instructions.data_buff[send_long++]=COM_MULTIPLE_LAMP_INS;
	/*面板没有延时只有两字节状态位*/
	user_instructions.data_buff[send_long++]=0;
	user_instructions.data_buff[send_long++]=device_data;	
	ret =communication_send_instructions(local_add,comm_state,COM_LAMP_SWITCH,device_brand,device_add,device_channel,0,0,0,(u8*)&user_instructions.data_buff,send_long,send_buff);				
	return ret;
}

/*************************************************END*******************************************************/
















