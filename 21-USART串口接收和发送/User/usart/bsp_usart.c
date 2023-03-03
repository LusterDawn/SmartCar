#include "bsp_usart.h"



static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}






void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

void Usart_SendByte(USART_TypeDef*pUSATRTx,uint8_t data){
 USART_SendData(pUSATRTx, data);	
while( USART_GetFlagStatus(pUSATRTx,USART_FLAG_TXE)==RESET );
	
}
	
// 发送8位数据的数组
void Usart_SendArray(USART_TypeDef*pUSATRTx, uint8_t*array, uint8_t num)

{
	uint8_t i;
	for(i=0;i<num;i++)
	{
	
		Usart_SendByte(pUSATRTx,*array++);
		
	}//TC是判断发送一连串的标志位
	while( USART_GetFlagStatus(pUSATRTx,USART_FLAG_TC)==RESET );
	
}
 
//发送字符串
void Usart_SendStr(USART_TypeDef*pUSATRTx,uint8_t *str){
	uint8_t i=0;
	while(*(str+i)!='\0'){
		Usart_SendByte(pUSATRTx,*(str+i));
		i++;
		
	}
while( USART_GetFlagStatus(pUSATRTx,USART_FLAG_TC)==RESET );
}

//printf 函数
int fputc(int ch,FILE *f){
	
//直接把串口宏改一下就可以用了；
Usart_SendByte(DEBUG_USARTx,(uint8_t) ch);
while( USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE)==RESET );
	return (ch);
}

//scanf 函数
int fgetc(FILE *f)
{    //换成直接的串口；
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}