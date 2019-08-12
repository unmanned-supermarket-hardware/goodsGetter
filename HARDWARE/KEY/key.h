#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
/*
低电平有效，
行程开关COM接低电平
NO接PA15·
*/

#define KEY_PRES	1		//KEY按下


#define KEY  PAin(15)	 	//PA15 

void key_init(void);		//IO初始化
u8 key_scan(void);		//按键扫描函数
#endif
