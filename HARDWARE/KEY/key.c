#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//�������� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//������ʼ������ 
//PA6,7���ó�����    
void key_init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��

	JTAG_Set(SWD_ENABLE);	//�ر�JTAG,����SWD

	GPIOA->CRH &= 0x0FFFFFFF; //PA15 ���ó�����      
	GPIOA->CRH |= 0X80000000;
	
	GPIOA->ODR|=1<<15;	   	//PA6����  15


} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//TOP_KEY_PRES��TOP_KEY����
//BOTTOM_KEY_PRES��BOTTOM_KEY����

//ע��˺�������Ӧ���ȼ�,TOP_KEY>BOTTOM_KEY!!
u8 key_scan()
{	 
	u8 key_up=1;//�������ɿ���־,֧������		  

	if(key_up && (KEY==0) )
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY==0)return KEY_PRES;
		
	}else if(KEY==1)key_up=1; 	     
	return 0;// �ް�������
}




















