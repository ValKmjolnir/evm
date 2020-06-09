/**
******************************************************************************
* @file    			w5500_conf.c                                                                                              
* @version 			V1.2										  		
* @date    			2017-11-01								 
* @brief 				��3.5.0�汾�⽨�Ĺ���ģ��						  	
* @attention		����MCU����ֲw5500������Ҫ�޸ĵ��ļ�������w5500��MAC��IP��ַ 	
*				
* @company  		��������Ƽ����޹�˾
* @information  WIZnet W5500�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website  		www.wisioe.com																							 	 				
* @forum        www.w5500.cn																								 		
* @qqGroup      383035001																										 
******************************************************************************
*/

#include <stdio.h> 
#include <string.h>
#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"


CONFIG_MSG  ConfigMsg;																	/*���ýṹ��*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM�洢��Ϣ�ṹ��*/

/*MAC��ַ���ֽڱ���Ϊż��*/
/*������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*����Ĭ��IP��Ϣ*/
uint8 local_ip[4]={192,168,1,25};				/*����W5500Ĭ��IP��ַ*/
uint8 subnet[4]={255,255,255,0};				/*����W5500Ĭ����������*/
uint8 gateway[4]={192,168,1,1};					/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};			/*����W5500Ĭ��DNS*/

uint16 local_port=8080;	                    	/*���屾�ض˿�*/

/*����Զ��IP��Ϣ*/
uint8  remote_ip[4]={192,168,1,160};			/*Զ��IP��ַ*/
uint16 remote_port=8080;						/*Զ�˶˿ں�*/

/*IP���÷���ѡ��������ѡ��*/
uint8  ip_from=IP_FROM_DEFINE;				

uint8  dhcp_ok=0;								/*dhcp�ɹ���ȡIP*/
uint32 ms=0;									/*�������*/
uint32 dhcp_time= 0;							/*DHCP���м���*/
volatile unsigned char    ntptimer = 0;							/*NPT�����*/

/**
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{	
		
  /*���ƶ����������Ϣ�����ýṹ��*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	// if(ip_from==IP_FROM_DEFINE)	
	// 	printf(" ʹ�ö����IP��Ϣ����W5500:\r\n");

	// /*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/		
	// if(ip_from==IP_FROM_DHCP)								
	// {
	// 	/*����DHCP��ȡ��������Ϣ�����ýṹ��*/
	// 	if(dhcp_ok==1)
	// 	{
	// 		// printf(" IP from DHCP\r\n");		 
	// 		memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
	// 		memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
	// 		memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
	// 		memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
	// 	}
	// 	else
	// 	{
	// 		// printf(" DHCP�ӳ���δ����,���߲��ɹ�\r\n");
	// 		// printf(" ʹ�ö����IP��Ϣ����W5500\r\n");
	// 	}
	// }
		
	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	// printf(" W5500 IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	// printf(" W5500 �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	// printf(" W5500 ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void gpio_for_w5500_config(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// SPI_FLASH_Init();																		 /*��ʼ��STM32 SPI�ӿ�*/
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);
		
	// /*����RESET����*/	
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz*/		
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*��������ģʽΪͨ���������*/		
	// GPIO_Init(GPIOB, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
	// GPIO_SetBits(GPIOB, GPIO_Pin_1);		
	// /*����INT����*/
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz */		
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*��������ģʽΪͨ������ģ����������*/	
	// GPIO_Init(GPIOC, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
}

/**
*@brief		W5500Ƭѡ�ź����ú���
*@param		val: Ϊ��0����ʾƬѡ�˿�Ϊ�ͣ�Ϊ��1����ʾƬѡ�˿�Ϊ��
*@return	��
*/
void wiz_cs(uint8_t val)
{
	#ifdef  STM32_SPI1
		if (val == LOW) 
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_4); 
		}
		else if (val == HIGH)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_4); 
		}
	#endif
	#ifdef STM32_SPI2
		if (val == LOW) 
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
		}
		else if (val == HIGH)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_12); 
		}
	#endif
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
// void reset_w5500(void)
// {
	// GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	// w5500_delay_us(50);  
	// GPIO_SetBits(GPIOB, GPIO_Pin_1);
	// w5500_delay_us(50);
// }


/**
*@brief		STM32��ʱ��2��ʼ��
*@param		��
*@return	��
*/
void timer2_init(void)
{
	TIM2_Configuration();																		/* TIM2 ��ʱ���� */
	TIM2_NVIC_Configuration();															/* ��ʱ�����ж����ȼ� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/* TIM2 ���¿�ʱ�ӣ���ʼ��ʱ */
}

/**
*@brief		dhcp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void dhcp_timer_init(void)
{
	timer2_init();																	
}

/**
*@brief		ntp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void ntp_timer_init(void)
{
	timer2_init();																	
}

/**
*@brief		��ʱ��2�жϺ���
*@param		��
*@return	��
*/
void timer2_isr(void)
{
	ms++;	
	if(ms>=1000)
	{  
		ms=0;
		dhcp_time++;			/*DHCP��ʱ��1S*/
		#ifndef	__NTP_H__
		ntptimer++;				/*NTP����ʱ���1S*/
		#endif
	}

}
/**
*@brief		STM32ϵͳ����λ����
*@param		��
*@return	��
*/
void reboot(void)
{
	pFunction Jump_To_Application;
	uint32 JumpAddress;
	printf(" ϵͳ�����С���\r\n");
	JumpAddress = *(vu32*) (0x00000004);
	Jump_To_Application = (pFunction) JumpAddress;
	Jump_To_Application();
}


