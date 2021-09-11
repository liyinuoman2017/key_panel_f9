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


/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/

/*ģʽ*/
#define SWITCH_MODE   		(10)
#define ADJUST_MODE   		(11)
#define THYRISTOR_MODE		(12)
#define PANEL_MODE			(13)
/*�û�ģʽѡ��*/
#define USER_MODE 	  	PANEL_MODE 


/*��尴������ ѡ�����ģʽ�� ��Ҫѡ�񰴼�����*/
#define NUM_8  8
#define NUM_4  4
#define PANEL_NUM  NUM_8

#define PANEL  	0       //��ͨ���ģʽ
#define X4  	1       //X4Һ�����ģʽ
#define PANEL_TYPE  PANEL



/*�������������*/
#define DATA_STREAM_BUFF_MAX 50

#define SINGLE_CHANNEL 1
#define MULTI_CHANNEL  2

#define SINGLE_OUT	   3
#define MULTI_OUT	   4

#define STANDBY_CHANNEL  3
#define MULTI_ANSWER  0X0F

#define DIAL_SWITCH_NUM 6
#define OUTPUT_NUM 4
#define BUTTON_NUM 8

#define RGB_OFF 	0	
#define RGB_ON 		1
#define RGB_TWINKLE 2

#define BUTTON_NEGATION 1
#define BUTTON_TWIKLE	0
/*
*********************************************************************************************************
Typedef
*********************************************************************************************************
*/
/*��������*/
typedef  int	  int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int	  uint32_t;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

/*ͨ������������*/
typedef struct data_stream_def
{		   								
	u8 cmd;
	u8 buff[DATA_STREAM_BUFF_MAX];
	u8 hex[DATA_STREAM_BUFF_MAX];
}data_stream_t;

#define STREAM_CACHE_MAX 10
/*ͨ������������*/
typedef struct data_stream_cache_def
{		   								
	u8 pointer;
	data_stream_t data[STREAM_CACHE_MAX];
}data_stream_cache_t;

/*��������ģ��*/
typedef  u8 (*fun_template_t)(data_stream_t* stream);

/*�����ģ��*/
typedef struct application_template_def
{	
	/*��ȡ���ݲ���*/	
	fun_template_t read;
	/*��ȡ���ݲ���*/
	fun_template_t write;
	/*��ʼ������*/
	fun_template_t initialization;
	/*�жϲ���  ���ڼ��� �ñ�־λ  �����жϺ����� ��ʵʱ��*/
	fun_template_t interrput;
	/*�������в��� */
	fun_template_t  run;
}application_template_t;


typedef struct run_data_def
{
	u16 ms_clk;		
	u8 s_clk;
	u8 flag;
	u16 delay;

}run_data_t;

/*�߼�ҵ��Ա*������Ϣ*/
typedef struct logic_data_frame_def
{
	u8 add;
	u8 button_channel;
	u8 led_state[BUTTON_NUM];
	u8 output_state[OUTPUT_NUM];
	u8 output_mapping[BUTTON_NUM];  //8������ӳ��
	u8 infrared;
	u8 button_type;

}logic_data_frame_t;
/*
*********************************************************************************************************
Variables
*********************************************************************************************************
*/
extern data_stream_t public_null;
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
void delay(u16 data_del);
/***********************************************END*****************************************************/


