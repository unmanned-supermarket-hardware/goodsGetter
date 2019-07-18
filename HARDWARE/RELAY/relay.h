#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"

//继电器端口定义
#define RELAY PAout(8)	// PA8

#define MAGNET_ON 0
#define MAGNET_OFF 1
void relay_init(void);	//初始化		 		
void setMagnet(u8 state);
#endif

















