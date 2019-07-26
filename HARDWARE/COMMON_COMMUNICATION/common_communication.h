#ifndef __COMMON_COMMUNICATION_H
#define __COMMON_COMMUNICATION_H	 
#include "sys.h"

#define MAX_MSG_SIZE  256
#define MAX_JSON_SIZE (MAX_MSG_SIZE-8)

//计算crc校验值
unsigned	char crc8_calculate(unsigned char * ucPtr, unsigned char ucLen) ;
int generate_send_str(cJSON *root,char * strSend);

#endif

