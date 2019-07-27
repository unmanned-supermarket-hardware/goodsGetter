#ifndef __ZMODULE_COMMUNICATION_H
#define __ZMODULE_COMMUNICATION_H	 
#include "sys.h"
//���ط���ȡ����Ԫ����Ϣ��businessType
#define ZMODULE_MSG_WRONG_BUSINESS_TYPE  -1
#define ZMODULE_MSG_WRONG_JSON  -2
#define ZMODULE_MSG_WRONG_CRC -3
#define ZMODULE_MSG_ARRIVE_HEIGHT 23


extern double destination_height;
extern double destination_depth;
extern int global_state ;

extern u8 UART5_JSON_BUF[256]; //���յ�������
extern u8 UART5_JSON_CRC;

int resolve_zmodule_msg(void);  //����ģ��ͨ������һ�������������businessType������Ŀ��height��depthȫ�ֱ�����ֵ

void send_height_to_module(void);
void on_arrive_height(void); //����ӵ�ģ�鴫���ġ��Ѿ��ƶ���Ŀ��߶ȡ�����Ϣ������ִ�дκ�����ȡ�������ȥ���򿪵�������������������ط���Ϣ��֪�Լ����õ����global_state = GOT_GOOD;��

#endif

