/*
 * w5500_conf.h
 *
 *  Created on: Apr 29, 2020
 *      Author: LiZhiqiang
 */

#ifndef THIRD_PARTY_W5500_INC_W5500_CONF_H_
#define THIRD_PARTY_W5500_INC_W5500_CONF_H_

#include "Types.h"
typedef  void (*pFunction)(void);

extern uint8    remote_ip[4];                            	  /* 远端IP地址                   */
extern uint16   remote_port;                            	  /* 远端端口号                   */
extern uint16   local_port;                             	  /* 定义本地端口                 */
extern uint8    use_dhcp;                              	    /* 是否使用DHCP获取IP           */
extern uint8    use_eeprom;                             	  /* 是否使用EEPROM中的IP配置信息 */

/***************----- W5500 GPIO定义 -----***************/
#define W5500_SCS		GPIO_PIN_8	//定义W5500的CS引脚
#define W5500_SCS_PORT	GPIOA

#define W5500_RST		GPIO_PIN_4	//定义W5500的RST引脚
#define W5500_RST_PORT	GPIOA

#define W5500_INT		GPIO_PIN_3	//定义W5500的INT引脚
#define W5500_INT_PORT	GPIOA

#define FW_VER_HIGH  						1
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460       			            /*定义每个数据包的大小*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0       			              /*使用初始定义的IP信息*/
#define IP_FROM_DHCP	          1       			              /*使用DHCP获取IP信息*/
#define IP_FROM_EEPROM	        2       			              /*使用EEPROM定义的IP信息*/
extern uint8_t	ip_from;											                /*选择IP信息配置源*/
#pragma pack(1)
/*此结构体定义了W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG
{
	uint8_t mac[6];																							/*MAC地址*/
	uint8_t lip[4];																							/*local IP本地IP地址*/
	uint8_t sub[4];																							/*子网掩码*/
	uint8_t gw[4];																							/*网关*/
	uint8_t dns[4];																							/*DNS服务器地址*/
	uint8_t rip[4];																							/*remote IP远程IP地址*/
	uint8_t sw_ver[2];																					/*软件版本号*/

}CONFIG_MSG;
#pragma pack()

#pragma pack(1)
/*此结构体定义了eeprom写入的几个变量，可按需修改*/
typedef struct _EEPROM_MSG
{
	uint8_t mac[6];																							/*MAC地址*/
	uint8_t lip[4];																							/*local IP本地IP地址*/
	uint8_t sub[4];																							/*子网掩码*/
	uint8_t gw[4];																							/*网关*/
}EEPROM_MSG_STR;
#pragma pack()

/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32_t addrbsb,  uint8_t data);						/*写入一个8位数据到W5500*/
uint8_t IINCHIP_READ(uint32_t addrbsb);													/*从W5500读出一个8位数据*/
uint16_t wiz_write_buf(uint32_t addrbsb,uint8_t* buf,uint16_t len);	/*向W5500写入len字节数据*/
uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t* buf,uint16_t len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void reset_w5500(void);																			/*硬复位W5500*/
void set_w5500_mac(void);																		/*配置W5500的MAC地址*/
void set_w5500_ip(void);																		/*配置W5500的IP地址*/

#endif /* THIRD_PARTY_W5500_INC_W5500_CONF_H_ */
