#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ�����ʺ�STM32F10xϵ�У�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.7
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************

 
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
//int fputc(int ch, FILE *f)
//{      
//	while((UART4->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
//	UART4->DR = (u8) ch;      	//дDR,����1����������
//	return ch;
//}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_1_RX_STA=0;       //����״̬���	  
  



/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ֽ�
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ַ���
*********************************************************************************/
void usart1_sendString(char *data,u8 len)
{
	int i=0;
	USART1->SR;
	for(i=0;i<len;i++)
	{
		USART1->DR = data[i];
		while((USART1->SR&0x40)==0);	
	}
	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1�����ж�
*********************************************************************************/
u8 new_master_msg = 0;
u8 USART1_JSON_BUF[256]; //���յ�������
u8 USART1_RX_STA=0; 
u16 USART1_JSON_SIZE = 0;
u16 USART1_JSON_INDEX = 0;
u8	USART1_JSON_CRC = 0;

#define UART_IDLE 0
#define WELL 1 //�յ�#
#define EXCLAMATION 2 //�յ���
#define HIGH_SIZE 3
#define LOW_SIZE 4
#define JSON_END 5
#define STAR 6
#define CRC_CHECK 7
#define AND 8

void USART1_IRQHandler(void)
{
	u8 res;	
	char strTemp[64];
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART1->SR&(1<<5))	//���յ�����
	{	 
		res=USART1->DR;
//		USART1->CR1 &=~(1<<3);  //���η���
		//--------------------------------------
		switch(USART1_RX_STA)
		{
			case(UART_IDLE):	{if(res == '#') 							USART1_RX_STA = WELL;					break;}
			case(WELL):				{if(res == '!') 							USART1_RX_STA = EXCLAMATION;	break;}
			case(EXCLAMATION):{USART1_JSON_SIZE = res<<8; 	USART1_RX_STA = HIGH_SIZE;		break;}
			case(HIGH_SIZE):	{USART1_JSON_SIZE += res; 		USART1_RX_STA = LOW_SIZE;			break;}
			case(LOW_SIZE):		
			{
				if(USART1_JSON_INDEX < USART1_JSON_SIZE -1)
				{
					USART1_JSON_BUF[USART1_JSON_INDEX]=res; 
					USART1_JSON_INDEX++;
				}
				else if(USART1_JSON_INDEX == USART1_JSON_SIZE -1) //JSON�����һ���ֽ���
				{
					USART1_JSON_BUF[USART1_JSON_INDEX]=res; 
					USART1_JSON_BUF[USART1_JSON_SIZE]= '\0'; 
					USART1_JSON_INDEX = 0;
					USART1_RX_STA = JSON_END;
					
				}					
				break;
			}
			case(JSON_END):		{if(res == '*')								USART1_RX_STA = STAR; 				break;}
			case(STAR):				{USART1_JSON_CRC = res; 			USART1_RX_STA = CRC_CHECK;		break;}
			case(CRC_CHECK):  
			{
				if(res == '&')
				{
					USART1_RX_STA = UART_IDLE;
					printf("new");
					new_master_msg = 1;
				}
				break;
			}
		}	
			
		//--------------------------------------
//		USART1->CR1 |=1<<3;  //���¿�������
	}
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1��ʼ��
*********************************************************************************/
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
}

//---------------------------------------------------------------------------------
