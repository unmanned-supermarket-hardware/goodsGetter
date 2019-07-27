#ifndef __ZMODULE_COMMUNICATION_H
#define __ZMODULE_COMMUNICATION_H	 
#include "sys.h"
//主控发给取货单元的信息的businessType
#define ZMODULE_MSG_WRONG_BUSINESS_TYPE  -1
#define ZMODULE_MSG_WRONG_JSON  -2
#define ZMODULE_MSG_WRONG_CRC -3
#define ZMODULE_MSG_ARRIVE_HEIGHT 23


extern double destination_height;
extern double destination_depth;
extern int global_state ;

extern u8 UART5_JSON_BUF[256]; //接收到的数据
extern u8 UART5_JSON_CRC;

int resolve_zmodule_msg(void);  //解析模组通过串口一传来的命令，返回businessType，并给目标height和depth全局变量赋值

void send_height_to_module(void);
void on_arrive_height(void); //如果接到模组传来的“已经移动到目标高度”的消息，立即执行次函数（取货（伸出去，打开电磁铁，缩回来，给主控发消息告知自己已拿到货物）global_state = GOT_GOOD;）

#endif

