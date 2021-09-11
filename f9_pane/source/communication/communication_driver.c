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
#include "communication_bsp_hc32l130.h"
#include "communication_driver.h"
#include "uart.h"
#include "user_agreement.h"
#include "data_business.h"
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
static data_stream_t current_stream;
static application_template_t* current_operation = &communication_bsp_operation;

commnuication_frame_t rs485_commnuication_frame;
static  run_data_t comm_data;
static  run_data_t* current_run_data =&comm_data;
extern volatile u8 analysis_flag ;
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
void user_agreement_pack(u8 *instructions, u8 *send_buff, u8 *send_length);
u8 user_agreement_analysis(u8* receive_buff , u8 receive_length );
static uint8_t check_boot_command(uint8_t *u8RxData,uint32_t length);

application_template_t communication_driver_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
	.run=&run,	
};


/***********************************************************************************************************
* @����	: 
* @����	: ��ȡ��������
***********************************************************************************************************/
u8 common_data_read(commnuication_frame_t* com_pointer,data_stream_t* stream)
{
	if(com_pointer->data_pointer>0)
	{	
		// ��ջ���     // ��  0   ������  ��  data_cache
		memset(com_pointer->receive_buff, 0,sizeof(com_pointer->receive_buff));  
		//��ս���BUFF  ��0  ��� ����  analysis_buff
		memset(com_pointer->analysis_buff, 0,sizeof(com_pointer->analysis_buff));
		//�ӻ����п������ݵ�����analysis_buff
		memcpy(com_pointer->analysis_buff, com_pointer->data_cache[com_pointer->data_pointer-1], sizeof(com_pointer->analysis_buff));
		//BOOT����ָ���ж�
		check_boot_command((uint8_t*)&com_pointer->analysis_buff[0],sizeof(com_pointer->analysis_buff));	
		//����  ���� analysis_buff  ���ݳ���
		com_pointer->analysis_long = com_pointer->data_cache_long[com_pointer->data_pointer-1];
		// ��ջ���     
		memset(com_pointer->data_cache[com_pointer->data_pointer-1], 0, sizeof(com_pointer->data_cache[com_pointer->data_pointer-1]));	
        //  ���� data_cache_long  ���ݳ���  ����
		com_pointer->data_cache_long[com_pointer->data_pointer-1] =0 ;
		 // �����ݳ���  ��һ  
		com_pointer->data_pointer--;	
		//����Ϣ�����������������	
		stream->cmd = com_pointer->analysis_long;
		memcpy(stream->buff, com_pointer->analysis_buff, sizeof(stream->buff));	
		//��ս���BUFF  ��0  ��� ����  analysis_buff
		memset(com_pointer->analysis_buff, 0, sizeof(com_pointer->analysis_buff));      
		return 1;
	}	
	return 0;	
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
u8 *analysis_pointer = current_stream.buff;
static u8 read( data_stream_t* stream)
{
	/*ִ�д������ݶ�����*/
	if(common_data_read(&rs485_commnuication_frame, &current_stream))
	{
		analysis_pointer = stream->hex;
		/*ִ��Э�����*/
		if(user_agreement_analysis(current_stream.buff, current_stream.cmd))
		 return 1;		
	}
    return 0; 
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
void communication_485_data_send(u8 *send_buff, u8 send_length)
{
	data_stream_t send_stream;
	send_stream.cmd = send_length;
	memcpy(send_stream.buff , send_buff , send_length);
	/*ִ��д����*/
	communication_bsp_operation.write(&send_stream);
}
/******************************************
�������ƣ�
��    �ܣ� �������� 
instructions Ϊ���������ָ��
send_buff Ϊ�û���Ҫ���͵�����
send_length Ϊ�û����͵����ݳ���

instructions[0]�� 1Ϊ�������ò���  2 Ϊ������ò���  0FΪ���״̬��ѯ 
instructions[1]�� ��analysis_buff[0]Ϊ1ʱ���ֽ�Ϊ��ͨ��ֵ    ��analysis_buff[0]Ϊ2ʱ���ֽ�Ϊ8������״̬ÿ��BIT����һ����BIT0�����1 BIT7�����8 1Ϊ��  0Ϊ��
instructions[2]�� ��analysis_buff[0]Ϊ1ʱ���ֽ�Ϊ��״̬  1Ϊ��  0Ϊ��

��    ����
����ֵ  ��
*******************************************/
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
void single_button_send_data(u8 channel , u8 value);
void multiple_button_answer_send_data(u8 all_state);

void user_agreement_pack(u8 *instructions, u8 *send_buff, u8 *send_length)
{
	if(instructions[0] == SINGLE_CHANNEL)
	{
		single_button_send_data(instructions[1], instructions[2]);
	}
	else if(instructions[0] == MULTI_ANSWER)
	{
		multiple_button_answer_send_data(instructions[1]);
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	/*ִ��Э��������*/
	user_agreement_pack(stream->hex , current_stream.buff , &current_stream.cmd);
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	current_operation->initialization(stream);
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_run_data->flag = 1;
	common_idle_judge(&rs485_commnuication_frame);
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 run(data_stream_t* stream)
{
	if(current_run_data->flag == 1)
	{
		current_run_data->flag = 0;	
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	:ˢ�´�����ʱ,��Ž������� 
***********************************************************************************************************/
void common_receive_refresh(uint8_t receive_data, commnuication_frame_t *com_pointer)
{
	//ˢ�¼�����
	com_pointer->refresh_num=0;
	//��������
	if(com_pointer->current_num<(CACHE_BUFF_NUM-1))
		com_pointer->receive_buff[com_pointer->current_num++]=receive_data;		
}
/***********************************************************************************************************
* @����	: 
* @����	:���洮������ 
***********************************************************************************************************/
static void common_cache_data(uint16_t buff_long,commnuication_frame_t *com_pointer)
{
	//�жϻ�������Ƿ���
	if(com_pointer->data_pointer<(CACHE_BUFF_LONGTH - 1))
	{
		//��������
		memcpy(com_pointer->data_cache[com_pointer->data_pointer], com_pointer->receive_buff, sizeof(com_pointer->data_cache[com_pointer->data_pointer]));		
		//�������ݳ���
		com_pointer->data_cache_long[com_pointer->data_pointer] = buff_long;
		//������ż�һ
		com_pointer->data_pointer++;		
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: ���ڿ����ж�
***********************************************************************************************************/
static void common_idle_judge(commnuication_frame_t *com_pointer)
{
	com_pointer->refresh_num++;
	if(com_pointer->refresh_num >= IDLE_NUM)
	{
		com_pointer->refresh_num = IDLE_NUM;
		if(com_pointer->current_num > 0)
		{	
			//��������
			common_cache_data(com_pointer->current_num, com_pointer);
			com_pointer->current_num = 0;
		}		
	}
}
/***********************************************************************************************************
* @����	: 
* @����	: UART1�жϺ���
***********************************************************************************************************/
void Uart1_IRQHandler(void)
{
    uint8_t Res;
    if(Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);        //���ж�״̬λ
        Res= Uart_ReceiveData(M0P_UART1);         //���������ֽڣ��ѻ���Ĵ�����ֵ�洢��������
        /*���ݴ��뻺��*/      
        common_receive_refresh(Res,&rs485_commnuication_frame);  
    }	
}
/***********************************************************************************************************
* @����	: 
* @����	:UART0�жϺ��� 
***********************************************************************************************************/
void Uart0_IRQHandler(void)
{
    uint8_t Res;
    if(Uart_GetStatus(M0P_UART0, UartRC))
    {
        Uart_ClrStatus(M0P_UART0, UartRC);        //���ж�״̬λ
        Res= Uart_ReceiveData(M0P_UART0);         //���������ֽڣ��ѻ���Ĵ�����ֵ�洢��������
        /*���ݴ��뻺��*/      
        common_receive_refresh(Res,&rs485_commnuication_frame);   
	}	
}
/***********************************************************************************************************
* @����	: 
* @����	:boot���� 
***********************************************************************************************************/
static uint8_t check_boot_command(uint8_t *u8RxData
,uint32_t length)
{
	#include "flash.h"
	volatile static uint8_t u8TxData[]={0x6D,0xAC,0x00,0xFF,0x0A,0x00,0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCF,0x68};
	if(length < 18)
		return 0;
	if ((u8RxData[0]==0x6D)&&(u8RxData[1]==0xAC)&&(u8RxData[6]==0x26)&&(u8RxData[16]==0xA6)&&(u8RxData[17]==0xDA)) //��APP����֡
	{	
		/*�ظ�����ָ��*/
		current_stream.cmd =  18;
		memcpy(current_stream.buff, (char*)u8TxData, 18);			
		/*ִ��д����*/
		current_operation->write(&current_stream);		
        /*boot para����д���ֵ*/
        Flash_SectorErase(0x1100);
		Flash_WriteWord(0x1100, 0x67890123);	
        //�����λMCU        
		NVIC_SystemReset();  
	}
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: �������ӳ��ָ�� �˲��� �û������޸� 
***********************************************************************************************************/
void system_mapping_set(u8* receive_buff , u8 receive_length )
{

	/*�������ӳ��ָ�� �˲��� �û������޸�*/
	//�ж�֡ͷ ��������   �ܳ���
	if((receive_buff[0] == 0x7e)
	&&(receive_buff[2] == COM_MAPPING)
	&&(receive_length == 0X10))
	{
		//�ж�CRC8У��      AA  ����У��
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			memcpy(system_data.mapping , &receive_buff[4] , 8);
			system_data.standby_time =receive_buff[12];
			system_data.button_type =receive_buff[13];
			/*дFLASH*/
			flash_data_write();	
		}	
	}	
}
/***********************************************************************************************************
* @����	: 
* @����	:���������ɫָ�� �˲��� �û������޸�  
***********************************************************************************************************/
void system_rgb_set(u8* receive_buff , u8 receive_length )
{
	/*���������ɫָ�� �˲��� �û������޸�*/
	//�ж�֡ͷ ��������   �ܳ���
	if((receive_buff[0] == 0x7e)
	&&(receive_buff[2] == COM_RGB)
	&&(receive_length == 0X12))
	{
		//�ж�CRC8У��      AA  ����У��
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			memcpy(&system_data.work_on[0],&receive_buff[4],12);
			system_data.check[0] =0;
			system_data.check[1] =1;
			system_data.check[2] =2;
			system_data.check[3] =3;
			/*дFLASH*/
			flash_data_write();		
		}	
	}	
}
/***********************************************************************************************************
* @����	: 
* @����	: �����ڲ�Э�� �û������޸�
***********************************************************************************************************/
void factory_fixed_agreement(u8* receive_buff , u8 receive_length )
{
	/*������尴��ӳ���ϵ*/
	system_mapping_set(receive_buff ,  receive_length );	
	/*���������ɫָ��*/
	system_rgb_set(receive_buff ,  receive_length );
}
/***********************************************************************************************************
* @����	: 
* @����	:��ͨ���ƿ���  channelͨ�� 1~8   value��ֵ 1�� 0�� ֻ����user_agreement_analysis�����ڲ�����
***********************************************************************************************************/
void single_channel_lamp_control_instructions(u8 channel , u8 value)
{
	analysis_pointer[0] = SINGLE_CHANNEL;     /*����*/
	analysis_pointer[1] = channel - 1;/*ͨ��*/
	analysis_pointer[2] = value;/*��ֵ*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @����	: 
* @����	: ��ͨ���ƿ��� 
all_stateΪ״̬�����ֽ�Ϊ8����״̬ÿ��BIT����һ����BIT0�����1 BIT7�����8 �� 1Ϊ��  0Ϊ�� 
ֻ����user_agreement_analysis�����ڲ�����
***********************************************************************************************************/
void multi_channel_lamp_control_instructions(u8 all_state)
{
	analysis_pointer[0] = MULTI_CHANNEL;     	/*����*/
	analysis_pointer[1] = all_state;		/*����*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @����	: 
* @����	: ��ͨ����ѯ  ֻ����user_agreement_analysis�����ڲ�����
***********************************************************************************************************/
void multi_channel_lamp_query_instructions(void)
{
	analysis_pointer[0] = MULTI_ANSWER;     	/*����*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @����	: 
* @����	: ��ͨ��������� channelͨ�� 1~4   value��ֵ 1�� 0�� ֻ����user_agreement_analysis�����ڲ�����
***********************************************************************************************************/
void single_channel_output_control_instructions(u8 channel , u8 value)
{
	analysis_pointer[0] = SINGLE_OUT;     /*����*/
	analysis_pointer[1] = channel - 1;/*ͨ��*/
	analysis_pointer[2] = value;/*��ֵ*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @����	: 
* @����	: ��ͨ��������� ��ǰ4ͨ����Ч��
all_stateΪ״̬�����ֽ�Ϊ8�����״̬ÿ��BIT����һ����BIT0�������1 BIT3�������4 �� 1Ϊ��  0Ϊ�� ����4λ��Ч
ֻ����user_agreement_analysis�����ڲ�����
***********************************************************************************************************/
void multi_channel_output_control_instructions(u8 all_state)
{
	analysis_pointer[0] = MULTI_OUT;     	/*����*/
	analysis_pointer[1] = all_state;		/*����*/
	analysis_flag = 1;
}
/***********************************************END*****************************************************/

