#include "master_communication.h"
#include "sys.h"
/*
��������ͨ������1����������
����ֵ��

*/

int resolve_master_msg()
{
	cJSON *root1, *businessTypeJSON ;  
	char businessType[5];
	int	res;
	
	root1 = cJSON_Parse((char *)USART1_JSON_BUF);
	
	//printf("%s\n",USART1_JSON_BUF);
	//JSON ��Ч���ж�
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
	//һ������·���ֵ��businessType
	res = atoi(businessType);
	if(res != MASTER_MSG_CHECK && res != MASTER_MSG_GET &&  res != MASTER_MSG_DROP_GOOD  &&  res != MASTER_MSG_DROP_TRAY)
	{
		res = MASTER_MSG_WRONG_BUSINESS_TYPE; //business type���󣬷��ش�����
	}
	else if(res == MASTER_MSG_GET)  //�����ȡ������Ҫ��height��depth��ֵ
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



//����ӵ�ȡ���������ִ�д˺�������ģ�鷢��Ϣ��global_state = GOING_TO_HEIGHT;��
void on_get_good(void)
{
	send_height_to_module();
	global_state = GOING_TO_HEIGHT;
}



void on_drop_good(void);  //����ӵ����ش�����ж���������ִ�дκ�����push())
void on_drop_dish(void);//����ӵ����ش����Ķ������������ִ�дκ���(setMagnet(off))

