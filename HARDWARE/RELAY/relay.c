  #include "relay.h"


//��ʼ��PA8Ϊ�����.��ʹ���������ڵ�ʱ��		    
//PA8����ɫLED��ͬʱ���̵�����������ͬʱ�������͵�ƽ������
void relay_init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 

//	GPIOA->CRL&=0X0FFFFFFF;                   // 0011 ****
//	GPIOA->CRL|=0X30000000;//PA7 �������   	 
//	GPIOA->ODR&=~(1<<7);      //PA7 ����ߣ�������

	
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 �������   	 
   GPIOA->ODR|=1<<8;      //PA8 ����� //�͵�ƽ����
											  
}


void setMagnet(u8 state)
{
	RELAY = state;
}



