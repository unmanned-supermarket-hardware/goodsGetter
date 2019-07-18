#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
/*
低电平有效，
行程开关COM接低电平
NO接PA6 和PA7
*/

#define TOP_KEY_PRES	1		//TOP_KEY按下
#define BOTTOM_KEY_PRES	2		//BOTTOM_KEY按下


#define TOP_KEY  PAin(6)   	//PC5  
#define BOTTOM_KEY  PAin(7)	 	//PA15 

void key_init(void);		//IO初始化
u8 key_scan(void);		//按键扫描函数
#endif
