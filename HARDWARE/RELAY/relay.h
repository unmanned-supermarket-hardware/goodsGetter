#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"

//�̵����˿ڶ���
#define RELAY_MAGNET PAout(8)	// PA8

#define RELAY_RED_HIGH PAout(4)		//PA4���Ƶļ̵�����COM��12V��ѹ��	NO�Ӻ���
#define RELAY_RED_LOW PAout(5)		//PA5���Ƶļ̵�����COM�ӵأ�			NO�Ӻ���		
#define RELAY_BLACK_HIGH PAout(6)	//PA6���Ƶļ̵�����COM��12V��ѹ��	NO�Ӻ���
#define RELAY_BLACK_LOW PAout(7)	//PA7���Ƶļ̵�����COM�ӵأ�			NO�Ӻ���	

#define MAGNET_ON 0  //�͵�ƽ����
#define MAGNET_OFF 1

#define TRIG 0  //�͵�ƽ����

#define STICK_DEFALT 0
#define STICK_OUT 1
#define STICK_REACHING_OUT 2
#define STICK_DRAWING_BACK 3
extern u8 stick_state;

void relay_init(void);	//��ʼ��		 		
void setMagnet(u8 state); //���õ����״̬��stateȡֵΪMAGNET_ON��MAGNET_OFF

void reachOut(void); //�������������
void drawBack(void); //��������������
void hang(void);
#endif

















