#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"

//�̵����˿ڶ���
#define RELAY PAout(8)	// PA8

#define MAGNET_ON 0
#define MAGNET_OFF 1
void relay_init(void);	//��ʼ��		 		
void setMagnet(u8 state);
#endif

















