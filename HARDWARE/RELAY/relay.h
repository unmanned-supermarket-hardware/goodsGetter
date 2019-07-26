#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"

//继电器端口定义
#define RELAY_MAGNET PAout(8)	// PA8

#define RELAY_RED_HIGH PAout(4)		//PA4控制的继电器，COM接12V高压，	NO接红线
#define RELAY_RED_LOW PAout(5)		//PA5控制的继电器，COM接地，			NO接红线		
#define RELAY_BLACK_HIGH PAout(6)	//PA6控制的继电器，COM接12V高压，	NO接黑线
#define RELAY_BLACK_LOW PAout(7)	//PA7控制的继电器，COM接地，			NO接黑线	

#define MAGNET_ON 0  //低电平触发
#define MAGNET_OFF 1

#define TRIG 0  //低电平触发

#define STICK_DEFALT 0
#define STICK_OUT 1
#define STICK_REACHING_OUT 2
#define STICK_DRAWING_BACK 3
extern u8 stick_state;

void relay_init(void);	//初始化		 		
void setMagnet(u8 state); //设置电磁铁状态，state取值为MAGNET_ON或MAGNET_OFF

void reachOut(void); //控制伸缩杆伸出
void drawBack(void); //控制伸缩杆缩回
void hang(void);
#endif

















