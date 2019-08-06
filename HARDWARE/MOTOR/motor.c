#include "sys.h"   
#include "motor.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/05
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


//------------------------------����--------------------------------------------
u8 reset_cmd[10] = 									{0x23 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 velocity_mode_cmd[10] = 					{0x23 ,0x01 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55	};
u8 velocity_position_mode_cmd[10] =	{0x23 ,0x01 ,0x05 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55	};
u8 stop_cmd[10] = 									{0x23 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 receive_data_cmd[10] = 					{0x23 ,0x0A ,0xFF ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };  //����λ�Ƿ��͵����ڣ���λΪmm����������ѡ���256
u8 stop_receive_data_cmd[10] = 			{0x23 ,0x0A ,0x00 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 check_online_cmd[10] = 					{0x23 ,0x0F ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };


u8 set_velocity_cmd[2][10] =			{{ 0x23 ,0x04 ,0x13 ,0x88 ,0x01 ,0xF4 ,0x55 ,0x55 ,0x55 ,0x55	},
																		{0x23 ,0x04 ,0x13 ,0x88 ,0x01 ,0xF4 ,0x55 ,0x55 ,0x55 ,0x55	}};



u8 set_velocity_position_cmd[2][10]={{0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	},
																		{0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	}};

//------------------------------��ǰ״̬--------------------------------------------																		
double current_depth_in_m = 0;
//------------------------------����--------------------------------------------
/**************************ʵ�ֺ���**********************************************
*��    ��:		�������Ϳ�������
*********************************************************************************/																		
void motor_send_cmd(u8 motor,u8 *cmd)
{
	if(motor == GET_MOTOR)
		usart3_sendString((char *)cmd,10) ;
	else
		uart4_sendString((char *)cmd,10) ;
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		�����λ
*********************************************************************************/
//��λ,��ҪԼ500ms
void motor_reset(u8 motor)
{
		motor_send_cmd(motor,reset_cmd);
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		�����ٶ�ģʽ
*********************************************************************************/
void motor_enter_velocity_mode(u8 motor)
{
		motor_send_cmd(motor,velocity_mode_cmd) ;	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		�����ٶ�λ��ģʽ
*********************************************************************************/
void motor_enter_velocity_position_mode(u8 motor)
{
		motor_send_cmd(motor,velocity_position_mode_cmd) ;	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		��ʼ������λ + �����ٶ�ģʽ
*********************************************************************************/
void motor_init(u8 motor)
{
	motor_reset(motor);
	delay_ms(550);
	motor_enter_velocity_mode(motor);
	delay_ms(200);
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		�����ٶ�
*********************************************************************************/
double diameterInM = 0.03864;   //����ֱ��
#define pi 3.14
//diameterInM * pi = 0.1213296
//�����ٶ�
//�������͵�ʱ��������С�� 2 ���룬������10 ����Ϊ���ڡ�
void motor_set_velocity(u8 motor,double  velocityInMs)
{
	int velocityInRpm = (int)(velocityInMs * 960 / (diameterInM * pi));  //here Ҫ���е�λת��
	int i;
	set_velocity_cmd[motor][2] = (u8)((TEMP_PWM>>8)&0xff);
	set_velocity_cmd[motor][3] = (u8)(TEMP_PWM&0xff);
	set_velocity_cmd[motor][4] = (u8)((velocityInRpm>>8)&0xff);
	set_velocity_cmd[motor][5] = (u8)(velocityInRpm&0xff);
	
	//printf("rpm:%d, cmd:%x%x\n",velocityInRpm,set_velocity_cmd[motor][4],set_velocity_cmd[motor][5]);
	printf("\n");
	for(i=0;i<10;i++)
		printf("%2x ",set_velocity_cmd[motor][i]);
	printf("\n");
	motor_send_cmd(motor,set_velocity_cmd[motor]) ;	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		ֹͣ��������setvelocityΪ0����ֹ�߳�Ӱ��ͨ�ţ�������ʹ���������Ϊ��pwvҲ��Ϊ��0��
*********************************************************************************/
void motor_stop(u8 motor)
{
	motor_send_cmd(motor,stop_cmd) ;	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		������Ƿ����ӳɹ�����δʹ��
*********************************************************************************/
void motor_check_on_line(u8 motor)
{
	motor_send_cmd(motor,check_online_cmd) ;	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		���� ȡ����� �ƶ���ĳ��λ�ã�ֹͣ�����ǲ��ģ��⵽�����ݴﵽĳ����
*********************************************************************************/
//����ȡ������ƶ���ĳ��λ��
//depthInMm��ȡ������ƶ�����λ�õĻ�����ȣ���λ��Mm

#define STOP -1
#define FAR2FAR 0
#define NEAR2FAR 1
#define FAR2BACK 2
#define NEAR2BACK 3

#define DIR_FAR -1
#define DIR_BACK 1


int state = STOP;
int goToByLight(double destinationInM)
{
	double distance2Go;
	printf("goto");
	
	while(1)   //�����ʾ��û���Ǹ�λ��
	{
		
		if(current_depth_in_m < 0 )  //���⴫��������
		{
			printf("error");
			motor_stop(GET_MOTOR);
			return -1;
		}
		
		distance2Go = destinationInM - current_depth_in_m;
		//���ȥ
		if(distance2Go > 0)
		{
			if(distance2Go >SLOW_RANGE && state != FAR2FAR)  //�����Զ��������
			{
				printf("FAR2FAR");
				motor_set_velocity(GET_MOTOR,FAST_VELOCITY_MS * DIR_FAR);
				
			
				state = FAR2FAR;
			}
			else if(distance2Go > EQUAL_RANGE && distance2Go < SLOW_RANGE && state!= NEAR2FAR)   
			{
				printf("NEAR2FAR");
				motor_set_velocity(GET_MOTOR,SLOW_VELOCITY_MS* DIR_FAR);
			
				state = NEAR2FAR;
			}
			else if(distance2Go < EQUAL_RANGE && distance2Go > -EQUAL_RANGE )   
			{
				printf("arrive");
				motor_stop(GET_MOTOR);
				
				state = STOP;
				return 1;
			}
		}
		//�½�
		else if(distance2Go < 0) 
		{
			if(-distance2Go >SLOW_RANGE && state != FAR2BACK)  //�����Զ��������
			{
				printf("FAR2BACK");
				motor_set_velocity(GET_MOTOR,FAST_VELOCITY_MS * DIR_BACK);
				
				state = FAR2BACK;
			}
			else if(-distance2Go > EQUAL_RANGE && -distance2Go < SLOW_RANGE && state!= NEAR2BACK)   
			{
				printf("NEAR2BACK");
				motor_set_velocity(GET_MOTOR,SLOW_VELOCITY_MS* DIR_BACK);
			
				state = NEAR2BACK;
			}
			else if(-distance2Go < EQUAL_RANGE && distance2Go > -EQUAL_RANGE )   
			{
				printf("arrive");
				motor_stop(GET_MOTOR);
 
				state = STOP;
				return 1;
			}
		}
	}
	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		ʹȡ����Ԫ�ջص�ͷ��������ֱ��������λ���أ�
*********************************************************************************/
int goToByKey()
{
	motor_set_velocity(GET_MOTOR,SLOW_VELOCITY_MS * DIR_BACK);
	while(key_scan() != KEY_PRES)
	{
		;
	}
	motor_set_velocity(GET_MOTOR,0);
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		ʹȡ����Ԫ��ָ���ٶ��ƶ���ָ��λ�ã������ݲ��ģ��õ��ľ�������
*********************************************************************************/
extern 	short int real_current  ;
extern 	short int real_velocity ;
extern 	int real_position ;
int goToByMotor(double destinationInM)
{
	u8 Data[10];
	u32 test;
	int velocityInRpm = (int)(SLOW_VELOCITY_MS * 960 / (diameterInM * pi));  //here Ҫ���е�λת��
	int positionInQc =(destinationInM /  (diameterInM * pi) ) * 32000 ;
	
	motor_reset(GET_MOTOR);
	delay_ms(600);
	motor_enter_velocity_position_mode(GET_MOTOR);
	
	
	//��ָ�������Ϣ
	motor_send_cmd(GET_MOTOR,receive_data_cmd);
	
	//��ָ��������

	set_velocity_position_cmd[GET_MOTOR][4] = (unsigned char)((velocityInRpm>>8)&0xff);
	set_velocity_position_cmd[GET_MOTOR][5] = (unsigned char)(velocityInRpm&0xff);
	
	set_velocity_position_cmd[GET_MOTOR][6] = (unsigned char)((positionInQc>>24)&0xff);
	set_velocity_position_cmd[GET_MOTOR][7] = (unsigned char)((positionInQc>>16)&0xff);
	set_velocity_position_cmd[GET_MOTOR][8] = (unsigned char)((positionInQc>>8)&0xff);
	set_velocity_position_cmd[GET_MOTOR][9] = (unsigned char)(positionInQc &0xff);
	motor_send_cmd(GET_MOTOR,set_velocity_position_cmd[GET_MOTOR]);
	
	while(real_velocity == 0);

	while(real_position != positionInQc);
	
	printf("arrive!!!!!\n");
	//��ָ�ֹͣ������Ϣ
	motor_send_cmd(GET_MOTOR,stop_receive_data_cmd);
}

