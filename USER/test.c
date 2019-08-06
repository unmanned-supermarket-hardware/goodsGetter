#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 



extern u8 startOpen,startClose,isOpen;
extern u8 new_master_msg;
extern u8 new_zmodule_msg;
extern u8 new_motor_data;

extern 	short int real_current  ;
extern 	short int real_velocity ;
extern 	int real_position ;

extern u8 USART1_JSON_BUF[256]; //���յ�������
extern u8 USART2_JSON_BUF[256]; //���յ�������
extern u8 USART3_JSON_BUF[256]; //���յ�������
extern u8 UART5_JSON_BUF[256]; //���յ�������


double destination_height;
double destination_depth;
int global_state = IDLE;
int is_distance_receiving = 1;  //��ʾ����һֱ�ڽ�������
int is_distance_right = 1;      //��ʾ�����������������ΪD = ***m������Error
u32 distanceModuleMonitor = 0;

int main(void)
{			
	int i;
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char *strJson;
	double diameterInM = 0.03864;   //����ֱ��
	//ÿ�������յ��º�������ʱ����0�����ڼ������Ƿ���Ȼ�ڴ�����
	
	
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ�� 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart4_init(36,115200);
	uart5_init(36,115200);
	relay_init();
	key_init();

	//�����ʼ��
	printf("���ģ���ʼ��");
	//delay_ms(500);
	usart2_sendString("iFACM:0",7);
	delay_ms(500);


	printf("�����������ˣ�\n");
	//ȡ�������ʼ��
	printf("�����ʼ��");
	delay_ms(500);
	motor_reset(GET_MOTOR);  //��֪Ϊ�ε�һ�鷢�ͻ�ȱͷһ���ֽڣ���˷�������
	delay_ms(600);

	
	motor_enter_velocity_mode(GET_MOTOR);
	delay_ms(500);

	
		//��ʱ����ʼ������ʱ���distance��debug
		//TIM2_Int_Init(60000,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ5000ms  6S
		TIM3_Int_Init(5000,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  

		goToByMotor(diameterInM * 15);
		


	while(1)
	{


//		//���ģ�����
//		distanceModuleMonitor++;
//		if(distanceModuleMonitor>655344)
//		{
//			//printf("too long without distance data received!\n");
//			is_distance_receiving = 0;
//		}
//		else
//			is_distance_receiving = 1;
//		
//		//������ģ�鹤��״̬��������
//		if(!is_distance_receiving || !is_distance_right)
//		{
//			//����˶��߼��ÿ��ǰ�
//			;
//		}
//		//�������ط�������Ϣ
//		if(new_master_msg)
//		{
//			//printf("%s\n",USART1_JSON_BUF);
//			switch(resolve_master_msg())
//			{
//				case(MASTER_MSG_CHECK):{on_check_msg();break;}
//				case(MASTER_MSG_GET):{on_get_good_msg(); break;}
//				case(MASTER_MSG_DROP_GOOD):{on_drop_good_msg();break;}
//				case(MASTER_MSG_DROP_TRAY):{on_drop_tray_msg();break;}
//			}
//			new_master_msg = 0;
//		}
//		//����ģ�鷢������Ϣ
//		if(new_zmodule_msg)
//		{
//			//printf("%s\n",UART5_JSON_BUF);
//			switch(resolve_zmodule_msg())
//			{
//				case(ZMODULE_MSG_ARRIVE_HEIGHT):{on_arrive_height_msg(); break;}
//	
//			}
//			new_zmodule_msg = 0;
//		}

		


	}	 
}













