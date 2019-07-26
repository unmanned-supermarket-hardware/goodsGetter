#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 



extern u8 startOpen,startClose,isOpen;
extern u8 new_master_msg;
extern u8 new_zmodule_msg;
extern u8 USART1_JSON_BUF[256]; //���յ�������
extern u8 USART2_JSON_BUF[256]; //���յ�������
extern u8 USART3_JSON_BUF[256]; //���յ�������
extern u8 UART5_JSON_BUF[256]; //���յ�������


double destination_height;
double destination_depth;
int global_state = IDLE;



int main(void)
{			
	
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char *strJson;
	
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
	//delay_ms(500);
	usart2_sendString("iFACM:0",7);
	delay_ms(500);


//	//ȡ�������ʼ��
//	printf("�����ʼ��");
//	delay_ms(500);
//	motor_reset(GET_MOTOR);  //��֪Ϊ�ε�һ�鷢�ͻ�ȱͷһ���ֽڣ���˷�������
//	delay_ms(600);
//	motor_reset(DROP_MOTOR);  //��֪Ϊ�ε�һ�鷢�ͻ�ȱͷһ���ֽڣ���˷�������
//	delay_ms(600);
//	
//	motor_enter_velocity_mode(GET_MOTOR);
//	delay_ms(500);
//	motor_enter_velocity_mode(DROP_MOTOR);
//	delay_ms(500);
	

	//-----------------------------------jsonģ���������ط���ȡ������
		
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
//-----------------------------------jsonģ����		��ģ��߶ȵ�λ����
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













