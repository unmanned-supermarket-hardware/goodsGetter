#ifndef __MASTER_COMMUNICATION_H
#define __MASTER_COMMUNICATION_H	 
#include "sys.h"
//���ط���ȡ����Ԫ����Ϣ��businessType
#define MASTER_MSG_WRONG_BUSINESS_TYPE  -1
#define MASTER_MSG_WRONG_JSON  -2
#define MASTER_MSG_WRONG_CRC  -3

#define MASTER_MSG_CHECK  13
#define MASTER_MSG_GET  14
#define MASTER_MSG_DROP_GOOD  15
#define MASTER_MSG_DROP_TRAY  16


extern double destination_height;
extern double destination_depth;
extern int global_state ;

extern u8 USART1_JSON_BUF[256]; //���յ�������
extern u8	USART1_JSON_CRC ;

extern int is_distance_receiving ;  //��ʾ����һֱ�ڽ�������
extern int is_distance_right ;      //��ʾ�����������������ΪD = ***m������Error

int resolve_master_msg(void);  //��������ͨ������һ�������������businessType������Ŀ��height��depthȫ�ֱ�����ֵ


void on_check_msg(void);
void on_get_good_msg(void);   //����ӵ�ȡ���������ִ�дκ�������ģ�鷢��Ϣ��global_state = GOING_TO_HEIGHT;��


void on_drop_good_msg(void);  //����ӵ����ش�����ж���������ִ�дκ�����push())
void on_drop_tray_msg(void);//����ӵ����ش����Ķ������������ִ�дκ���(setMagnet(off))


void report_state(void); //�����ػ㱨��ǰ���
#endif

