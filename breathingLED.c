/*
   FILE          : breathingLED.c
   PROJECT       : Stm32f3 Discovery Board/Linux
   PROGRAMMER    : Rohit Bhardwaj
   DESCRIPTION   : The code is for  STM32F303 board, it configure, and write to the Timers in PWM mode. Moreover,Commands present, 
                   and work to implement a 'Breathing' LED that autonomously cycles smoothly between 0 and 100%, and back to 0% again.
   	
	The program make use of HAL(Hardware Abstraction Layer) which is C code that implements basic drivers for all the peripherals 
	in the STM32 Family. It makes the code more portable over STM32 Family.
*/

#include <stdint.h>
#include <stdio.h>
#include "stm32f3xx_hal.h"
#include "common.h"

/*Global Handle Structure*/
TIM_HandleTypeDef tim1;
TIM_OC_InitTypeDef sConfig;
uint32_t enablebreathe = 0;

// FUNCTION      : pwminit()
// DESCRIPTION   : The function initialize the Timer 1 & GPIO Pins
// PARAMETERS    : The function accepts an int value
// RETURNS       : returns nothing
void pwminit(int mode)
{
/* Turn on clocks to I/O */
__GPIOA_CLK_ENABLE();

/* Configure GPIO pins */
GPIO_InitTypeDef  GPIO_InitStruct;
GPIO_InitStruct.Pin = (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
GPIO_InitStruct.Alternate = 6;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

uint32_t rc;

/* Initialize the Timer(PWM) */ 
__TIM1_CLK_ENABLE(); 
tim1.Instance = TIM1; 
tim1.Init.Prescaler     = HAL_RCC_GetPCLK2Freq()*2/1000000; 
tim1.Init.CounterMode   = TIM_COUNTERMODE_UP; 
tim1.Init.Period        = 1000; 
tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
tim1.Init.RepetitionCounter = 0; 

/*Initalize the Timer*/
rc = HAL_TIM_Base_Init(&tim1);
if(rc != HAL_OK) 
 {
  printf("Unable to initalize Timer, rc=%d\n",(unsigned)rc);
  return;
 }

/*Start the timer*/
 rc = HAL_TIM_Base_Start(&tim1);
 if(rc != HAL_OK) 
 {
  printf("Unable to start the timer, rc=%d\n",(unsigned)rc);
  return;
 }

/*Configure output:*/ 
sConfig.OCMode       = TIM_OCMODE_PWM1; 
sConfig.Pulse        = 500; 
sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH; 
sConfig.OCNPolarity  = TIM_OCNPOLARITY_LOW; 
sConfig.OCFastMode   = TIM_OCFAST_DISABLE; 
sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET; 
sConfig.OCNIdleState =TIM_OCNIDLESTATE_RESET;
HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig,TIM_CHANNEL_1);
HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig,TIM_CHANNEL_2);
HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig,TIM_CHANNEL_3);

 HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 1);
 HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
 TIM1->CNT = 0;       /* Reset counter */
}

ADD_CMD("pwminit",pwminit,"              Initialize the Timer & GPIO Pins");


// FUNCTION      : pwm()
// DESCRIPTION   : The function sets PWM output value for desired channel
// PARAMETERS    : The function accepts an int value
// RETURNS       : returns nothing
void pwm(int mode)
{
  if(mode != CMD_INTERACTIVE)
  {
    return;
  }

 uint32_t channel;
 int32_t value;

 fetch_uint32_arg(&channel);
 fetch_int32_arg(&value);

 if(channel == 1)
 {
   /* Start the PWM output: */ 
   HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_1);
   TIM1->CCR1 = value;
 }

 else if(channel == 2)
  {
   /* Start the PWM output: */ 
   HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_2);
   TIM1->CCR2 = value;
  }

 else if(channel == 3)
  {
   /* Start the PWM output: */ 
   HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_3);
   TIM1->CCR3 = value;
  }

   else if(channel == 0)
  {
   /* Stop the PWM output: */ 
   HAL_TIM_PWM_Stop(&tim1,TIM_CHANNEL_1);
   HAL_TIM_PWM_Stop(&tim1,TIM_CHANNEL_2);
   HAL_TIM_PWM_Stop(&tim1,TIM_CHANNEL_3);
  }
}
ADD_CMD("pwm",pwm,"              pwm<channel><value>");

// FUNCTION      : TIM1_UP_TIM16_IRQHandler()
// DESCRIPTION   : The function implement a 'Breathing' LED that autonomously cycles 
//                 smoothly between 0 and 100%, and back to 0% again
// PARAMETERS    : nothing
// RETURNS       : returns nothing
void TIM1_UP_TIM16_IRQHandler(void)    //startup_stm32f303xc.s
{
  static int i = 0;
  static int increment = 1;
  TIM1 ->SR &= ~1;//~UIF reset the flag in timer
 if(enablebreathe)
{
  TIM1->CCR1 = i;//start from zero by setting it to zero
  if (increment) 
 {
   i++;         //increment the value in i
  if(i >= 1000) //when it reaches to maximum ie 1000
  { 
    increment = 0; // change the condition by making it zero
   }  
  }
  else    //now start the decrementation 
  {
    i--;     //decrement it
   if(i == 0) //when it reaches to zero
   {
     increment = 1; //reanalyze it back to 1
    }
   } 
  }  
}

// FUNCTION      : monitor()
// DESCRIPTION   : The function fetches the value from the commands
// PARAMETERS    : The function accepts an int value
// RETURNS       : returns nothing
void monitor(int mode)
{
    if(mode != CMD_INTERACTIVE)
  {
    return;
  }

  fetch_uint32_arg(&enablebreathe);
}

ADD_CMD("enable",monitor,"              eneblebreathe");
