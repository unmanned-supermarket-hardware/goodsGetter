#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
/*
�͵�ƽ��Ч��
�г̿���COM�ӵ͵�ƽ
NO��PA6 ��PA7
*/

#define TOP_KEY_PRES	1		//TOP_KEY����
#define BOTTOM_KEY_PRES	2		//BOTTOM_KEY����


#define TOP_KEY  PAin(6)   	//PC5  
#define BOTTOM_KEY  PAin(7)	 	//PA15 

void key_init(void);		//IO��ʼ��
u8 key_scan(void);		//����ɨ�躯��
#endif
