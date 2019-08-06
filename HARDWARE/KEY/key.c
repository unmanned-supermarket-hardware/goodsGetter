#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//按键初始化函数 
//PA6,7设置成输入    
void key_init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟

	JTAG_Set(SWD_ENABLE);	//关闭JTAG,开启SWD

	GPIOA->CRH &= 0x0FFFFFFF; //PA15 设置成输入      
	GPIOA->CRH |= 0X80000000;
	
	GPIOA->ODR|=1<<15;	   	//PA6上拉  15


} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//TOP_KEY_PRES，TOP_KEY按下
//BOTTOM_KEY_PRES，BOTTOM_KEY按下

//注意此函数有响应优先级,TOP_KEY>BOTTOM_KEY!!
u8 key_scan()
{	 
	u8 key_up=1;//按键按松开标志,支持连按		  

	if(key_up && (KEY==0) )
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY==0)return KEY_PRES;
		
	}else if(KEY==1)key_up=1; 	     
	return 0;// 无按键按下
}




















