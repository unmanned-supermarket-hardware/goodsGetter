#include "timer.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
extern u8 USART2_RX_BUF[64]; //���յ�������
extern 	short int real_current  ;
extern 	short int real_velocity ;
extern 	int real_position ;
extern int global_state;

/*

#define IDLE 0
#define GOING_TO_HEIGHT 1
#define ARRIVE_HEIGHT 2
#define GOT_GOOD 3
#define PUSH_GOOD 4
#define DROP_TRAY 5
*/
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		printf("current_depth_in_m =  \nd2Str = %s\n",USART2_RX_BUF);	
		
		switch(global_state)
		{
			case(0):{printf("idle\n\n");break;}
			case(1):{printf("GOING_TO_HEIGHT�����ڵȴ�ģ�鵽��λ��\n\n");break;}
			case(2):{printf("ARRIVE_HEIGHTģ�鵽��\n\n");break;}
			case(3):{printf("GOT_GOODȡ������\n\n");break;}
		}
		//printf("%d\t%d\t%d\n",real_current,real_velocity,real_position);
		//printf("%d\n",atoi("0015"));
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  	//Ԥ��Ƶ������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2									 
}




//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;	//TIM2ʱ��ʹ��    
 	TIM2->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM2->PSC=psc;  	//Ԥ��Ƶ������
	TIM2->DIER|=1<<0;   //��������ж�				
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM2_IRQn,2);//��ռ1�������ȼ�3����2									 
}















