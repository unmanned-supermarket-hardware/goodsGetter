#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
/*
�͵�ƽ��Ч��
�г̿���COM�ӵ͵�ƽ
NO��PA15��
*/

#define KEY_PRES	1		//KEY����


#define KEY  PAin(15)	 	//PA15 

void key_init(void);		//IO��ʼ��
u8 key_scan(void);		//����ɨ�躯��
#endif
