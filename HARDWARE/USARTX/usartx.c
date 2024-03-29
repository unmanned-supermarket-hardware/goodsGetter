
#include "usartx.h"
//----------------------------------------------------------串口2：连接测距模块-------------------------------------------------------------//
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  //RCC->APB2ENR 见中文参考手册 p70  port A - G的时钟使能 分别在第2-8位，USART1在14位
	RCC->APB1ENR|=1<<17;  //使能串口时钟 //RCC->APB1ENR 见中文参考手册 p72  USART2-5的时钟使能 分别在第17-20位
	
	/*CRL & CRH
	中文参考手册p113
	 每个io口4位，
	低两位是mode：
	00输入，01输出10MHz，10输出2MHz，11输出50MHz，
	高两位是CNF：00模拟输入，01浮空输入 。10上拉下拉输入，11保留  ，
	00：通用推挽输出模式
01：通用开漏输出模式
10：复用功能推挽输出模式
11：复用功能开漏输出模式

串口配置时，RX为1000，TX为1011
	*/
	GPIOA->CRL&=0XFFFF00FF;  
	GPIOA->CRL|=0X00008B00;//IO状态设置
	
	/*ODR p115
	似乎可以不用配置
	端口输出数据（可用来配置上拉下拉）
	串口配置时，RX为上拉输入，对应位配置为1
	*/
	GPIOA->ODR|=1<<3;	  
	/*
	p67
	APB1RSTR 的 第17-20分别是串口2-5的复位
	*/
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//组2，最低优先级 
	
	USART2->SR;  //这句一定要加
}
/**************************************************************************
函数功能：串口2接收中断
连接到红外，获取红外值并赋值给current_depth
入口参数：无
返回  值：无
**************************************************************************/
u8 USART2_RX_BUF[64]; //接收到的数据
u16 USART2_RX_STA=0; 
u8 USART2_COUNT = 0;
double d2 =0;
char d2Str[5];
u8 USART2_RX_FLAG = 0;
extern double current_depth_in_m;
extern u32 distanceModuleMonitor ;
extern int is_distance_right;
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

int USART2_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];
	if(USART2->SR&(1<<5))//接收到数据
	{	      
			res =USART2->DR;
		USART2_RX_FLAG = 1;
		//usart2_send(res);
			if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
				if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
					if(res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
					else 
					{
						USART2_RX_STA|=0x8000;	//接收完成了
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
						//usart1_sendString((char *)USART2_RX_BUF,USART2_COUNT+1);
						//-------------------
						USART2_RX_BUF[(USART2_RX_STA&0X3FFF)-2] = '\0';
						strncpy(d2Str,(char *)(USART2_RX_BUF+2),6);
						sprintf(strTemp,"%s\r\n",d2Str);
						
						//usart1_sendString(strTemp,strlen(strTemp));
						distanceModuleMonitor = 0; //每接到一组数据就清零
						if(USART2_RX_BUF[0] =='D')
						{
				
							current_depth_in_m = atof(d2Str);
							//current_depth_mm = 5;
							
							is_distance_right = 1;
						}
							
						else
						{
							current_depth_in_m = -1;
							is_distance_right = 1;
						}
						//-------------------

						USART2_RX_STA = 0;
						USART2_COUNT = 0;
					}
					
				}else //还没收到0X0D
				{	
					if(res==0x0d)
					{
						USART2_RX_STA|=0x4000;
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
						USART2_RX_STA++;
						USART2_COUNT ++;
					}
					else
					{
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
						USART2_RX_STA++;
						USART2_COUNT ++;
						if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}
	}
return 0;	
}

/**************************实现函数**********************************************
*功    能:		usart2发送一个字节
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
/**************************实现函数**********************************************
*功    能:		usart2发送一个字符串
*********************************************************************************/
void usart2_sendString(char *data,u8 len)
{
	int i=0;
	
	USART2->CR1 &=~(1<<2);  //屏蔽接收
	USART2_RX_STA =0;    //如果接收到一半就放弃这组数据
	USART2_COUNT =0;
	

	for(i=0;i<len;i++)
	{
		USART2->DR = data[i];
		while((USART2->SR&0x40)==0);	
	}
	USART2->CR1 |=1<<2;   //重新开启接收
}

//----------------------------------------------------------串口3：连取货电机-------------------------------------------------------------//

/**************************************************************************
函数功能：串口3初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	

	RCC->APB2ENR|=1<<0;    //开启辅助时钟  这一位是保留的，不知道要他何用
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟   左移位数：a 2    d 5
	RCC->APB1ENR|=1<<18;  //使能串口时钟 	 左移位数： 2-》17    5-》20    推出4-》19
	
	GPIOB->CRH&=0XFFFF00FF;   //
	GPIOB->CRH|=0X00008B00;//IO状态设置
	//GPIOB->ODR|=1<<11;	 


	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//组2，最低优先级 
	
	USART3->SR;
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
u8 USART3_RX_BUF[64]; //接收到的数据
u16 USART3_RX_STA=0; 
u8 USART3_COUNT = 0;
u8 new_motor_data = 0;

	short int real_current  ;
	short int real_velocity ;
	int real_position ;

#define U3_RX_IDLE 0
#define BYTE1 1
#define BYTE2 2
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

int USART3_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];

	if(USART3->SR&(1<<5))//接收到数据
	{	      
			res =USART3->DR;
		switch(USART3_RX_STA)
		{
			case(U3_RX_IDLE): {if(res == 0x23) USART3_RX_STA = BYTE1; break;}
			case(BYTE1):			{if(res == 0x0B) USART3_RX_STA = BYTE2; USART3_COUNT = 1;break;}
			case(BYTE2):			
			{
				USART3_COUNT++;
				USART3_RX_BUF[USART3_COUNT] = res;
				if(USART3_COUNT >= 9)
				{
					real_current = 	(USART3_RX_BUF[2]<<8)|USART3_RX_BUF[3];
					real_velocity = (USART3_RX_BUF[4]<<8)|USART3_RX_BUF[5];
					real_position = (USART3_RX_BUF[6]<<24)|(USART3_RX_BUF[7]<<16)|(USART3_RX_BUF[8]<<8)|USART3_RX_BUF[9];
					USART3_RX_STA = U3_RX_IDLE;
					new_motor_data = 1;
				}
			}
		}
			


	}
return 0;	
}
/**************************实现函数**********************************************
*功    能:		usart3发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}
/**************************实现函数**********************************************
*功    能:		usart3发送一个字符串
*********************************************************************************/

void usart3_sendString(char *data,u8 len)
{
	int i=0;
	USART3->CR1 &=~(1<<2);  //屏蔽接收
	USART3_RX_STA =0;    //如果接收到一半就放弃这组数据
	USART3_COUNT =0;
	
	
	for(i=0;i<len;i++)
	{
		USART3->DR = data[i];
		while((USART3->SR&0x40)==0);	
	}
	USART3->CR1 |=1<<2;   //重新开启接收
}
//----------------------------------------------------------串口4：原定连卸货电机，现在不需要了-------------------------------------------------------------//

/**************************************************************************
函数功能：串口4初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart4_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	

	RCC->APB2ENR|=1<<0;    //开启辅助时钟
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  
	RCC->APB1ENR|=1<<19;  //使能串口时钟   //使能串口5时钟 	  2-》17    5-》20    推出4-》19
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO状态设置
	//GPIOC->ODR|=1<<11;	 


	RCC->APB1RSTR|=1<<19;   //复位串口4
	RCC->APB1RSTR&=~(1<<19);//停止复位	   	   
	//波特率设置
 	UART4->BRR=mantissa; // 波特率设置	 
	UART4->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	UART4->CR1|=1<<8;    //PE中断使能
	UART4->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,UART4_IRQn,2);//组2，最低优先级 
	UART4->SR;
}

/**************************************************************************
函数功能：串口4接收中断
入口参数：无
返回  值：无
**************************************************************************/
u8 UART4_RX_BUF[64]; //接收到的数据
u16 UART4_RX_STA=0; 
u8 UART4_COUNT = 0;
char d4Str[5];
double d4=0;
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

int UART4_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];
	if(UART4->SR&(1<<5))//接收到数据
	{	      
			res =UART4->DR;
			//sprintf(strTemp,"u4:%c\n",res);
			//uart4_sendString(strTemp,strlen(strTemp));
	}
return 0;	
}
/**************************实现函数**********************************************
*功    能:		uart4发送一个字节
*********************************************************************************/
void uart4_send(u8 data)
{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);	
}
/**************************实现函数**********************************************
*功    能:		uart4发送一个字符串
*********************************************************************************/
void uart4_sendString(char *data,u8 len)
{
	int i=0;
	UART4->CR1 &=~(1<<2);  //屏蔽接收
	UART4_RX_STA =0;    //如果接收到一半就放弃这组数据
	UART4_COUNT =0;
	
	
	for(i=0;i<len;i++)
	{
		UART4->DR = data[i];
		while((UART4->SR&0x40)==0);	
	}
	UART4->CR1 |=1<<2;   //重新开启接收
}
//----------------------------------------------------------串口5：连线性模组-------------------------------------------------------------//

/**************************实现函数**********************************************
*功    能:		uart5初始化
*********************************************************************************/
//TX PC12
//RX PD2
void uart5_init(u32 pclk1,u32 bound)	
//串口 2，3，4，5 用pclk1 36m  调用格式uart_init5 (36,9600);    //9600波特率pclk1 =36m
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<5;    //使能PORTd时钟
	RCC->APB2ENR|=1<<4;    //使能PORTc时钟  

	RCC->APB1ENR|=1<<20;  //使能串口5时钟 	  2-》17    5-》20    推出4-》19
	
			  
	RCC->APB1RSTR|=1<<20;   //复位串口4
	RCC->APB1RSTR&=~(1<<20);//停止复位
		   	   
	//波特率设置
 	UART5->BRR=mantissa; // 波特率设置	 
	UART5->CR1|=0X200C;  //1位停止,无校验位.
   
	//IO状态设置 RX PD2 接收脚输入 上拉   11-8:1000
	GPIOD->CRL&=~(0X00000007<<8);//IO状态设置 接收脚输入 上拉     
	GPIOD->CRL|=0x00000001<<11;//IO状态设置 接收脚输入 上拉 
	//输入上拉
	//GPIOD->ODR|=1<<2;	  //PD2上拉
	
  
	//IO状态设置 发送脚TX PC12 复用！！！！推挽输出,输出11     19-16: 1011
	GPIOC->CRH&=~(0X00000001<<18);         // 
	GPIOC->CRH|=0X0000000B<<16;              //
	 
	
	UART5->CR1|=1<<8;    //PE中断使能   这个8和5好像每个串口都一样
	UART5->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,UART5_IRQn,2);//抢占优先级是3 响应优先级是3 组2，最低优先级 
	
	UART5->SR;
}


/**************************************************************************
函数功能：串口5接收中断,连接线性模组
入口参数：无
返回  值：无
**************************************************************************/

u8 new_zmodule_msg = 0;
u8 UART5_JSON_BUF[256]; //接收到的数据
u8 UART5_RX_STA=0; 
u16 UART5_JSON_SIZE = 0;
u16 UART5_JSON_INDEX = 0;
u8	UART5_JSON_CRC = 0;
#define UART_IDLE 0
#define WELL 1 //收到#
#define EXCLAMATION 2 //收到！
#define HIGH_SIZE 3
#define LOW_SIZE 4
#define JSON_END 5
#define STAR 6
#define CRC_CHECK 7
#define AND 8

int UART5_IRQHandler(void)
{	
		u8 res; 
    char strTemp[64];

    if(UART5->SR&(1<<5))   //接收到数据
    {    
        res=UART5->DR;
        switch(UART5_RX_STA)
        {
            case(UART_IDLE):    {if(res == '#')             UART5_RX_STA = WELL;                   break;}
            case(WELL):             {if(res == '!')             UART5_RX_STA = EXCLAMATION;    break;}
            case(EXCLAMATION):{UART5_JSON_SIZE = res<<8;   UART5_RX_STA = HIGH_SIZE;      break;}
            case(HIGH_SIZE):    {UART5_JSON_SIZE += res;       UART5_RX_STA = LOW_SIZE;           break;}
            case(LOW_SIZE):     
            {
                if(UART5_JSON_INDEX < UART5_JSON_SIZE -1)
                {
                    UART5_JSON_BUF[UART5_JSON_INDEX]=res; 
                    UART5_JSON_INDEX++;
                }
                else if(UART5_JSON_INDEX == UART5_JSON_SIZE -1) //最后一个字节了
                {
                    UART5_JSON_BUF[UART5_JSON_INDEX]=res; 
                    UART5_JSON_BUF[UART5_JSON_SIZE]= '\0'; 
                    UART5_JSON_INDEX = 0;
                    UART5_RX_STA = JSON_END;
                }                   
                break;
            }
						case(JSON_END):     {if(res == '*')                             UART5_RX_STA = STAR;               break;}
            case(STAR):             {UART5_JSON_CRC = res;             			UART5_RX_STA = CRC_CHECK;      break;}
            case(CRC_CHECK):  
            {
                if(res == '&')
                {
                    UART5_RX_STA = UART_IDLE; 
                    new_zmodule_msg = 1;
                }
                break;
            }
        }
			}				
	return 0;	
}

/**************************实现函数**********************************************
*功    能:		uart5发送一个字节
*********************************************************************************/
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	
}


/**************************实现函数**********************************************
*功    能:		uart5发送一个字符串
*********************************************************************************/
void uart5_sendString(char * data ,u8 len)
{
	int i=0;
	UART5->CR1 &=~(1<<2);  //屏蔽接收
	UART5_RX_STA =0;    //如果接收到一半就放弃这组数据

	
	
	for(i=0;i<len;i++)
	{
		UART5->DR = data[i];
		while((UART5->SR&0x40)==0);
	}
	UART5->CR1 |=1<<2;   //重新开启接收
}

