#include "main.h"
#include "BMW.h"
#define LOW     BRR 
#define HIGH    BSRR 
static void MX_TIM2_Init(void);
void SystemClock_Config(void);

uint32_t LIN_B = 0;
uint8_t DATA[9];

void USART3_IRQHandler(void)
{
  if((USART3->SR & USART_SR_LBD) == USART_SR_LBD)
  {
    uint32_t i = 0;
     
    USART3->SR &= ~(USART_SR_LBD);/* Clear LIN break detection flag */
           
    while(LL_USART_IsActiveFlag_RXNE(USART3)==0);

    i=(uint16_t)(USART3->DR & (uint16_t)0x01FF);
           
    while(LL_USART_IsActiveFlag_RXNE(USART3)==0);

    i=(uint16_t)(USART3->DR & (uint16_t)0x01FF);
           
    while(LL_USART_IsActiveFlag_RXNE(USART3)==0);

    i=(uint16_t)(USART3->DR & (uint16_t)0x01FF);
           
    if(i==20)
    {
      LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
      LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
      i=0;
      while(i<9)
      {
        while (!LL_USART_IsActiveFlag_TXE(USART3));
        USART3->DR = DATA[i];
        i++;
      }
      while (!LL_USART_IsActiveFlag_TC(USART3));
      LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3|LL_GPIO_PIN_5);
      LIN_B=1;
    }     
  }
}

int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  LL_GPIO_AF_Remap_SWJ_NOJTAG();
  LL_GPIO_AF_Remap_SWJ_NONJTRST();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();

  while (1)
  {
    if(LIN_B==1)
    {
      Create_Message(&DATA[0],20);
      LIN_B=0;
    }
      LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
      LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
  }
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2);

  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

  while(LL_RCC_HSI_IsReady() != 1);

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);
  LL_RCC_PLL_Enable();

  while(LL_RCC_PLL_IsReady() != 1);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

  LL_SetSystemCoreClock(64000000);
}


void MX_USART3_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART3_IRQn);

  USART_InitStruct.BaudRate = 19200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  LL_USART_Init(USART3, &USART_InitStruct);
  
  LL_USART_SetLINBrkDetectionLen(USART3, LL_USART_LINBREAK_DETECT_10B);
  LL_USART_EnableLIN(USART3);
  LL_USART_ConfigLINMode(USART3);
  LL_USART_EnableIT_RXNE(USART3);
  LL_USART_Enable(USART3);
}

void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
   LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3|LL_GPIO_PIN_5);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_9|LL_GPIO_PIN_8|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_15; 
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8; 
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15; 
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2); //ENABLE_WAKEUP
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);   //ENABLE_LIN_PHY
}


static void MX_TIM2_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  LL_TIM_SetEncoderMode(TIM2, LL_TIM_ENCODERMODE_X4_TI12);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV4);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV32_N8);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV4);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV32_N8);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1023;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  TIM2->CR1 |= TIM_CR1_CEN;
}



void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

