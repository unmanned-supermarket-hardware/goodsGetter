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
	//CRC��֤
	if(USART1_JSON_CRC != crc8_calculate(USART1_JSON_BUF,strlen((char *)USART1_JSON_BUF)))
	{
		//��ʱ��û��parse root1������ֱ��return��
		printf("error! wrong crc\n");
		return MASTER_MSG_WRONG_CRC;
	}
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


void on_check(void)
{
	report_state();
}


//����ӵ�ȡ���������ִ�д˺�������ģ�鷢��Ϣ��global_state = GOING_TO_HEIGHT;��
void on_get_good(void)
{
	send_height_to_module();
	global_state = GOING_TO_HEIGHT;
}



void on_drop_good(void) //����ӵ����ش�����ж���������ִ�дκ�����push())
{
	push();
	global_state = PUSH_GOOD;
}
void on_drop_tray(void)//����ӵ����ش����Ķ������������ִ�дκ���(setMagnet(off))
{
	setMagnet(MAGNET_OFF);
	global_state = IDLE;
}

void report_state(void) //�����ػ㱨��ǰ���
{
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;
	char businessType[5] ="0013";
	int errorCode = 200;
	char errorDesc[64] = "fff";
	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0013");
	
	if(!is_distance_receiving  ) //û�յ��������ȼ�����
	{
		errorCode = 119;
		sprintf(errorDesc,"%s","һ��ʱ��û�յ�����������");
	}
	else if(!is_distance_right  )
	{
		errorCode = 118;
		sprintf(errorDesc,"%s","���ģ�鷵��ֵ��Error");
	}
	cJSON_AddNumberToObject(root,"errorCode",errorCode);
	cJSON_AddStringToObject(root,"errorDesc",errorDesc);
	
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//����
		sendMsgToMaster(strSend,strSendLen);
	}
	//�����ڴ�
	cJSON_Delete(root);
}
