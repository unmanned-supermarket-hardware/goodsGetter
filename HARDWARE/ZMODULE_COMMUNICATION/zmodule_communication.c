#include "zmodule_communication.h"
#include "sys.h"

/*
解析线性模组通过串口5传来的命令
返回值：
*/

int resolve_zmodule_msg()
{
	cJSON *root1, *businessTypeJSON ;  
	char businessType[5];
	int	res;
	
	root1 = cJSON_Parse((char *)UART5_JSON_BUF);
	
	//printf("%s\n",USART1_JSON_BUF);
	//CRC校验
	if(UART5_JSON_CRC != crc8_calculate(UART5_JSON_BUF,strlen((char *)UART5_JSON_BUF)))
	{
		//此时还没有parse root1，可以直接return了
		printf("error! wrong crc\n");
		return ZMODULE_MSG_WRONG_CRC;
	}
	
	//JSON 有效性判断
	if(root1 == NULL) return ZMODULE_MSG_WRONG_JSON;
	
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
	if(res != ZMODULE_MSG_ARRIVE_HEIGHT)
	{
		res = MASTER_MSG_WRONG_BUSINESS_TYPE; //business type有误，返回错误码
	}
	
	cJSON_Delete(root1);
	cJSON_Delete(businessTypeJSON);
	return res;
}


void send_height_to_module()
{

	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0022");
	cJSON_AddNumberToObject(root,"Height",destination_height);
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//发送
		sendMsgToModule(strSend,strSendLen);
	}
	//清理内存
	cJSON_Delete(root);

}

//如果接到模组传来的“已经移动到目标高度”的消息，立即执行次函数（取货（伸出去，打开电磁铁，缩回来，给主控发消息告知自己已拿到货物）global_state = GOT_GOOD;）	
void on_arrive_height_msg(void)
{
	const int SUCCESS = 0;
	const int FAIL = -1;
	
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	
	int result = SUCCESS;
	if(goTo(destination_depth)!= 1) result = FAIL;
	setMagnet(MAGNET_ON);
	if(goTo(DEFALT_DEPTH)!= 1) result = FAIL;
	
	//告知主控

	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0015");
	cJSON_AddNumberToObject(root,"Result",result);
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//发送
		sendMsgToMaster(strSend,strSendLen);
	}
	global_state = GOT_GOOD;
	//清理内存
	cJSON_Delete(root);
	
}

void check_zmodule(void)
{
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0024");
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//发送
		sendMsgToModule(strSend,strSendLen);
	}

	//清理内存
	cJSON_Delete(root);
}