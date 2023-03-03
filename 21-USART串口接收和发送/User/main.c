#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_key.h"
#include "stm32f10x_it.h"
#include "bsp_exti.h"
#include <stdio.h>

int main(void)
{	

  USART_Config();
	Key_GPIO_Config();
	EXTI_Key_Config();
	
	while (1)
				{
					 //每秒钟向串口调试助手发送Init_ok;
					if(flag_port == 1)
					{
						printf("Init_ok\n");
						SysTick_Delay_Ms( 1000 );
					}
				}

}
