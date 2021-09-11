/**
*********************************************************************************************************
*                                        multi_switch_control
*                                      (c) Copyright 2021-2031
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* BSP层只处理简单的IO操作，不负责逻辑控制
*********************************************************************************************************
**/

/*
*********************************************************************************************************
Includes 
*********************************************************************************************************
*/
#include "user_type.h"
#include "gpio.h"
#include "rgb_bsp_hc32l130.h"
#include "spi.h"
/*
*********************************************************************************************************
Define
*********************************************************************************************************
*/
/*spi 数据0电平*/
//#define CODE0 0xC0 
///*spi 数据1电平*/
//#define CODE1 0xFC
#define CODE0 0x80 
///*spi 数据1电平*/
#define CODE1 0xFE

///*面板配置*/
#define USER_SPI_CLK SysctrlPeripheralSpi0
#define USER_SPI 	M0P_SPI0
#define USER_SPI_PORT 	GpioPortA
#define USER_SPI_PIN 	GpioPin7
#define USER_SPI_TYPE 	GpioAf1	
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
u8 send_data_te[50];
/*
*********************************************************************************************************
Function 
*********************************************************************************************************
*/
static u8 read(data_stream_t* stream);
static u8 write(data_stream_t* stream);
static u8 initialization(data_stream_t* stream);
static u8 interrput(data_stream_t* stream);


application_template_t rgb_bsp_operation=
{
	.read=&read,
	.write=&write,
	.initialization=&initialization,
	.interrput=&interrput,
};

/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 read(data_stream_t* stream)
{

	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
void reset_data_flow(void)
{
	unsigned char i;
	Spi_SendData(USER_SPI,0);
	for(i=0;i<50;i++);
}
/***********************************************************************************************************
* @名称	: 
* @描述	:SPI发送一组RGB数值   由于RGB时序严格  调用函数有延时  
***********************************************************************************************************/
void send_rgb_data(uint8_t *dat) 
{
	int8_t ram_i=0;
	/*将RGB数值转换成SPI数据 对应 0 1电平信号*/
	for(ram_i=7;ram_i>=0;ram_i--)
	{
		if((dat[1]&(0x01<<ram_i))!=0)
			send_data_te[7-ram_i]=CODE1;
		else
			send_data_te[7-ram_i]=CODE0;
	}
	for(ram_i=7;ram_i>=0;ram_i--)
	{
		if((dat[0]&(0x01<<ram_i))!=0)
			send_data_te[8+7-ram_i]=CODE1;
		else
			send_data_te[8+7-ram_i]=CODE0;
	}
	for(ram_i=7;ram_i>=0;ram_i--)
	{
		if((dat[2]&(0x01<<ram_i))!=0)
			send_data_te[16+7-ram_i]=CODE1;
		else
			send_data_te[16+7-ram_i]=CODE0;
	}
	
	/*连续发送24个 SPI 数据*/
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE); 
	USER_SPI->DATA=send_data_te[0];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[1];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[2];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[3];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[4];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[5];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[6];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[7];

	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[8];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[9];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[10];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[11];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[12];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[13];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[14];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[15];

	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[16];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[17];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[18];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[19];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[20];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[21];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[22];
	while(Spi_GetStatus(USER_SPI, SpiTxe) == FALSE);
	USER_SPI->DATA=send_data_te[23];
	
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 write(data_stream_t* stream)
{
	/*总线复位*/
	reset_data_flow();
	/*关总中断*/
	__disable_irq();
	/*写入8组数据*/	
	for(u8 i = 0;i < 8;i++)
	{
		send_rgb_data(&stream->buff[i*3]);
		send_rgb_data(&stream->buff[i*3]);			
	}
	/*开总中断*/	
	__enable_irq();				
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
    stc_spi_cfg_t  SpiInitStruct;
	/*SPI1 时钟初始化 */
    Sysctrl_SetPeripheralGate(USER_SPI_CLK,TRUE);
	/*SPI1 初始化 需要匹配48MHZ主频*/
    SpiInitStruct.enSpiMode = SpiMskMaster;   
    SpiInitStruct.enPclkDiv = SpiClkMskDiv8;  
    SpiInitStruct.enCPHA    = SpiMskCphasecond;
    SpiInitStruct.enCPOL    = SpiMskcpollow;  
    Spi_Init(USER_SPI, &SpiInitStruct);
	
	
    stc_gpio_cfg_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);

	
	/*SPI1 PA1引脚初始化*/
    stcGpioCfg.enDrv = GpioDrvH;
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(USER_SPI_PORT, USER_SPI_PIN,&stcGpioCfg);
    Gpio_SetAfMode(USER_SPI_PORT, USER_SPI_PIN,USER_SPI_TYPE);        
	/*SPI1 设置CS*/
	
    Spi_SetCS(USER_SPI, FALSE);	
	
	return 0;
}
/***********************************************************************************************************
* @名称	: 
* @描述	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	
	return 0;
}

/***********************************************END*****************************************************/

