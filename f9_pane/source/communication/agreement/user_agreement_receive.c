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
* @����	: 
* @����	: �û�Э�����
receive_buff 	Ϊ���յ���һ������(�Զ���ȡ�����û�����)
receive_length  Ϊ���յ���һ�����ݳ���(�Զ���ȡ�����û�����)
�û����Ը��ݽ��������� ִ�еƺ�����Ŀ���
���Ʋ�������ֻ���ڱ������ڲ�ִ�У����Ʋ����������£�
single_channel_lamp_control_instructions ��ͨ���ƿ���
multi_channel_lamp_control_instructions  ��ͨ���ƿ���
multi_channel_lamp_query_instructions    ��ͨ����ѯ
single_channel_output_control_instructions ��ͨ��������� 
multi_channel_output_control_instructions  ��ͨ���������
***********************************************************************************************************/
u8 user_agreement_analysis(u8* receive_buff , u8 receive_length )
{	
	analysis_flag = 0;/*���ɸ�*/
	/*�ƿ��� �û����������޸�*/
	lamp_control_handle(receive_buff ,  receive_length );
	/*������� �û����������޸�*/
	output_control_handle(receive_buff ,  receive_length );
	/**�����ڲ�Э�� �û������޸� **/
	factory_fixed_agreement(receive_buff , receive_length );
	return analysis_flag;/*���ɸ�*/
}
/***********************************************************************************************************
* @����	: 
* @����	:������� 
***********************************************************************************************************/
void output_control_handle(u8* receive_buff , u8 receive_length )
{
	//�ж�֡ͷ ��������   ��ַ
	if((receive_buff[0] == 0x7e)
	&&((receive_buff[2] == COM_INQUIRY)||(receive_buff[2] == COM_DOWNSTREAM))
	&&(communication_add == receive_buff[1]))
	{
		//�ж�CRC8У��      ����У��AA
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			if(receive_buff[2] == COM_INQUIRY)/*�������� */
			{
				//�жϵ�����   
				if(receive_buff[4] == COM_OUTPUT_TYPE)
				{
					//�жϵ�������
					if(receive_buff[INS_START] == COM_LAMP_SINGLE_INS)	
						single_channel_output_control_instructions(receive_buff[INS_CHANNEL], receive_buff[INS_START+1]); /* ��ͨ��������� */
					//�ж϶������
					else if(receive_buff[INS_START] == COM_MULTIPLE_LAMP_INS)	
						multi_channel_output_control_instructions(receive_buff[INS_START+2]);   /* ��ͨ��������� */
				}		
			}	
		}	
	}		
}
/***********************************************************************************************************
* @����	: 
* @����	: �ƿ��� 
***********************************************************************************************************/
void lamp_control_handle(u8* receive_buff , u8 receive_length )
{
	//�ж�֡ͷ ��������   ��ַ
	if((receive_buff[0] == 0x7e)
	&&((receive_buff[2] == COM_INQUIRY)||(receive_buff[2] == COM_DOWNSTREAM))
	&&(communication_add == receive_buff[1]))
	{
		//�ж�CRC8У��      ����У��AA
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			if(receive_buff[2] == COM_INQUIRY)/*�������� */
			{
				//�жϵ�����   
				if(receive_buff[4] == COM_LAMP_TYPE)
				{
					//�жϵ�������
					if(receive_buff[INS_START] == COM_LAMP_SINGLE_INS)	
						single_channel_lamp_control_instructions(receive_buff[INS_CHANNEL], receive_buff[INS_START+1]);/*��ͨ���ƿ��� */
					//�ж϶������
					else if(receive_buff[INS_START] == COM_MULTIPLE_LAMP_INS)	
						multi_channel_lamp_control_instructions(receive_buff[INS_START+2]);/*��ͨ���ƿ��� */
				}		
			}
			else if(receive_buff[2] == COM_DOWNSTREAM) /* ��ѯ���� */
			{	
				//�жϵ�����
				if(receive_buff[4] == COM_LAMP_TYPE)
					multi_channel_lamp_query_instructions();		/*��ͨ����ѯ*/					
			}	
		}	
	}
}	
/***********************************************END*****************************************************/



