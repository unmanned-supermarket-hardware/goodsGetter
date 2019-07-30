#include "zmodule_communication.h"
#include "sys.h"

/*
��������ģ��ͨ������5����������
����ֵ��
*/

int resolve_zmodule_msg()
{
	cJSON *root1, *businessTypeJSON ;  
	char businessType[5];
	int	res;
	
	root1 = cJSON_Parse((char *)UART5_JSON_BUF);
	
	//printf("%s\n",USART1_JSON_BUF);
	//CRCУ��
	if(UART5_JSON_CRC != crc8_calculate(UART5_JSON_BUF,strlen((char *)UART5_JSON_BUF)))
	{
		//��ʱ��û��parse root1������ֱ��return��
		printf("error! wrong crc\n");
		return ZMODULE_MSG_WRONG_CRC;
	}
	
	//JSON ��Ч���ж�
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
	//һ������·���ֵ��businessType
	res = atoi(businessType);
	if(res != ZMODULE_MSG_ARRIVE_HEIGHT)
	{
		res = MASTER_MSG_WRONG_BUSINESS_TYPE; //business type���󣬷��ش�����
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
		//����
		sendMsgToModule(strSend,strSendLen);
	}
	//�����ڴ�
	cJSON_Delete(root);

}

//����ӵ�ģ�鴫���ġ��Ѿ��ƶ���Ŀ��߶ȡ�����Ϣ������ִ�дκ�����ȡ�������ȥ���򿪵�������������������ط���Ϣ��֪�Լ����õ����global_state = GOT_GOOD;��	
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
	
	//��֪����

	
	root=cJSON_CreateObject();

	cJSON_AddStringToObject(root,"businessType","0015");
	cJSON_AddNumberToObject(root,"Result",result);
	strSendLen = generate_send_str(root,strSend);
	if(strSendLen >0)
	{
		//����
		sendMsgToMaster(strSend,strSendLen);
	}
	global_state = GOT_GOOD;
	//�����ڴ�
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
		//����
		sendMsgToModule(strSend,strSendLen);
	}

	//�����ڴ�
	cJSON_Delete(root);
}