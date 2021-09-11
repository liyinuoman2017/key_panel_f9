/**
*********************************************************************************************************
*                                        multi_switch_control
*                                      (c) Copyright 2021-2031
*                                         All Rights Reserved
*
* @File    : 
* @By      : liwei
* @Version : V0.01
* BSP��ֻ����򵥵�IO�������������߼�����
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
/*spi ����0��ƽ*/
//#define CODE0 0xC0 
///*spi ����1��ƽ*/
//#define CODE1 0xFC
#define CODE0 0x80 
///*spi ����1��ƽ*/
#define CODE1 0xFE

///*�������*/
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
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 read(data_stream_t* stream)
{

	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
void reset_data_flow(void)
{
	unsigned char i;
	Spi_SendData(USER_SPI,0);
	for(i=0;i<50;i++);
}
/***********************************************************************************************************
* @����	: 
* @����	:SPI����һ��RGB��ֵ   ����RGBʱ���ϸ�  ���ú�������ʱ  
***********************************************************************************************************/
void send_rgb_data(uint8_t *dat) 
{
	int8_t ram_i=0;
	/*��RGB��ֵת����SPI���� ��Ӧ 0 1��ƽ�ź�*/
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
	
	/*��������24�� SPI ����*/
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
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 write(data_stream_t* stream)
{
	/*���߸�λ*/
	reset_data_flow();
	/*�����ж�*/
	__disable_irq();
	/*д��8������*/	
	for(u8 i = 0;i < 8;i++)
	{
		send_rgb_data(&stream->buff[i*3]);
		send_rgb_data(&stream->buff[i*3]);			
	}
	/*�����ж�*/	
	__enable_irq();				
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 initialization(data_stream_t* stream)
{
    stc_spi_cfg_t  SpiInitStruct;
	/*SPI1 ʱ�ӳ�ʼ�� */
    Sysctrl_SetPeripheralGate(USER_SPI_CLK,TRUE);
	/*SPI1 ��ʼ�� ��Ҫƥ��48MHZ��Ƶ*/
    SpiInitStruct.enSpiMode = SpiMskMaster;   
    SpiInitStruct.enPclkDiv = SpiClkMskDiv8;  
    SpiInitStruct.enCPHA    = SpiMskCphasecond;
    SpiInitStruct.enCPOL    = SpiMskcpollow;  
    Spi_Init(USER_SPI, &SpiInitStruct);
	
	
    stc_gpio_cfg_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);

	
	/*SPI1 PA1���ų�ʼ��*/
    stcGpioCfg.enDrv = GpioDrvH;
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(USER_SPI_PORT, USER_SPI_PIN,&stcGpioCfg);
    Gpio_SetAfMode(USER_SPI_PORT, USER_SPI_PIN,USER_SPI_TYPE);        
	/*SPI1 ����CS*/
	
    Spi_SetCS(USER_SPI, FALSE);	
	
	return 0;
}
/***********************************************************************************************************
* @����	: 
* @����	: 
***********************************************************************************************************/
static u8 interrput(data_stream_t* stream)
{
	
	return 0;
}

/***********************************************END*****************************************************/

