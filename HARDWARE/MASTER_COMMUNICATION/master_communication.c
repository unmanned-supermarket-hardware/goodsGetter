#include "master_communication.h"
#include "sys.h"
/*
解析主控通过串口1传来的命令
返回值：

*/

int resolve_master_msg()
{
	cJSON *root1, *businessTypeJSON ;  
	char businessType[5];
	int	res;
	//CRC验证
	if(USART1_JSON_CRC != crc8_calculate(USART1_JSON_BUF,strlen((char *)USART1_JSON_BUF)))
	{
		//此时还没有parse root1，可以直接return了
		printf("error! wrong crc\n");
		return MASTER_MSG_WRONG_CRC;
	}
	root1 = cJSON_Parse((char *)USART1_JSON_BUF);
	
	//printf("%s\n",USART1_JSON_BUF);
	//JSON 有效性判断
	if(root1 == NULL) return MASTER_MSG_WRONG_JSON;
	
	businessTypeJSON = cJSON_GetObjectItem(root1, "businessType");
	if (!businessTypeJSON) {
//			printf("get businessType faild !\n");
//			printf("Error before: [%s]\n", cJSON_GetErrorPtr());
			res = -1;
	}
	else
	{
		sprintf(businessType, "%s", businessTypeJSON->valuestring);
	}
	//一般情况下返回值是businessType
	res = atoi(businessType);
	if(res != MASTER_MSG_CHECK && res != MASTER_MSG_GET &&  res != MASTER_MSG_DROP_GOOD  &&  res != MASTER_MSG_DROP_TRAY)
	{
		res = MASTER_MSG_WRONG_BUSINESS_TYPE; //business type有误，返回错误码
	}
	else if(res == MASTER_MSG_GET)  //如果是取货，还要给height和depth赋值
	{
			cJSON *heightJSON,*depthJSON;
		 heightJSON = cJSON_GetObjectItem(root1, "Height");
		 depthJSON = cJSON_GetObjectItem(root1, "Depth");
		 
		 destination_height = heightJSON->valuedouble;
		 destination_depth = depthJSON->valuedouble;
		 
		 cJSON_Delete(heightJSON);
		 cJSON_Delete(depthJSON);
	}
 
	cJSON_Delete(root1);
	cJSON_Delete(businessTypeJSON);
	return res;
}


void on_check(void)
{
	report_state();
}


//如果接到取货命令，立即执行此函数（给模组发消息，global_state = GOING_TO_HEIGHT;）
void on_get_good(void)
{
	send_height_to_module();
	global_state = GOING_TO_HEIGHT;
}



void on_drop_good(void) //如果接到主控传来的卸货命令，立即执行次函数（push())
{
	push();
	global_state = PUSH_GOOD;
}
void on_drop_tray(void)//如果接到主控传来的丢盘子命令，立即执行次函数(setMagnet(off))
{
	setMagnet(MAGNET_OFF);
	global_state = IDLE;
}

void report_state(void) //向主控汇报当前情况
{
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char businessType[5] ="0013";
	int errorCode = 200;
	char errorDesc[64] = "fff";
	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0013");
	
	if(!is_distance_receiving  ) //没收到红外优先级更高
	{
		errorCode = 119;
		sprintf(errorDesc,"%s","一段时间没收到距离数据了");
	}
	else if(!is_distance_right  )
	{
		errorCode = 118;
		sprintf(errorDesc,"%s","测距模块返回值是Error");
	}
	cJSON_AddNumberToObject(root,"errorCode",errorCode);
	cJSON_AddStringToObject(root,"errorDesc",errorDesc);
	
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//发送
		sendMsgToMaster(strSend,strSendLen);
	}
	//清理内存
	cJSON_Delete(root);
}
