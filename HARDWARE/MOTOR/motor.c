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
u8 stop_cmd[10] = 									{0x23 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 receive_data_cmd[10] = 					{0x23 ,0x0A ,0xFF ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };  //第三位是发送的周期，单位为mm，我们这里选最大256
u8 stop_receive_data_cmd[10] = 			{0x23 ,0x0A ,0x00 ,0x00 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
u8 check_online_cmd[10] = 					{0x23 ,0x0F ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };


u8 set_velocity_cmd[2][10] =			{{ 0x23 ,0x04 ,0x13 ,0x88 ,0x01 ,0xF4 ,0x55 ,0x55 ,0x55 ,0x55	},
																		{0x23 ,0x04 ,0x13 ,0x88 ,0x01 ,0xF4 ,0x55 ,0x55 ,0x55 ,0x55	}};



u8 set_velocity_position_cmd[2][10]={{0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	},
																		{0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	}};

//------------------------------当前状态--------------------------------------------																		
double current_depth_in_m = 0;
//------------------------------函数--------------------------------------------
/**************************实现函数**********************************************
*功    能:		向电机发送控制命令
*********************************************************************************/																		
void motor_send_cmd(u8 motor,u8 *cmd)
{
	if(motor == GET_MOTOR)
		usart3_sendString((char *)cmd,10) ;
	else
		uart4_sendString((char *)cmd,10) ;
}

/**************************实现函数**********************************************
*功    能:		电机复位
*********************************************************************************/
//复位,需要约500ms
void motor_reset(u8 motor)
{
		motor_send_cmd(motor,reset_cmd);
}

/**************************实现函数**********************************************
*功    能:		进入速度模式
*********************************************************************************/
void motor_enter_velocity_mode(u8 motor)
{
		motor_send_cmd(motor,velocity_mode_cmd) ;	
}


/**************************实现函数**********************************************
*功    能:		进入速度位置模式
*********************************************************************************/
void motor_enter_velocity_position_mode(u8 motor)
{
		motor_send_cmd(motor,velocity_position_mode_cmd) ;	
}

/**************************实现函数**********************************************
*功    能:		初始化：复位 + 进入速度模式
*********************************************************************************/
void motor_init(u8 motor)
{
	motor_reset(motor);
	delay_ms(550);
	motor_enter_velocity_mode(motor);
	delay_ms(200);
}

/**************************实现函数**********************************************
*功    能:		设置速度
*********************************************************************************/
double diameterInM = 0.03864;   //齿轮直径
#define pi 3.14
//diameterInM * pi = 0.1213296
//设置速度
//连续发送的时间间隔不能小于 2 毫秒，建议以10 毫秒为周期。
void motor_set_velocity(u8 motor,double  velocityInMs)
{
	int velocityInRpm = (int)(velocityInMs * 960 / (diameterInM * pi));  //here 要进行单位转换
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

/**************************实现函数**********************************************
*功    能:		停止（不建议setvelocity为0，防止线长影响通信，更建议使用这个，因为把pwv也设为了0）
*********************************************************************************/
void motor_stop(u8 motor)
{
	motor_send_cmd(motor,stop_cmd) ;	
}

/**************************实现函数**********************************************
*功    能:		检查电机是否连接成功，暂未使用
*********************************************************************************/
void motor_check_on_line(u8 motor)
{
	motor_send_cmd(motor,check_online_cmd) ;	
}


/**************************实现函数**********************************************
*功    能:		控制 取货电机 移动到某个位置，停止条件是测距模块测到的数据达到某条件
*********************************************************************************/
//控制取货电机移动到某个位置
//depthInMm：取货电机移动到的位置的货物深度，单位：Mm

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
	
	while(1)   //红外表示还没到那个位置
	{
		
		if(current_depth_in_m < 0 )  //红外传感器错误
		{
			printf("error");
			motor_stop(GET_MOTOR);
			return -1;
		}
		
		distance2Go = destinationInM - current_depth_in_m;
		//伸出去
		if(distance2Go > 0)
		{
			if(distance2Go >SLOW_RANGE && state != FAR2FAR)  //还差的远，快点儿走
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
		//下降
		else if(distance2Go < 0) 
		{
			if(-distance2Go >SLOW_RANGE && state != FAR2BACK)  //还差的远，快点儿走
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

/**************************实现函数**********************************************
*功    能:		使取货单元收回到头（往回缩直到触发限位开关）
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

/**************************实现函数**********************************************
*功    能:		使取货单元以指定速度移动到指定位置，不依据测距模块得到的距离数据
*********************************************************************************/
extern 	short int real_current  ;
extern 	short int real_velocity ;
extern 	int real_position ;
int goToByMotor(double destinationInM)
{
	u8 Data[10];
	u32 test;
	int velocityInRpm = (int)(SLOW_VELOCITY_MS * 960 / (diameterInM * pi));  //here 要进行单位转换
	int positionInQc =(destinationInM /  (diameterInM * pi) ) * 32000 ;
	
	motor_reset(GET_MOTOR);
	delay_ms(600);
	motor_enter_velocity_position_mode(GET_MOTOR);
	
	
	//发指令：反馈信息
	motor_send_cmd(GET_MOTOR,receive_data_cmd);
	
	//发指令让他动

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
	//发指令：停止反馈信息
	motor_send_cmd(GET_MOTOR,stop_receive_data_cmd);
}

