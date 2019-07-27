  #include "relay.h"

/*
继电器COM连12V
NO连电磁铁一边，电磁铁另一边连GND
继电器跳线帽选择低电平触发，IN连PA8
*/

u8 stick_state = STICK_DEFALT;

//初始化PA8为输出口.并使能这两个口的时钟		    
//PA8连红色LED的同时连继电器，灯亮的同时触发（低电平触发）
void relay_init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 

	GPIOA->CRL &= 0X0000FFFF;  	 
	GPIOA->CRL |= 0X33330000;  //PA4 5 6 7  推挽输出   
	GPIOA->ODR|=1<<4;      //PA8 输出高 //低电平触发
	GPIOA->ODR|=1<<5;
	GPIOA->ODR|=1<<6;
	GPIOA->ODR|=1<<7;
	
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 推挽输出   	 
  GPIOA->ODR|=1<<8;      //PA8 输出高 //低电平触发
											  
}


void setMagnet(u8 state)
{
	RELAY_MAGNET = state;
}

void reachOut()
{
	RELAY_RED_LOW = !TRIG;
	RELAY_BLACK_HIGH = !TRIG;
	delay_ms(200);
	RELAY_RED_HIGH = TRIG;
	RELAY_BLACK_LOW = TRIG;
	stick_state = STICK_REACHING_OUT;
	
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	stick_state = STICK_OUT;
	printf("complete reaching out");
}
void drawBack()
{
	RELAY_RED_HIGH = !TRIG;
	RELAY_BLACK_LOW = !TRIG;
	delay_ms(200);
	RELAY_RED_LOW = TRIG;
	RELAY_BLACK_HIGH = TRIG;
	stick_state = STICK_DRAWING_BACK;
		
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	stick_state = STICK_DEFALT;
	printf("complete drawing back");
}

void hang()
{
	RELAY_RED_HIGH = !TRIG;
	RELAY_BLACK_LOW = !TRIG;
	delay_ms(200);
	RELAY_RED_LOW = !TRIG;
	RELAY_BLACK_HIGH = !TRIG;
}

void push()
{
	reachOut();
	hang();
	drawBack();
}