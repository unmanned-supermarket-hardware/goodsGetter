#ifndef __MASTER_COMMUNICATION_H
#define __MASTER_COMMUNICATION_H	 
#include "sys.h"
//主控发给取货单元的信息的businessType
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

extern u8 USART1_JSON_BUF[256]; //接收到的数据
extern u8	USART1_JSON_CRC ;

extern int is_distance_receiving ;  //表示红外一直在接收数据
extern int is_distance_right ;      //表示红外接收数据正常，为D = ***m，而非Error

int resolve_master_msg(void);  //解析主控通过串口一传来的命令，返回businessType，并给目标height和depth全局变量赋值


void on_check_msg(void);
void on_get_good_msg(void);   //如果接到取货命令，立即执行次函数（给模组发消息，global_state = GOING_TO_HEIGHT;）


void on_drop_good_msg(void);  //如果接到主控传来的卸货命令，立即执行次函数（push())
void on_drop_tray_msg(void);//如果接到主控传来的丢盘子命令，立即执行次函数(setMagnet(off))


void report_state(void); //向主控汇报当前情况
#endif

