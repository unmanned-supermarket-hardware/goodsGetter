#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 

//ALIENTEK Mini STM32�����巶������3
//����ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
# define DATA_LEN 13

extern u8 startOpen,startClose,isOpen;
extern u8 USART2_RX_FLAG;
extern u8 USART2_RX_BUF[64]; //���յ�������
extern u8 USART3_RX_BUF[64]; //���յ�������
extern u8 UART5_RX_BUF[64]; //���յ�������





int main(void)
{			

	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ�� 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart4_init(36,115200);
	uart5_init(36,115200);
	relay_init();
	//key_init();
	TIM2_Int_Init(60000,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ5000ms  6S
	TIM3_Int_Init(5000,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	
	
	delay_ms(1000);
	

	//�����ʼ��
	printf("���ģ���ʼ��");
	delay_ms(500);
	usart2_sendString("iFACM:0",7);
	delay_ms(500);


	//ȡ�������ʼ��
	printf("�����ʼ��");
	delay_ms(500);
	motor_reset(GET_MOTOR);  //��֪Ϊ�ε�һ�鷢�ͻ�ȱͷһ���ֽڣ���˷�������
	delay_ms(600);
	motor_reset(DROP_MOTOR);  //��֪Ϊ�ε�һ�鷢�ͻ�ȱͷһ���ֽڣ���˷�������
	delay_ms(600);
	
	motor_enter_velocity_mode(GET_MOTOR);
	delay_ms(500);
	motor_enter_velocity_mode(DROP_MOTOR);
	delay_ms(500);
	
	reachOut();
	hang();
	drawBack();
	
	while(1)
	{
		
		//printf("bottom_key  = %d top_key = %d",is_bottom_key_pressed(),is_bottom_key_pressed());
		//printf("%d\n",key_scan());
//		goTo(0.82);
//		setMagnet(MAGNET_ON);
//		delay_ms(1000);
//		goTo(0.62);
//		setMagnet(MAGNET_OFF);
//		delay_ms(1000);
		



	}	 
}














