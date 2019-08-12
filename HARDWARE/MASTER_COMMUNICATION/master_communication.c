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
		 //here�������ﴦ��Ŀ����Ⱥ����ӿ�ȵĹ�ϵ
		
		 destination_height = heightJSON->valuedouble;
	printf("%f - %f + (0.5 * %f) \n",current_depth_in_m,OFFSET,depthJSON->valuedouble);
//		 if(OFFSET - current_depth_in_m < 0.5 * (depthJSON->valuedouble))
//			 destination_depth = current_depth_in_m - OFFSET + (0.5* (depthJSON->valuedouble));
//		 else
			 destination_depth = OFFSET - (0.5* (depthJSON->valuedouble));
		 
		 cJSON_Delete(heightJSON);
		 cJSON_Delete(depthJSON);
	}

 
	cJSON_Delete(root1);
	cJSON_Delete(businessTypeJSON);
	return res;
}


void on_check_msg(void)
{
	report_state();
}


//����ӵ�ȡ���������ִ�д˺�������ģ�鷢��Ϣ��global_state = GOING_TO_HEIGHT;��
void on_get_good_msg(void)
{
	send_height_to_module(destination_height);
	global_state = GOING_TO_HEIGHT;
}



void on_drop_good_msg(void) //����ӵ����ش�����ж���������ִ�дκ�����push())
{
	push();
	report_drop_good();
	global_state = PUSH_GOOD;
}
void on_drop_tray_msg(void)//����ӵ����ش����Ķ������������ִ�дκ���(setMagnet(off))
{
	setMagnet(MAGNET_OFF);
	report_drop_tray();
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
void report_drop_tray(void) //�����ػ㱨��ǰ���
{
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;

	const int SUCCESS = 0;
	const int FAIL = -1;
	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0019");
	cJSON_AddNumberToObject(root,"Result",SUCCESS);
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//����
		sendMsgToMaster(strSend,strSendLen);
	}

	//�����ڴ�
	cJSON_Delete(root);
}
void report_drop_good(void) //�����ػ㱨��ǰ���
{
	cJSON *root;
	char strSend[MAX_MSG_SIZE];
	u8 strSendLen;

	const int SUCCESS = 0;
	const int FAIL = -1;
	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0017");
	cJSON_AddNumberToObject(root,"Result",SUCCESS);
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//����
		sendMsgToMaster(strSend,strSendLen);
	}

	//�����ڴ�
	cJSON_Delete(root);
}