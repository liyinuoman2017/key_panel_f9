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
* @����	: 
* @����	: ���������ݷ���
�����⵽�������º�ִ�б�����
�û�ֻ��Ҫ����channel����ͨ��0~7��value������ֵ1Ϊ��  0Ϊ�� �������
ʹ��communication_485_data_send �����������ݼȿ���
�û����Ը���Э���޸�
***********************************************************************************************************/
void single_button_send_data(u8 channel , u8 value)
{
	u8 length, send_buff[30];
	/*�������ݴ�� �û����Ը���Э���޸�*/	
	length = single_lamp_switch_package(
		communication_add,     							// �豸��ַ
		COM_SET, 										
		0,												// Ʒ��
		0x01, 											// ��ַ
		channel+1,										// ͨ��
		value,     										// ��ֵ
		send_buff);
	/*���ݷ���*/	
	communication_485_data_send(send_buff , length);
}
/***********************************************************************************************************
* @����	: 
* @����	: ָʾ��״̬��ѯ 
�����⵽���ڲ�ѯָ���ִ�б�����
�û�ֻ��Ҫ����all_stateָʾ״̬�����ֽ�Ϊ8������״̬ÿ��BIT����һ����BIT0�����1 BIT7�����8 1Ϊ��  0Ϊ��
ʹ��communication_485_data_send �����������ݼȿ���
�û����Ը���Э���޸�
***********************************************************************************************************/
void multiple_button_answer_send_data(u8 all_state)
{	
	u8 length, send_buff[30];	
	/*������ݴ��  �û����Ը���Э���޸�*/
	length = multiple_lamp_switch(
		communication_add,
		COM_INQUIRY,
		0,			// Ʒ��
		0, 	// ��ַ ���θ�2λ
		0,			// ͨ��
		all_state,
		send_buff);
	/*���ݷ���*/
	communication_485_data_send(send_buff , length);	
}
/***********************************************************************************************************
* @����	: 
* @����	: У��
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
* @����	: 
* @����	: У��
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
* @����	: 
* @����	: Э�����ݴ�� 
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
	//�ж�ָ�������ܳ���
	if(instructions_long<(CACHE_BUFF_NUM-2))
	{
		//���֡ͷ ID ���� ���� ����
		send_buff[ins_long++]=0x7E;// ֡ͷ
		send_buff[ins_long++]=device_address;   //������ַ
		send_buff[ins_long++]=data_type;		 //��������
		send_buff[ins_long++]=0;                //��������ܳ���
		send_buff[ins_long++]=device_type;		 //�豸����  ��  �յ�
		send_buff[ins_long++]=device_brand;	 //Ʒ��
		send_buff[ins_long++]=device_add;		 //�豸��ַ 
		send_buff[ins_long++]=device_channel;	 //�豸ͨ�� 
		//��չ��Ϣ
		send_buff[ins_long++]=null_1;	         //����
		send_buff[ins_long++]=null_2;	         //����
		send_buff[ins_long++]=null_3;	         //���� 
		//���ָ������
		for(ram_i=0;ram_i<instructions_long;ram_i++)
		{
			send_buff[ins_long+ram_i]=instructions_data[ram_i];
		}
		ins_long=ins_long+instructions_long; //
		//ָ���ܳ�
		send_buff[3]=ins_long+2; // ���ĳ��� 
		//CRCУ��
		send_buff[ins_long]=CRC8_Generate(send_buff,ins_long);  // Ч��ֵ
		//֡β
		send_buff[ins_long+1]=0x7d;  //��β
	}
	return (ins_long+2);
}  
/***********************************************************************************************************
* @����	: 
* @����	:��ͨ�� 
***********************************************************************************************************/
u8 single_lamp_switch_package(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_state,u8* send_buff)
{
	/*�����������Ͳ�һ��*/	
	#define COM_LAMP_SWITCH 0X01
	u8 send_long ,ret;
	send_long=0;
	user_instructions.data_buff[send_long++]=COM_LAMP_SINGLE_INS;
	user_instructions.data_buff[send_long++]=device_state;// �豸 ״̬
	ret = communication_send_instructions(local_add,comm_state,COM_LAMP_SWITCH,device_brand,device_add,device_channel,0,0,0,(u8*)&user_instructions.data_buff,send_long,send_buff);	
	return ret;
}
/***********************************************************************************************************
* @����	: 
* @����	: ��ͨ��
***********************************************************************************************************/
u8 multiple_lamp_switch(unsigned char local_add,unsigned char comm_state,unsigned char device_brand,unsigned char device_add,unsigned char device_channel,unsigned char device_data,u8* send_buff)
{
	u8 send_long,ret;
	send_long=0;
	user_instructions.data_buff[send_long++]=COM_MULTIPLE_LAMP_INS;
	/*���û����ʱֻ�����ֽ�״̬λ*/
	user_instructions.data_buff[send_long++]=0;
	user_instructions.data_buff[send_long++]=device_data;	
	ret =communication_send_instructions(local_add,comm_state,COM_LAMP_SWITCH,device_brand,device_add,device_channel,0,0,0,(u8*)&user_instructions.data_buff,send_long,send_buff);				
	return ret;
}

/*************************************************END*******************************************************/
















