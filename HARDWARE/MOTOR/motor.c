#include "sys.h"   
#include "motor.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/05
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


//------------------------------命令--------------------------------------------
u8 reset_cmd[10] = 									{0x23 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 velocity_mode_cmd[10] = 					{0x23 ,0x01 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55	};
u8 velocity_position_mode_cmd[10] =	{0x23 ,0x01 ,0x05 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55	};
u8 stop_cmd[10] = 									{0x23 ,0x04 ,0x13 ,0x88 ,0x00 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 receive_data_cmd[10] = 					{0x23 ,0x0A ,0x09 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 stop_receive_data_cmd[10] = 			{0x23 ,0x0A ,0x00 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 check_online_cmd[10] = 					{0x23 ,0x0F ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };


u8 set_velocity_cmd[2][10] =			{{ 0x23 ,0x04 ,0x13 ,0x88 ,0x01 ,0xF4 ,0x55 ,0x55 ,0x55 ,0x55	},
																		{0x23 ,0x04 ,0x13 ,0x88 ,0x01 ,0xF4 ,0x55 ,0x55 ,0x55 ,0x55	}};


u8 set_velocity_position_cmd[2][10]={{0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	},
																		{0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	}};

//------------------------------当前状态--------------------------------------------																		
double current_depth_in_m = 0;
//------------------------------函数--------------------------------------------
void motor_send_cmd(u8 motor,u8 *cmd)
{
	if(motor == GET_MOTOR)
		usart3_sendString((char *)cmd,10) ;
	else
		uart4_sendString((char *)cmd,10) ;
}

//复位,需要约500ms
void motor_reset(u8 motor)
{
		motor_send_cmd(motor,reset_cmd);
}
//进入速度模式
void motor_enter_velocity_mode(u8 motor)
{
		motor_send_cmd(motor,velocity_mode_cmd) ;	
}

//进入速度位置模式
void motor_enter_velocity_position_mode(u8 motor)
{
		motor_send_cmd(motor,velocity_position_mode_cmd) ;	
}
//初始化：复位 + 进入速度模式
void motor_init(u8 motor)
{
	motor_reset(motor);
	delay_ms(550);
	motor_enter_velocity_mode(motor);
	delay_ms(200);
}

double diameterInM = 0.03864;
#define pi 3.14
//diameterInM * pi = 0.1213296
//设置速度
//连续发送的时间间隔不能小于 2 毫秒，建议以10 毫秒为周期。
void motor_set_velocity(u8 motor,double  velocityInMs)
{
	int velocityInRpm = (int)(velocityInMs * 960 / (diameterInM * pi));  //here 要进行单位转换
	
	set_velocity_cmd[motor][2] = (u8)((TEMP_PWM>>8)&0xff);
	set_velocity_cmd[motor][3] = (u8)(TEMP_PWM&0xff);
	set_velocity_cmd[motor][4] = (u8)((velocityInRpm>>8)&0xff);
	set_velocity_cmd[motor][5] = (u8)(velocityInRpm&0xff);
	
	//printf("rpm:%d, cmd:%x%x\n",velocityInRpm,set_velocity_cmd[motor][4],set_velocity_cmd[motor][5]);
	motor_send_cmd(motor,set_velocity_cmd[motor]) ;	
}

void motor_stop(u8 motor)
{
	motor_send_cmd(motor,stop_cmd) ;	
}
void motor_check_on_line(u8 motor)
{
	motor_send_cmd(motor,check_online_cmd) ;	
}


//控制取货电机移动到某个位置
//depthInMm：取货电机移动到的位置的货物深度，单位：Mm
u8 last_state = -1;
u8 current_state = -1;
u8 state =-1;
#define FAR 0
#define NEAR 1
#define TOO_MUCH 2
void goTo(double depthInM)
{
	printf("goto");
	while(1)   //红外表示还没到那个位置
	{
		
		if(current_depth_in_m < 0 )  //红外传感器错误
		{
			printf("error");
			motor_set_velocity(GET_MOTOR,0);
			return;
		}
		else if(depthInM - current_depth_in_m >SLOW_RANGE && state != FAR)  //离目标差的还远，快速运动
		{
			printf("far");
			motor_set_velocity(GET_MOTOR,FAST_VELOCITY_MS);
			delay_ms(10);
			state = FAR;
		}
		else if(depthInM - current_depth_in_m > EQUAL_RANGE && depthInM - current_depth_in_m < SLOW_RANGE && state!= NEAR)   //还没到，但快到了，减速
		{
			printf("slow");
			motor_set_velocity(GET_MOTOR,SLOW_VELOCITY_MS);
			delay_ms(10);
			state = NEAR;
		}
		else if(depthInM - current_depth_in_m < EQUAL_RANGE && depthInM - current_depth_in_m > -EQUAL_RANGE )   //到了，停下
		{
			printf("arrive");
			motor_set_velocity(GET_MOTOR,0);
			delay_ms(10);
			state = -1;
			return;
		}
		else if(depthInM - current_depth_in_m < -EQUAL_RANGE && state!= TOO_MUCH)  //超了，往回走
		{
			printf("too much");
			motor_set_velocity(GET_MOTOR,-SLOW_VELOCITY_MS);
			delay_ms(10);
			state = TOO_MUCH;
		}
	}
	
	
}

