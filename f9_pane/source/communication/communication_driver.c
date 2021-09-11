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
* @名称	: 
* @描述	: 读取缓存数据
***********************************************************************************************************/
u8 common_data_read(commnuication_frame_t* com_pointer,data_stream_t* stream)
{
	if(com_pointer->data_pointer>0)
	{	
		// 清空缓存     // 把  0   把数据  给  data_cache
		memset(com_pointer->receive_buff, 0,sizeof(com_pointer->receive_buff));  
		//清空解析BUFF  用0  填充 数组  analysis_buff
		memset(com_pointer->analysis_buff, 0,sizeof(com_pointer->analysis_buff));
		//从缓存中拷贝数据到解析analysis_buff
		memcpy(com_pointer->analysis_buff, com_pointer->data_cache[com_pointer->data_pointer-1], sizeof(com_pointer->analysis_buff));
		//BOOT串口指令判断
		check_boot_command((uint8_t*)&com_pointer->analysis_buff[0],sizeof(com_pointer->analysis_buff));	
		//拷贝  数组 analysis_buff  数据长度
		com_pointer->analysis_long = com_pointer->data_cache_long[com_pointer->data_pointer-1];
		// 清空缓存     
		memset(com_pointer->data_cache[com_pointer->data_pointer-1], 0, sizeof(com_pointer->data_cache[com_pointer->data_pointer-1]));	
        //  数组 data_cache_long  数据长度  清零
		com_pointer->data_cache_long[com_pointer->data_pointer-1] =0 ;
		 // 把数据长度  减一  
		com_pointer->data_pointer--;	
		//将结息数据输出到数据流中	
		stream->cmd = com_pointer->analysis_long;
		memcpy(stream->buff, com_pointer->analysis_buff, sizeof(stream->buff));	
		//清空解析BUFF  用0  填充 数组  analysis_buff
		memset(com_pointer->analysis_buff, 0, sizeof(com_pointer->analysis_buff));      
		return 1;
	}	
	return 0;	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
u8 *analysis_pointer = current_stream.buff;
static u8 read( data_stream_t* stream)
{
	/*执行串口数据读处理*/
	if(common_data_read(&rs485_commnuication_frame, &current_stream))
	{
		analysis_pointer = stream->hex;
		/*执行协议解析*/
		if(user_agreement_analysis(current_stream.buff, current_stream.cmd))
		 return 1;		
	}
    return 0; 
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void communication_485_data_send(u8 *send_buff, u8 send_length)
{
	data_stream_t send_stream;
	send_stream.cmd = send_length;
	memcpy(send_stream.buff , send_buff , send_length);
	/*执行写处理*/
	communication_bsp_operation.write(&send_stream);
}
/******************************************
函数名称：
功    能： 发送数据 
instructions 为程序产生的指令
send_buff 为用户需要发送的内容
send_length 为用户发送的数据长度

instructions[0]： 1为单灯设置操作  2 为多灯设置操作  0F为多灯状态查询 
instructions[1]： 当analysis_buff[0]为1时本字节为灯通道值    当analysis_buff[0]为2时本字节为8个灯总状态每个BIT代表一个灯BIT0代表灯1 BIT7代表灯8 1为开  0为关
instructions[2]： 当analysis_buff[0]为1时本字节为灯状态  1为开  0为关

参    数：
返回值  ：
*******************************************/
/***********************************************************************************************************
* @名称	: 
* @描述	: 
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
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 write( data_stream_t* stream)
{
	/*执行协议打包处理*/
	user_agreement_pack(stream->hex , current_stream.buff , &current_stream.cmd);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
	current_operation->initialization(stream);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	current_run_data->flag = 1;
	common_idle_judge(&rs485_commnuication_frame);
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
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
* @名称	: 
* @描述	:刷新串口延时,存放接收数据 
***********************************************************************************************************/
void common_receive_refresh(uint8_t receive_data, commnuication_frame_t *com_pointer)
{
	//刷新计数器
	com_pointer->refresh_num=0;
	//缓存数据
	if(com_pointer->current_num<(CACHE_BUFF_NUM-1))
		com_pointer->receive_buff[com_pointer->current_num++]=receive_data;		
}
/***********************************************************************************************************
* @名称	: 
* @描述	:缓存串口数据 
***********************************************************************************************************/
static void common_cache_data(uint16_t buff_long,commnuication_frame_t *com_pointer)
{
	//判断缓存队列是否满
	if(com_pointer->data_pointer<(CACHE_BUFF_LONGTH - 1))
	{
		//拷贝数据
		memcpy(com_pointer->data_cache[com_pointer->data_pointer], com_pointer->receive_buff, sizeof(com_pointer->data_cache[com_pointer->data_pointer]));		
		//保存数据长度
		com_pointer->data_cache_long[com_pointer->data_pointer] = buff_long;
		//数据序号加一
		com_pointer->data_pointer++;		
	}
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 串口空闲判断
***********************************************************************************************************/
static void common_idle_judge(commnuication_frame_t *com_pointer)
{
	com_pointer->refresh_num++;
	if(com_pointer->refresh_num >= IDLE_NUM)
	{
		com_pointer->refresh_num = IDLE_NUM;
		if(com_pointer->current_num > 0)
		{	
			//缓存数据
			common_cache_data(com_pointer->current_num, com_pointer);
			com_pointer->current_num = 0;
		}		
	}
}
/***********************************************************************************************************
* @名称	: 
* @描述	: UART1中断函数
***********************************************************************************************************/
void Uart1_IRQHandler(void)
{
    uint8_t Res;
    if(Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);        //清中断状态位
        Res= Uart_ReceiveData(M0P_UART1);         //接收数据字节，把缓存寄存器的值存储在数组中
        /*数据存入缓冲*/      
        common_receive_refresh(Res,&rs485_commnuication_frame);  
    }	
}
/***********************************************************************************************************
* @名称	: 
* @描述	:UART0中断函数 
***********************************************************************************************************/
void Uart0_IRQHandler(void)
{
    uint8_t Res;
    if(Uart_GetStatus(M0P_UART0, UartRC))
    {
        Uart_ClrStatus(M0P_UART0, UartRC);        //清中断状态位
        Res= Uart_ReceiveData(M0P_UART0);         //接收数据字节，把缓存寄存器的值存储在数组中
        /*数据存入缓冲*/      
        common_receive_refresh(Res,&rs485_commnuication_frame);   
	}	
}
/***********************************************************************************************************
* @名称	: 
* @描述	:boot处理 
***********************************************************************************************************/
static uint8_t check_boot_command(uint8_t *u8RxData
,uint32_t length)
{
	#include "flash.h"
	volatile static uint8_t u8TxData[]={0x6D,0xAC,0x00,0xFF,0x0A,0x00,0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCF,0x68};
	if(length < 18)
		return 0;
	if ((u8RxData[0]==0x6D)&&(u8RxData[1]==0xAC)&&(u8RxData[6]==0x26)&&(u8RxData[16]==0xA6)&&(u8RxData[17]==0xDA)) //是APP更新帧
	{	
		/*回复握手指令*/
		current_stream.cmd =  18;
		memcpy(current_stream.buff, (char*)u8TxData, 18);			
		/*执行写处理*/
		current_operation->write(&current_stream);		
        /*boot para区域写标记值*/
        Flash_SectorErase(0x1100);
		Flash_WriteWord(0x1100, 0x67890123);	
        //软件复位MCU        
		NVIC_SystemReset();  
	}
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 配置面板映射指令 此部分 用户不能修改 
***********************************************************************************************************/
void system_mapping_set(u8* receive_buff , u8 receive_length )
{

	/*配置面板映射指令 此部分 用户不能修改*/
	//判断帧头 数据类型   总长度
	if((receive_buff[0] == 0x7e)
	&&(receive_buff[2] == COM_MAPPING)
	&&(receive_length == 0X10))
	{
		//判断CRC8校验      AA  万能校验
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			memcpy(system_data.mapping , &receive_buff[4] , 8);
			system_data.standby_time =receive_buff[12];
			system_data.button_type =receive_buff[13];
			/*写FLASH*/
			flash_data_write();	
		}	
	}	
}
/***********************************************************************************************************
* @名称	: 
* @描述	:配置面板颜色指令 此部分 用户不能修改  
***********************************************************************************************************/
void system_rgb_set(u8* receive_buff , u8 receive_length )
{
	/*配置面板颜色指令 此部分 用户不能修改*/
	//判断帧头 数据类型   总长度
	if((receive_buff[0] == 0x7e)
	&&(receive_buff[2] == COM_RGB)
	&&(receive_length == 0X12))
	{
		//判断CRC8校验      AA  万能校验
		if(CRC8_Check(receive_buff,receive_buff[3])||(receive_buff[receive_buff[3]-2]==0XAA))
		{		
			memcpy(&system_data.work_on[0],&receive_buff[4],12);
			system_data.check[0] =0;
			system_data.check[1] =1;
			system_data.check[2] =2;
			system_data.check[3] =3;
			/*写FLASH*/
			flash_data_write();		
		}	
	}	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 厂家内部协议 用户不能修改
***********************************************************************************************************/
void factory_fixed_agreement(u8* receive_buff , u8 receive_length )
{
	/*配置面板按键映射关系*/
	system_mapping_set(receive_buff ,  receive_length );	
	/*配置面板颜色指令*/
	system_rgb_set(receive_buff ,  receive_length );
}
/***********************************************************************************************************
* @名称	: 
* @描述	:单通道灯控制  channel通道 1~8   value数值 1开 0关 只能在user_agreement_analysis函数内部调用
***********************************************************************************************************/
void single_channel_lamp_control_instructions(u8 channel , u8 value)
{
	analysis_pointer[0] = SINGLE_CHANNEL;     /*类型*/
	analysis_pointer[1] = channel - 1;/*通道*/
	analysis_pointer[2] = value;/*数值*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 多通道灯控制 
all_state为状态，本字节为8个灯状态每个BIT代表一个灯BIT0代表灯1 BIT7代表灯8 ， 1为开  0为关 
只能在user_agreement_analysis函数内部调用
***********************************************************************************************************/
void multi_channel_lamp_control_instructions(u8 all_state)
{
	analysis_pointer[0] = MULTI_CHANNEL;     	/*类型*/
	analysis_pointer[1] = all_state;		/*数据*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 多通道查询  只能在user_agreement_analysis函数内部调用
***********************************************************************************************************/
void multi_channel_lamp_query_instructions(void)
{
	analysis_pointer[0] = MULTI_ANSWER;     	/*类型*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 单通道输出控制 channel通道 1~4   value数值 1开 0关 只能在user_agreement_analysis函数内部调用
***********************************************************************************************************/
void single_channel_output_control_instructions(u8 channel , u8 value)
{
	analysis_pointer[0] = SINGLE_OUT;     /*类型*/
	analysis_pointer[1] = channel - 1;/*通道*/
	analysis_pointer[2] = value;/*数值*/
	analysis_flag = 1;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 多通道输出控制 （前4通道有效）
all_state为状态，本字节为8个输出状态每个BIT代表一个灯BIT0代表输出1 BIT3代表输出4 ， 1为开  0为关 ，高4位无效
只能在user_agreement_analysis函数内部调用
***********************************************************************************************************/
void multi_channel_output_control_instructions(u8 all_state)
{
	analysis_pointer[0] = MULTI_OUT;     	/*类型*/
	analysis_pointer[1] = all_state;		/*数据*/
	analysis_flag = 1;
}
/***********************************************END*****************************************************/

