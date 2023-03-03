#include "stm32f10x_it.h"
#include "bsp_exti.h"
#include "bsp_usart.h"
#include <stdio.h>
#include <string.h> 

int flag_port = 0;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



void KEY1_IRQHandler(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		// LED1 ȡ��		
		flag_port = !	flag_port;
    //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}
/*
	ͨ�����ڶ�ȡһ����'\n'��β���ַ��������ԴӴ������ַ���������
	���Ȳ��̶������ܳ��Ȳ�����8�ֽڣ������б����������һ��������
	Ȼ�������޳�����ַ�����������֣���������ش������֣�
*/

// �����жϷ�����
int k = 0;
void DEBUG_USART_IRQHandler(void)
{ 
	static char str1[8]={0}; 
	//int length = 8;
	static int i=0;
  static char str[8]={0};
	char flag=0;
	int j = 0;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{
		str[i] = USART_ReceiveData(DEBUG_USARTx);
		flag=1;
	}
	if(flag==1)	//������
	{
		if(str[i]=='\n')	//�ַ������ս���
		{
			k=0;
			for(j=0;j<8;j++)
			{
				if(str[j]>='0'&&str[j]<='9')
				{
					;
				}
				else
				{
					str1[k]=str[j];
					k++;
				}
			}
			printf("len=%d,str=%s",i-1,str1);
			i=0;
			memset(str1,0,8);
			memset(str,0,8);
			str[0]='\0';
		}
		else
		{
			if(i==7)	//��������
			{
				memset(str1,0,8);
				memset(str,0,8);
				i=0;
			}
			else
				i++;
		}
	}
	else
	{
		return;
	}
}
