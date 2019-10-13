#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化（适合STM32F10x系列）		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************

 
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
//int fputc(int ch, FILE *f)
//{      
//	while((UART4->SR&0X40)==0);//等待上一次串口数据发送完成  
//	UART4->DR = (u8) ch;      	//写DR,串口1将发送数据
//	return ch;
//}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
//u16 USART_1_RX_STA=0;       //接收状态标记	  
  



/**************************实现函数**********************************************
*功    能:		usart1发送一个字节
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}

/**************************实现函数**********************************************
*功    能:		usart1发送一个字符串
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


/**************************实现函数**********************************************
*功    能:		usart1接收中断
*********************************************************************************/
u8 new_master_msg = 0;
u8 USART1_JSON_BUF[256]; //接收到的数据
u8 USART1_RX_STA=0; 
u16 USART1_JSON_SIZE = 0;
u16 USART1_JSON_INDEX = 0;
u8	USART1_JSON_CRC = 0;

#define UART_IDLE 0
#define WELL 1 //收到#
#define EXCLAMATION 2 //收到！
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
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		res=USART1->DR;
//		USART1->CR1 &=~(1<<3);  //屏蔽发送
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
				else if(USART1_JSON_INDEX == USART1_JSON_SIZE -1) //JSON的最后一个字节了
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
//		USART1->CR1 |=1<<3;  //重新开启发送
	}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	
/**************************实现函数**********************************************
*功    能:		usart1初始化
*********************************************************************************/
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
}

//---------------------------------------------------------------------------------
