/**
*********************************************************************************************************
*                                        
*                                      (c) Copyright 2021-2031
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* 
*********************************************************************************************************
**/
#ifndef __COMMUNICATION_FLAMILY_H__
#define __COMMUNICATION_FLAMILY_H__

/*
*********************************************************************************************************
Includes 
*********************************************************************************************************
*/


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


///////////////////////宏定义
#define COM_NULL 			0
#define COM_UPSTREAM        1
#define COM_DOWNSTREAM		2
#define COM_SET				3
#define COM_INQUIRY			4

#define COM_RGB				0X10
#define COM_MAPPING			0X11

#define EXTEND_ENABLE 	1
#define EXTEND_DISABLE	0


#define EXTEND_USE 1
#ifdef EXTEND_USE
#define INS_START 11

#else
#define INS_START 6
#endif

#define INS_CHANNEL 7

#define COM_LAMP_TYPE				0X01
#define COM_LAMP_SINGLE_INS 		0X00
#define COM_LAMP_SWITCH_INS 		0X01
#define COM_LAMP_ADJUST_INS 		0X02
#define COM_DOUBLE_LAMP_ADJUST_INS 	0X03
#define COM_TRIPLE_LAMP_ADJUST_INS 	0X04
#define COM_MULTIPLE_LAMP_INS 	    0X05


#define COM_AIR_CONDITION_TYPE		0X02
#define COM_AIR_CONDITION_POWER		0X01
#define COM_AIR_CONDITION_MODE		0X02
#define COM_AIR_CONDITION_SPEED		0X03
#define COM_AIR_CONDITION_TEMP		0X04

#define COM_CURTAIN_TYPE 			0X03
#define COM_CURTAIN_SWITCH 			0X01
#define COM_CURTAIN_SCALE  			0X02

#define COM_MUSIC_TYPE				0X04
#define COM_MUSIC_PLAY 				0X01
#define COM_MUSIC_UP_DOWN 			0X02
#define COM_MUSIC_SOURCE 			0X03
#define COM_MUSIC_VOLUME 			0X04


#define COM_FLOOR_HAET_TYPE 		0X05
#define COM_FLOOR_HAET_POWER 		0X01
#define COM_FLOOR_HAET_TEMP  		0X02

#define COM_WIND_TYPE 				0X06
#define COM_WIND_POWER 				0X01
#define COM_WIND_SPEED  			0X02

#define COM_SCENE_TYPE 				0X07

#define COM_OUTPUT_TYPE				0XF1
///////////////////////变量声明


#define COM_LONG 60



typedef struct flamily_ins_frame
{
	uint8_t operation;			//指令 
	unsigned char data_buff[20];			//信息  

}FLAMILY_INS_FRAME;


///////////////////////函数声明
void Communication_Send_Instructions(
unsigned char device_address,unsigned char data_type,unsigned char device_type,
unsigned char device_brand,unsigned char device_add,unsigned char device_channel,
unsigned char null_1,unsigned char null_2,unsigned char null_3,
unsigned char *instructions_data ,unsigned char instructions_long);

u8 user_agreement_analysis(u8* receive_buff , u8 receive_length );
void Eliminate_Warning(void);
void Multiple_Lamp_Switch_Package(unsigned char device_add,unsigned int lamp_delay,unsigned char lamp_state,unsigned char com_state,unsigned char extend);
void Single_Lamp_Switch_Package(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char lamp_state);
void Single_Lamp_Switch_Package(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_state);
unsigned char CRC8_Check(unsigned char *ptr,unsigned char len);
void system_rgb_set(u8* receive_buff , u8 receive_length );

u8 single_lamp_switch_package(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_state,u8* send_buff);

u8 multiple_lamp_switch(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_data,u8* send_buff);
#endif

