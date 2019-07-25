#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 

//ALIENTEK Mini STM32开发板范例代码3
//串口实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
# define DATA_LEN 13

extern u8 startOpen,startClose,isOpen;
extern u8 USART2_RX_FLAG;
extern u8 USART2_RX_BUF[64]; //接收到的数据
extern u8 USART3_RX_BUF[64]; //接收到的数据
extern u8 UART5_RX_BUF[64]; //接收到的数据





int main(void)
{			

	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart4_init(36,115200);
	uart5_init(36,115200);
	relay_init();
	//key_init();
	TIM2_Int_Init(60000,7199);//10Khz的计数频率，计数到5000为5000ms  6S
	TIM3_Int_Init(5000,7199);//10Khz的计数频率，计数到5000为500ms  
	
	
	delay_ms(1000);
	

	//红外初始化
	printf("测距模块初始化");
	delay_ms(500);
	usart2_sendString("iFACM:0",7);
	delay_ms(500);


	//取货电机初始化
	printf("电机初始化");
	delay_ms(500);
	motor_reset(GET_MOTOR);  //不知为何第一遍发送会缺头一个字节，因此发送两边
	delay_ms(600);
	motor_reset(DROP_MOTOR);  //不知为何第一遍发送会缺头一个字节，因此发送两边
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














