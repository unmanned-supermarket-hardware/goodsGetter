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
int is_distance_receiving = 1;  //表示红外一直在接收数据
int is_distance_right = 1;      //表示红外接收数据正常，为D = ***m，而非Error
u32 distanceModuleMonitor = 0;

int main(void)
{			
	
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char *strJson;
	//每当串口收到新红外数据时就清0，用于检测红外是否仍然在传数据
	
	
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart4_init(36,115200);
	uart5_init(36,115200);
	relay_init();
	//key_init();

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
	
		//定时器初始化：定时输出distance供debug
		//TIM2_Int_Init(60000,7199);//10Khz的计数频率，计数到5000为5000ms  6S
		TIM3_Int_Init(5000,7199);//10Khz的计数频率，计数到5000为500ms  
		//独立看门狗初始化
		//625是1s，溢出时间与参数二成正比
		//IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s	   
		
		
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
//-----------------------------------json模拟区		：主控查询状态
//		root=cJSON_CreateObject();

//		cJSON_AddStringToObject(root,"businessType","0013");


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
		
		
		//测距模块监视
		distanceModuleMonitor++;
		if(distanceModuleMonitor>655344)
		{
			//printf("too long without distance data received!\n");
			is_distance_receiving = 0;
		}
		else
			is_distance_receiving = 1;
		
		//如果测距模块工作状态不正常，
		if(!is_distance_receiving || !is_distance_right)
		{
			//电机运动逻辑得考虑啊
			;
		}
		//处理主控发来的消息
		if(new_master_msg)
		{
			//printf("%s\n",USART1_JSON_BUF);
			switch(resolve_master_msg())
			{
				case(MASTER_MSG_CHECK):{on_check();break;}
				case(MASTER_MSG_GET):{on_get_good(); break;}
				case(MASTER_MSG_DROP_GOOD):{on_drop_good();break;}
				case(MASTER_MSG_DROP_TRAY):{on_drop_tray();break;}
			}
			new_master_msg = 0;
		}
		//处理模组发来的消息
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













