#ifndef __MASTER_COMMUNICATION_H
#define __MASTER_COMMUNICATION_H	 
#include "sys.h"
//主控发给取货单元的信息的businessType
#define MASTER_MSG_WRONG_BUSINESS_TYPE  -1
#define MASTER_MSG_WRONG_JSON  -2
#define MASTER_MSG_CHECK  13
#define MASTER_MSG_GET  14
#define MASTER_MSG_DROP_GOOD  15
#define MASTER_MSG_DROP_TRAY  16


extern double destination_height;
extern double destination_depth;
extern int global_state ;

extern u8 USART1_JSON_BUF[256]; //接收到的数据


int resolve_master_msg(void);  //解析主控通过串口一传来的命令，返回businessType，并给目标height和depth全局变量赋值

void on_get_good(void);   //如果接到取货命令，立即执行次函数（给模组发消息，global_state = GOING_TO_HEIGHT;）


void on_drop_good(void);  //如果接到主控传来的卸货命令，立即执行次函数（push())
void on_drop_dish(void);//如果接到主控传来的丢盘子命令，立即执行次函数(setMagnet(off))
void on_check(void);

#endif

