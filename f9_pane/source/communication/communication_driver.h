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
#ifndef __COMMUNICATION_DRIVER_H__
#define __COMMUNICATION_DRIVER_H__

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

#define CACHE_BUFF_LONGTH 		10
#define CACHE_BUFF_NUM			50
#define IDLE_NUM				10



/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/

typedef struct commnuication_frame_def
{
	uint8_t		data_cache[CACHE_BUFF_LONGTH][CACHE_BUFF_NUM];     	//通讯缓存
	uint16_t	data_cache_long[CACHE_BUFF_LONGTH]; 				//通讯缓存长度
	uint8_t		data_pointer;										//通讯缓存指针
	uint16_t 	refresh_num;
	uint16_t 	current_num;
	uint8_t		receive_buff[CACHE_BUFF_NUM];
	uint8_t 	analysis_buff[CACHE_BUFF_NUM];
	uint16_t 	analysis_long;
	uint16_t 	null;

}commnuication_frame_t;

typedef  void (*fun_void)(void);

/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/

extern application_template_t communication_driver_operation;
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
static void common_cache_data(uint16_t buff_long,commnuication_frame_t *com_pointer);
void common_receive_refresh(uint8_t receive_data, commnuication_frame_t *com_pointer);
static void common_idle_judge(commnuication_frame_t *com_pointer);
#endif
/***********************************************END*****************************************************/
