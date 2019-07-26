#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"


#define TEMP_PWM  2500
#define GET_MOTOR 0
#define DROP_MOTOR 1

#define FAST_VELOCITY_MS 0.2  //离目标位置距离大于 SLOW_RANGE 时，以FAST_VELOCITY_MS  米/秒 的速度移动
#define SLOW_VELOCITY_MS 0.1   //离目标位置距离小于 SLOW_RANGE 时，SLOW_VELOCITY_MS  米/秒 的速度移动
#define SLOW_RANGE 0.05  //快到的还有  SLOW_RANGE  m 的时候减速
#define EQUAL_RANGE 0.005  //距离容差（距离小于该值的可忽略不计）
/*
取货单元中共有两个电机
取货电机命名为：getMotor, 用usart3控制
卸货电机命名为：dropMotor,用uart4控制

设取货电机未伸出时位置为0，因此伸出时速度为正，缩回时速度为负
设卸货电机未抬起时位置为0，故其位置始终为正，抬起时速度为正，缩回时速度为负
 */
//------------------------------命令--------------------------------------------
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

//--------------------------现在的状态
extern double current_depth_in_m;
//------------------------------函数--------------------------------------------
//底层函数
void motor_send_cmd(u8 motor,u8 *cmd);
void motor_reset(u8 motor);

void motor_enter_velocity_mode(u8 motor);
void motor_enter_velocity_position_mode(u8 motor);


void motor_set_velocity(u8 motor,double velocityInMs);

void motor_check_on_line(u8 motor);

//上层函数

int goTo(double depthInM);

void fallDown(void);
void raiseUp(void);
//--------------------------------
void motor_stop(u8 motor);
void motor_init(u8 motor);
				    
#endif

















