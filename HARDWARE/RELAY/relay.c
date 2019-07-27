  #include "relay.h"

/*
�̵���COM��12V
NO�������һ�ߣ��������һ����GND
�̵�������ñѡ��͵�ƽ������IN��PA8
*/

u8 stick_state = STICK_DEFALT;

//��ʼ��PA8Ϊ�����.��ʹ���������ڵ�ʱ��		    
//PA8����ɫLED��ͬʱ���̵�����������ͬʱ�������͵�ƽ������
void relay_init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 

	GPIOA->CRL &= 0X0000FFFF;  	 
	GPIOA->CRL |= 0X33330000;  //PA4 5 6 7  �������   
	GPIOA->ODR|=1<<4;      //PA8 ����� //�͵�ƽ����
	GPIOA->ODR|=1<<5;
	GPIOA->ODR|=1<<6;
	GPIOA->ODR|=1<<7;
	
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 �������   	 
  GPIOA->ODR|=1<<8;      //PA8 ����� //�͵�ƽ����
											  
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