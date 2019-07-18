  #include "relay.h"


//初始化PA8为输出口.并使能这两个口的时钟		    
//PA8连红色LED的同时连继电器，灯亮的同时触发（低电平触发）
void relay_init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 

//	GPIOA->CRL&=0X0FFFFFFF;                   // 0011 ****
//	GPIOA->CRL|=0X30000000;//PA7 推挽输出   	 
//	GPIOA->ODR&=~(1<<7);      //PA7 输出高（下拉）

	
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 推挽输出   	 
   GPIOA->ODR|=1<<8;      //PA8 输出高 //低电平触发
											  
}


void setMagnet(u8 state)
{
	RELAY = state;
}



