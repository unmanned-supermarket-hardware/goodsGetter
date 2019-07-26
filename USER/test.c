#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 



extern u8 startOpen,startClose,isOpen;
extern u8 new_master_msg;
extern u8 new_zmodule_msg;
extern u8 USART1_JSON_BUF[256]; //接收到的数据
extern u8 USART2_JSON_BUF[256]; //接收到的数据
extern u8 USART3_JSON_BUF[256]; //接收到的数据
extern u8 UART5_JSON_BUF[256]; //接收到的数据


double destination_height;
double destination_depth;
int global_state = IDLE;



int main(void)
{			
	
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char *strJson;
	
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
	//delay_ms(500);
	usart2_sendString("iFACM:0",7);
	delay_ms(500);


//	//取货电机初始化
//	printf("电机初始化");
//	delay_ms(500);
//	motor_reset(GET_MOTOR);  //不知为何第一遍发送会缺头一个字节，因此发送两边
//	delay_ms(600);
//	motor_reset(DROP_MOTOR);  //不知为何第一遍发送会缺头一个字节，因此发送两边
//	delay_ms(600);
//	
//	motor_enter_velocity_mode(GET_MOTOR);
//	delay_ms(500);
//	motor_enter_velocity_mode(DROP_MOTOR);
//	delay_ms(500);
	

	//-----------------------------------json模拟区：主控发来取货命令
		
//	
//		root=cJSON_CreateObject();

//		cJSON_AddStringToObject(root,"businessType","0014");
//		cJSON_AddNumberToObject(root,"Height",0.5);
//		cJSON_AddNumberToObject(root,"Depth",0.6);
//		 
//		strSendLen = generate_send_str(root,strSend);
//		
//		
//		usart1_sendString(strSend,strSendLen);
//		
//		strJson =cJSON_Print(root); 
//		printf("json_size = %d\n",strlen(strJson));
//-----------------------------------json模拟区		：模组高度到位命令
//		root=cJSON_CreateObject();

//		cJSON_AddStringToObject(root,"businessType","0023");
//		cJSON_AddNumberToObject(root,"Result",1);

//		
//		strSendLen = generate_send_str(root,strSend);
//		usart1_sendString(strSend,strSendLen);
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
		
		if(new_master_msg)
		{
			//printf("%s\n",USART1_JSON_BUF);
			switch(resolve_master_msg())
			{
				case(MASTER_MSG_CHECK):{break;}
				case(MASTER_MSG_GET):{on_get_good(); break;}
			}
			new_master_msg = 0;
		}
		if(new_zmodule_msg)
		{
			//printf("%s\n",UART5_JSON_BUF);
			switch(resolve_zmodule_msg())
			{
				case(ZMODULE_MSG_ARRIVE_HEIGHT):{on_arrive_height(); break;}
	
			}
			new_zmodule_msg = 0;
		}

	}	 
}













