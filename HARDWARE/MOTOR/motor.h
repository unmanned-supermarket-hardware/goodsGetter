#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"


#define TEMP_PWM  2500
#define GET_MOTOR 0
#define DROP_MOTOR 1

#define FAST_VELOCITY_MS 0.2  //��Ŀ��λ�þ������ SLOW_RANGE ʱ����FAST_VELOCITY_MS  ��/�� ���ٶ��ƶ�
#define SLOW_VELOCITY_MS 0.1   //��Ŀ��λ�þ���С�� SLOW_RANGE ʱ��SLOW_VELOCITY_MS  ��/�� ���ٶ��ƶ�
#define SLOW_RANGE 0.05  //�쵽�Ļ���  SLOW_RANGE  m ��ʱ�����
#define EQUAL_RANGE 0.005  //�����ݲ����С�ڸ�ֵ�Ŀɺ��Բ��ƣ�
/*
ȡ����Ԫ�й����������
ȡ���������Ϊ��getMotor, ��usart3����
ж���������Ϊ��dropMotor,��uart4����

��ȡ�����δ���ʱλ��Ϊ0��������ʱ�ٶ�Ϊ��������ʱ�ٶ�Ϊ��
��ж�����δ̧��ʱλ��Ϊ0������λ��ʼ��Ϊ����̧��ʱ�ٶ�Ϊ��������ʱ�ٶ�Ϊ��
 */
//------------------------------����--------------------------------------------
extern u8 reset_cmd[10] ;
extern u8 velocity_mode_cmd[10] ;
extern u8 velocity_position_mode_cmd[10];
extern u8 stop_cmd[10] ;
extern u8 receive_data_cmd[10] ;
extern u8 stop_receive_data_cmd[10] ;
extern u8 check_online_cmd[10] ;

extern u8 set_get_motor_velocity_cmd[10] ;
extern u8 set_drop_motor_velocity_cmd[10] ;
extern u8 set_get_motor_v_p_cmd[10] ;
extern u8 set_drop_motor_v_p_cmd[10] ;

//--------------------------���ڵ�״̬
extern double current_depth_in_m;
//------------------------------����--------------------------------------------
//�ײ㺯��
void motor_send_cmd(u8 motor,u8 *cmd);
void motor_reset(u8 motor);

void motor_enter_velocity_mode(u8 motor);
void motor_enter_velocity_position_mode(u8 motor);


void motor_set_velocity(u8 motor,double velocityInMs);

void motor_check_on_line(u8 motor);

//�ϲ㺯��

int goTo(double depthInM);

void fallDown(void);
void raiseUp(void);
//--------------------------------
void motor_stop(u8 motor);
void motor_init(u8 motor);
				    
#endif

















