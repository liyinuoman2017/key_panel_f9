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
#include "data_business.h"
#include "flash.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/

#define USER_ADD 	(0xf000)
#define USER_START	(0xf000)
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
data_frame_t cpy_flash;
data_frame_t system_data;
u8 *data_opint = &system_data.check[0];
extern logic_data_frame_t user_logic_data;
extern u8 standby_time;
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
/***********************************************************************************************************
* @����	: 
* @����	: �û�������ȡ
***********************************************************************************************************/
void user_data_read(void)
{
	memcpy(user_logic_data.output_mapping, system_data.mapping, 8);	
	user_logic_data.button_type = system_data.button_type;
	standby_time =  system_data.standby_time;
}
/***********************************************************************************************************
* @����	: 
* @����	: FASH���ݶ�ȡ
***********************************************************************************************************/
void flash_data_read(void)
{
	u8* add_start = (u8*)USER_START;
	u8 flag=0;
	/*flash��д��ʼ��   48MHZ*/
    Flash_Init(6, TRUE);
	/*������*/
	for(u8 i=0 ;i < sizeof(data_frame_t); i++ )
		data_opint[i] = *add_start++;
	/*check*/
	for(u8 i=0 ;i < 4; i++  )
	{
		if(data_opint[i] != i)
			flag = 1;	
	}
	/*��ʼ��*/
	if(flag == 1)
	{
		system_data.work_on[0] = 255;
		system_data.work_on[1] = 0;
		system_data.work_on[2] = 255;

		system_data.work_off[0] = 0;
		system_data.work_off[1] = 255;
		system_data.work_off[2] = 255;

		system_data.standby_on[0] = 10;
		system_data.standby_on[1] = 0;
		system_data.standby_on[2] = 10;

		system_data.standby_off[0] = 0;
		system_data.standby_off[1] = 10;
		system_data.standby_off[2] = 10;
		
		for(u8 i=0 ;i < BUTTON_NUM; i++  )
		{
			system_data.mapping[i] = i;
		}
		for(u8 i=0 ;i < 4; i++  )
			system_data.check[i] =i;
		
		system_data.standby_time = 10;
		system_data.button_type = BUTTON_TWIKLE;
		flash_data_write();
	}
	/*������*/
	add_start = (u8*)USER_START;
	for(u8 i=0 ;i < sizeof(data_frame_t); i++  )
		data_opint[i] = *add_start++;	
	memcpy(&cpy_flash,&system_data,sizeof(data_frame_t));/*������ɫ����*/
	/*�����û�����*/
	user_data_read();
}
/***********************************************************************************************************
* @����	: 
* @����	: �û�дFLASH ����
***********************************************************************************************************/
void flash_data_write(void)
{	
	/*����*/
    while(Ok != Flash_SectorErase(USER_ADD));
	delay(100);
	
	for(u8 i=0 ;i < 4; i++  )
		system_data.check[i] =i;	
	/*д��*/
	for(u8 i=0 ;i < sizeof(data_frame_t); i++  )
		Flash_WriteByte(USER_START+i, data_opint[i]); 
	
	/*�����û�����*/
	user_data_read();	
}
/***********************************************END*****************************************************/

