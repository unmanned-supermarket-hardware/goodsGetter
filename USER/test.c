#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 


extern u8 new_master_msg;
extern u8 new_zmodule_msg;
extern u8 new_motor_data;

extern 	short int real_current  ;
extern 	short int real_velocity ;
extern 	int real_position ;

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
	int i;
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char *strJson;
//	double diameterInM = 0.03864;   //齿轮直径
	//每当串口收到新红外数据时就清0，用于检测红外是否仍然在传数据
	
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart4_init(36,115200);
	uart5_init(36,115200);
	relay_init();
	key_init();

	//红外初始化
	//printf("测距模块初始化");
	delay_ms(500);
	usart2_sendString("iFACM:0",7);
	delay_ms(500);


	printf("朋友我重启了！\n");
	//取货电机初始化
	//printf("电机初始化");
	delay_ms(1000);
	delay_ms(1000);
	motor_reset(GET_MOTOR);  //不知为何第一遍发送会缺头一个字节，因此发送两边
	delay_ms(1000);

	
	motor_enter_velocity_mode(GET_MOTOR);
	delay_ms(500);

	
	//定时器初始化：定时输出distance供debug
	//TIM2_Int_Init(60000,7199);//10Khz的计数频率，计数到5000为5000ms  6S
	TIM3_Int_Init(5000,7199);//10Khz的计数频率，计数到5000为500ms  

//	goToByLight(0.2);
//	goToByKey();

//push();
//setMagnet(MAGNET_ON);
//delay_ms(1000);
//delay_ms(1000);
//delay_ms(1000);
//setMagnet(MAGNET_OFF);
	//motor_set_velocity(GET_MOTOR,DIR_FAR * SLOW_VELOCITY_MS);


      root=cJSON_CreateObject();

      cJSON_AddStringToObject(root,"businessType","0014");
      cJSON_AddNumberToObject(root,"Height",1.15);
			cJSON_AddNumberToObject(root,"Depth",0.079);

      
      strSendLen = generate_send_str(root,strSend);
      usart1_sendString(strSend,strSendLen);
		//motor_set_velocity(GET_MOTOR,0.2);
		
//		motor_set_velocity(GET_MOTOR,FAST_VELOCITY_MS * DIR_FAR);
//			goToByLight(0.70);
//			delay_ms(1000);
//			goToByKey();
//		setMagnet(MAGNET_ON);
//		delay_ms(1000);
//		setMagnet(MAGNET_OFF);
		//goToByLight(0.3);
		//push();
		//	reachOut();
//			drawBack();
		printf("start\n");
	while(1)
	{


		//测距模块监视
		distanceModuleMonitor++;
		if(distanceModuleMonitor>955344)
		{
			printf("一段时间没收到距离数据\n");
			is_distance_receiving = 0;
			usart2_sendString("iFACM:0",7);
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
			printf("-------------------------------receive data---------------------\n");
			switch(resolve_master_msg())
			{
				case(MASTER_MSG_CHECK):{on_check_msg();break;}
				case(MASTER_MSG_GET):{on_get_good_msg(); break;}
				case(MASTER_MSG_DROP_GOOD):{on_drop_good_msg();break;}
				case(MASTER_MSG_DROP_TRAY):{on_drop_tray_msg();break;}
			}
			new_master_msg = 0;
		}
		//处理模组发来的消息
		if(new_zmodule_msg)
		{
			printf("---------------------------data from module-----------------");
			//printf("%s\n",UART5_JSON_BUF);
			switch(resolve_zmodule_msg())
			{
				case(ZMODULE_MSG_ARRIVE_HEIGHT):{on_arrive_height_msg(); break;}
	
			}
			new_zmodule_msg = 0;
		}

	}	 
}













