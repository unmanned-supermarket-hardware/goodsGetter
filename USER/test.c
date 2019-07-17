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

	motor_enter_velocity_mode(GET_MOTOR);
	delay_ms(500);
	while(1)
	{
		printf("current_depth_in_m = %f \nd2Str = %s\n\n",current_depth_in_m,USART2_RX_BUF);
		delay_ms(500);
	}	 
}














