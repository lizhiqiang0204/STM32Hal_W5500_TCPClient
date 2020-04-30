/*
 * w5500_conf.c
 *
 *  Created on: 2020年4月29日
 *      Author: LiZhiqiang
 */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "w5500_conf.h"
#include "w5500.h"
#include "spi.h"
#include "usart.h"
#include "Types.h"
#include <stdio.h>
#include <string.h>

CONFIG_MSG  ConfigMsg;																	/*配置结构体*/
/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
//uint8_t mac[6]={0x0c,0x29,0xab,0x7c,0x00,0x01};
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*定义默认IP信息*/
uint8_t local_ip[4]  ={192,168,2,11};											/*定义W5500默认IP地址*/
uint8_t subnet[4]    ={255,255,255,0};										/*定义W5500默认子网掩码*/
uint8_t gateway[4]   ={192,168,2,1};											/*定义W5500默认网关*/
uint8_t dns_server[4]={114,114,114,114};									/*定义W5500默认DNS*/

uint16_t local_port=5000;	                       					/*定义本地端口*/

/*定义远端IP信息*/
uint8_t  remote_ip[4]={192,168,2,155};											/*远端IP地址*/
uint16_t remote_port=5000;																/*远端端口号*/

/*IP配置方法选择，请自行选择*/
uint8_t	ip_from=IP_FROM_DEFINE;

uint8_t   dhcp_ok   = 0;													   			/*dhcp成功获取IP*/
uint32_t	ms        = 0;															  	/*毫秒计数*/
uint32_t	dhcp_time = 0;															  	/*DHCP运行计数*/
uint8_t	    ntptimer  = 0;															  	/*NPT秒计数*/

/**
*@brief		配置W5500的IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{

   /*复制定义的配置信息到配置结构体*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
//	if(ip_from==IP_FROM_DEFINE)
//		USART_printf(&huart1," 使用定义的IP信息配置W5500\r\n");
//		printf(" 使用定义的IP信息配置W5500\r\n");

//	/*使用EEPROM存储的IP参数*/
//	if(ip_from==IP_FROM_EEPROM)
//	{
//
//		/*从EEPROM中读取IP配置信息*/
//		read_config_from_eeprom();
//
//		/*如果读取EEPROM中MAC信息,如果已配置，则可使用*/
//		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)
//		{
//			printf(" IP from EEPROM\r\n");
//			/*复制EEPROM配置信息到配置的结构体变量*/
//			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);
//			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
//			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
//		}
//		else
//		{
//			printf(" EEPROM未配置,使用定义的IP信息配置W5500,并写入EEPROM\r\n");
//			write_config_to_eeprom();	/*使用默认的IP信息，并初始化EEPROM中数据*/
//		}
//	}
//
//	/*使用DHCP获取IP参数，需调用DHCP子函数*/
//	if(ip_from==IP_FROM_DHCP)
//	{
//		/*复制DHCP获取的配置信息到配置结构体*/
//		if(dhcp_ok==1)
//		{
//			printf(" IP from DHCP\r\n");
//			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
//			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
//			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
//			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
//		}
//		else
//		{
//			printf(" DHCP子程序未运行,或者不成功\r\n");
//			printf(" 使用定义的IP信息配置W5500\r\n");
//		}
//	}

	/*以下配置信息，根据需要选用*/
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;

	/*将IP配置信息写入W5500相应寄存器*/
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	getSIPR (local_ip);
//	USART_printf(&huart1," W5500 IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
//	printf(" W5500 IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
//	USART_printf(&huart1," W5500 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
//	printf(" W5500 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
//	USART_printf(&huart1," W5500 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
//	printf(" W5500 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
//	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		W5500片选信号设置函数
*@param		val: 为“0”表示片选端口为低，为“1”表示片选端口为高
*@return	无
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW)
	{
		HAL_GPIO_WritePin(W5500_SCS_PORT, W5500_SCS, GPIO_PIN_RESET);//置W5500的SCS为低电平
	}
	else if (val == HIGH)
	{
		HAL_GPIO_WritePin(W5500_SCS_PORT, W5500_SCS, GPIO_PIN_SET); //置W5500的SCS为高电平
	}
}

/**
*@brief		设置W5500的片选端口SCSn为低
*@param		无
*@return	无
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		设置W5500的片选端口SCSn为高
*@param		无
*@return	无
*/
void iinchip_cson(void)
{
   wiz_cs(HIGH);
}

/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
	HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST, GPIO_PIN_RESET);//复位引脚拉低
	osDelay(10);
	HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST, GPIO_PIN_SET);//复位引脚拉高
	osDelay(1600);
}

uint8_t SPI_SendByte(uint8_t byte)
{
	uint8_t res;
	HAL_SPI_TransmitReceive(&hspi1,&byte,&res,1,100);
	return res;
}

/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);
   IINCHIP_SpiSendData(data);
   iinchip_cson();
}

/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);
   iinchip_cson();
   return data;
}

/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) ;
   iinchip_csoff();
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();
   return len;
}

/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {

  }
  iinchip_csoff();
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));
  for(idx = 0; idx < len; idx++)
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();
  return len;
}

