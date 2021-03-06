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
#ifndef __CONTROL_BSP_H__
#define __CONTROL_BSP_H__

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

#define OUT_1  0
#define OUT_2  1
#define OUT_3  2
#define OUT_4  3

#define OUT1_ON 	M0P_GPIO->PAOUT_f.PA15
#define OUT1_OFF 	M0P_GPIO->PAOUT_f.PA08

#define OUT2_ON 	M0P_GPIO->PBOUT_f.PB10
#define OUT2_OFF 	M0P_GPIO->PBOUT_f.PB11 

#define OUT3_ON 	M0P_GPIO->PAOUT_f.PA06 
#define OUT3_OFF 	M0P_GPIO->PAOUT_f.PA07 

#define OUT4_ON 	M0P_GPIO->PAOUT_f.PA04
#define OUT4_OFF 	M0P_GPIO->PAOUT_f.PA05


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

extern application_template_t control_bsp_operation;

#endif
/***********************************************END*****************************************************/
