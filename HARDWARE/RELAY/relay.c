  #include "relay.h"

/*
�̵���COM��12V
NO�������һ�ߣ��������һ����GND
�̵�������ñѡ��͵�ƽ������IN��PA8
*/
//��ʼ��PA8Ϊ�����.��ʹ���������ڵ�ʱ��		    
//PA8����ɫLED��ͬʱ���̵�����������ͬʱ�������͵�ƽ������
void relay_init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 


	
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 �������   	 
   GPIOA->ODR|=1<<8;      //PA8 ����� //�͵�ƽ����
											  
}


void setMagnet(u8 state)
{
	RELAY = state;
}



