#include "bsp_usart.h"



static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}






void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

void Usart_SendByte(USART_TypeDef*pUSATRTx,uint8_t data){
 USART_SendData(pUSATRTx, data);	
while( USART_GetFlagStatus(pUSATRTx,USART_FLAG_TXE)==RESET );
	
}
	
// ����8λ���ݵ�����
void Usart_SendArray(USART_TypeDef*pUSATRTx, uint8_t*array, uint8_t num)

{
	uint8_t i;
	for(i=0;i<num;i++)
	{
	
		Usart_SendByte(pUSATRTx,*array++);
		
	}//TC���жϷ���һ�����ı�־λ
	while( USART_GetFlagStatus(pUSATRTx,USART_FLAG_TC)==RESET );
	
}
 
//�����ַ���
void Usart_SendStr(USART_TypeDef*pUSATRTx,uint8_t *str){
	uint8_t i=0;
	while(*(str+i)!='\0'){
		Usart_SendByte(pUSATRTx,*(str+i));
		i++;
		
	}
while( USART_GetFlagStatus(pUSATRTx,USART_FLAG_TC)==RESET );
}

//printf ����
int fputc(int ch,FILE *f){
	
//ֱ�ӰѴ��ں��һ�¾Ϳ������ˣ�
Usart_SendByte(DEBUG_USARTx,(uint8_t) ch);
while( USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE)==RESET );
	return (ch);
}

//scanf ����
int fgetc(FILE *f)
{    //����ֱ�ӵĴ��ڣ�
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}