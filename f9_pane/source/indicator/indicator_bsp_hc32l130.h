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
#ifndef __INDICATOR_BSP_H__
#define __INDICATOR_BSP_H__

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

#define LED_1  0
#define LED_2  1
#define LED_3  2
#define LED_4  3
#define LED_5  4
#define LED_6  5



#if (( USER_MODE == ADJUST_MODE ) || ( USER_MODE == THYRISTOR_MODE ))
	/*????*/
	#define LED_OUT1 M0P_GPIO->PBOUT_f.PB12
	#define LED_OUT2 M0P_GPIO->PBOUT_f.PB13
	#define LED_OUT3 M0P_GPIO->PBOUT_f.PB14
	#define LED_OUT4 M0P_GPIO->PBOUT_f.PB15
	#define LED_OUT5 M0P_GPIO->PAOUT_f.PA11
	#define LED_OUT6 M0P_GPIO->PAOUT_f.PA12
#else 
	/*????*/
	#define LED_OUT1 M0P_GPIO->PBOUT_f.PB15
	#define LED_OUT2 M0P_GPIO->PBOUT_f.PB14
	#define LED_OUT3 M0P_GPIO->PBOUT_f.PB13
	#define LED_OUT4 M0P_GPIO->PBOUT_f.PB12
	#define LED_OUT5 M0P_GPIO->PAOUT_f.PA11
	#define LED_OUT6 M0P_GPIO->PAOUT_f.PA12

#endif



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

extern application_template_t indicator_bsp_operation;
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
#endif
/***********************************************END*****************************************************/
