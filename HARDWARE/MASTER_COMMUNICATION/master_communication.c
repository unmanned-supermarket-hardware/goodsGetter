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



//如果接到取货命令，立即执行此函数（给模组发消息，global_state = GOING_TO_HEIGHT;）
void on_get_good(void)
{
	send_height_to_module();
	global_state = GOING_TO_HEIGHT;
}



void on_drop_good(void);  //如果接到主控传来的卸货命令，立即执行次函数（push())
void on_drop_dish(void);//如果接到主控传来的丢盘子命令，立即执行次函数(setMagnet(off))

