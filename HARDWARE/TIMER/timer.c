#include "timer.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
extern u8 USART2_RX_BUF[64]; //接收到的数据
extern 	short int real_current  ;
extern 	short int real_velocity ;
extern 	int real_position ;
extern int global_state;


//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		printf("d2Str = %s\t",USART2_RX_BUF);	
		
		switch(global_state)
		{
			case(0):{printf("idle\n\n");break;}
			case(1):{printf("GOING_TO_HEIGHT，正在等待模组到达位置\n\n");break;}
			case(2):{printf("ARRIVE_HEIGHT模组到了\n\n");break;}
			case(3):{printf("GOT_GOOD取到货物\n\n");break;}
		}
		//printf("%d\t%d\t%d\n",real_current,real_velocity,real_position);
		
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器设置
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}




//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;	//TIM2时钟使能    
 	TIM2->ARR=arr;  	//设定计数器自动重装值 
	TIM2->PSC=psc;  	//预分频器设置
	TIM2->DIER|=1<<0;   //允许更新中断				
	TIM2->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM2_IRQn,2);//抢占1，子优先级3，组2									 
}















