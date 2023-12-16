/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    project1_uart.c
  * @brief   Special UART functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Tubitak BILGEM.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "project1_uart.h"
#include "queue.h"
#include "circular_buffer.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t rxParameter = 0;

uint8_t ledOn[] = "Led On\r\n";
uint8_t ledOff[] = "Led Off\r\n";

/* Private function prototypes -----------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern FLAGS_STATES flags_States;
extern TIM_HandleTypeDef htim2;
extern uint32_t delayMs;
extern uint8_t rxIndex;
extern uint8_t rxData[20];

/******************************************************************************/
/*           				 Special UART Functions 				          */
/******************************************************************************/

void UART_Flags_Init(FLAGS_STATES *flags_States){
	flags_States->flag_LedOn = FLAG_STATE_RESET;
	flags_States->flag_LedOff = FLAG_STATE_RESET;
	flags_States->flag_LedBlink = FLAG_STATE_RESET;
	flags_States->flag_LedPWM = FLAG_STATE_RESET;
	flags_States->flag_UARTCallBack = FLAG_STATE_RESET;
	flags_States->flag_UART_LedOn = FLAG_STATE_RESET;
	flags_States->flag_UART_LedOff = FLAG_STATE_RESET;
	flags_States->flag_UART_LedBlink = FLAG_STATE_RESET;
	flags_States->flag_UART_LedPWM = FLAG_STATE_RESET;
	flags_States->flag_Parameter = FLAG_STATE_RESET;
	flags_States->state_Led = STATE_LED_FIRST;
	delayMs = BLINK_RATE_LOW;
}

void Queue_Process_LED(QUEUE_InitTypeDef *queue){
    char *token;
    char *str;

    for (int i = 0; i < QUEUE_SIZE; i++) {
        if(queue->queue[i].flag == QUEUE_STATE_FULL){
            //Data işleme queue içi
            char dataStr[21]; // Null için 1 ekledim
            strncpy(dataStr, (char*)queue->queue[i].datas, 20); // que size i 20 oldugundan
            dataStr[20] = '\0';
            str = dataStr;
            token = strtok(str, " ");

            // set all flags to RESET
            flags_States.flag_UART_LedBlink = FLAG_STATE_RESET;
            flags_States.flag_UART_LedOn = FLAG_STATE_RESET;
            flags_States.flag_UART_LedOff = FLAG_STATE_RESET;
            flags_States.flag_UART_LedPWM = FLAG_STATE_RESET;

            if(!strcmp(token, "Blink")){
                flags_States.flag_LedBlink = FLAG_STATE_SET;
      		    flags_States.flag_LedOn = FLAG_STATE_RESET;
      		    flags_States.flag_LedOff = FLAG_STATE_RESET;
      		    flags_States.flag_LedPWM = FLAG_STATE_RESET;
            }
            else if(!strcmp(token, "LedOn")){
                flags_States.flag_LedOn = FLAG_STATE_SET;
      		    flags_States.flag_LedOff = FLAG_STATE_RESET;
      		    flags_States.flag_LedPWM = FLAG_STATE_RESET;
    		    flags_States.flag_LedBlink = FLAG_STATE_RESET;
            }
            else if(!strcmp(token, "LedOff")){
                flags_States.flag_LedOn = FLAG_STATE_RESET;
      		    flags_States.flag_LedOff = FLAG_STATE_SET;
      		    flags_States.flag_LedPWM = FLAG_STATE_RESET;
    		    flags_States.flag_LedBlink = FLAG_STATE_RESET;
            }
            else if(!strcmp(token, "LedPWM")){
                flags_States.flag_LedBlink = FLAG_STATE_RESET;
      		    flags_States.flag_LedOn = FLAG_STATE_RESET;
      		    flags_States.flag_LedOff = FLAG_STATE_RESET;
      		    flags_States.flag_LedPWM = FLAG_STATE_SET;
            }
            else if(!strcmp(token, "ResetAll")){
      		  flags_States.flag_LedBlink = FLAG_STATE_RESET;
      		  flags_States.flag_LedOn = FLAG_STATE_RESET;
      		  flags_States.flag_LedOff = FLAG_STATE_RESET;
      		  flags_States.flag_LedPWM = FLAG_STATE_RESET;
      		  flags_States.flag_UART_LedBlink = FLAG_STATE_RESET;
      		  flags_States.flag_UART_LedOn = FLAG_STATE_RESET;
      		  flags_States.flag_UART_LedOff = FLAG_STATE_RESET;
      		  flags_States.flag_UART_LedPWM = FLAG_STATE_RESET;
      		  flags_States.flag_Parameter = FLAG_STATE_RESET;
      		  flags_States.flag_UARTCallBack = FLAG_STATE_RESET;
             }


            token = strtok(NULL, " ");
            if(token != NULL){
                rxParameter = atoi(token); // string i int yapma !!!
                flags_States.flag_Parameter = FLAG_STATE_SET; // parametre set etme
            }else{
                flags_States.flag_Parameter = FLAG_STATE_RESET; // parametre gelmezse reset kalsın çalışmasın.
            }

            queue->queue[i].flag = QUEUE_STATE_EMPTY; // queue yi boşalt
        }
    }
}



/**
  * @brief This function initialize Circular Buffer struct.
  */
/*
void CheckReceiveData(void){
	  uint8_t ledOnUART[20] = "LedOn";
	  uint8_t ledBlinkUART[20] = "Blink";
	  uint8_t ledOffUART[20] = "LedOff";
	  uint8_t ledPWMUART[20] = "LedPWM";
	  uint8_t resetAll[20] = "ResetAll";
	  if(!strcmp((char *)rxData,(char *)ledBlinkUART)){
		  memset(rxData,0,20);
		  rxIndex = 0;
		  flags_States.flag_UART_LedBlink = FLAG_STATE_SET;
		  flags_States.flag_UART_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedPWM = FLAG_STATE_RESET;
	  }
	  else if(!strcmp((char *)rxData,(char *)ledOnUART)){
		  memset(rxData,0,20);
		  rxIndex = 0;
		  flags_States.flag_UART_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOn = FLAG_STATE_SET;
		  flags_States.flag_UART_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedPWM = FLAG_STATE_RESET;
	  }
	  else if(!strcmp((char *)rxData,(char *)ledOffUART)){
		  memset(rxData,0,20);
		  rxIndex = 0;
		  flags_States.flag_UART_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOff = FLAG_STATE_SET;
		  flags_States.flag_UART_LedPWM = FLAG_STATE_RESET;
	  }
	  else if(!strcmp((char *)rxData,(char *)ledPWMUART)){
		  memset(rxData,0,20);
		  rxIndex = 0;
		  flags_States.flag_UART_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedPWM = FLAG_STATE_SET;
	  }
	  else if(!strcmp((char *)rxData,(char *)resetAll)){
		  memset(rxData,0,20);
		  rxIndex = 0;
		  flags_States.flag_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_LedPWM = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_UART_LedPWM = FLAG_STATE_RESET;
		  flags_States.flag_Parameter = FLAG_STATE_RESET;
		  flags_States.flag_UARTCallBack = FLAG_STATE_RESET;
	  }
}*/

/**
  * @brief This function initialize Circular Buffer struct.
  */
/*
void CheckParameterData(void){
	  if((flags_States.flag_UART_LedBlink == FLAG_STATE_SET || \
			  flags_States.flag_UART_LedOn == FLAG_STATE_SET || \
			  flags_States.flag_UART_LedOff == FLAG_STATE_SET || \
			  flags_States.flag_UART_LedPWM == FLAG_STATE_SET) && \
			  (rxData[0] == 'P')){
		  uint8_t count = 0;
		  rxParameter = 0;
		  for(int i = ((uint8_t)(rxData[2]-48) + 3); i > 3; i--){
			  uint8_t temp = (uint8_t)(rxData[i] - 48);
			  int power = pow_num(10,count++);
			  rxParameter += (power * temp);
		  }
		  memset(rxData,0,20);
		  rxIndex = 0;
		  flags_States.flag_Parameter = FLAG_STATE_SET;
	  }
}*/

/**
  * @brief This function initialize Circular Buffer struct.
  */
/*
int pow_num(int x, int y)
{
    int power = 1, i;
    for (i = 1; i <= y; ++i)
    {
        power = power * x;

    }
    return power;
}*/

/**
  * @brief This function initialize Circular Buffer struct.
  */
/*
void CheckFlags(void){
	  if((flags_States.flag_UART_LedBlink == FLAG_STATE_SET) && \
			  (flags_States.flag_Parameter == FLAG_STATE_SET)){
		  flags_States.flag_LedBlink = FLAG_STATE_SET;
		  flags_States.flag_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_LedPWM = FLAG_STATE_RESET;
		  flags_States.flag_Parameter = FLAG_STATE_RESET;
	  }
	  else if(flags_States.flag_UART_LedOn == FLAG_STATE_SET){
		  flags_States.flag_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_LedOn = FLAG_STATE_SET;
		  flags_States.flag_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_LedPWM = FLAG_STATE_RESET;
	  }
	  else if(flags_States.flag_UART_LedOff == FLAG_STATE_SET){
		  flags_States.flag_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_LedOff = FLAG_STATE_SET;
		  flags_States.flag_LedPWM = FLAG_STATE_RESET;
	  }
	  else if((flags_States.flag_UART_LedPWM == FLAG_STATE_SET) && \
			  (flags_States.flag_Parameter == FLAG_STATE_SET)){
		  flags_States.flag_LedBlink = FLAG_STATE_RESET;
		  flags_States.flag_LedOn = FLAG_STATE_RESET;
		  flags_States.flag_LedOff = FLAG_STATE_RESET;
		  flags_States.flag_LedPWM = FLAG_STATE_SET;
		  flags_States.flag_Parameter = FLAG_STATE_RESET;
	  }
	  flags_States.flag_UARTCallBack = FLAG_STATE_RESET;
}*/

/**
  * @brief This function initialize Circular Buffer struct.
  */
void CheckFunctions(void){
	if(flags_States.flag_LedOn == FLAG_STATE_RESET && \
				  flags_States.flag_LedOff == FLAG_STATE_RESET && \
				  flags_States.flag_LedBlink == FLAG_STATE_RESET && \
				  flags_States.flag_LedPWM == FLAG_STATE_RESET){
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,TIM_MAX_VALUE);
		//HAL_UART_Transmit(&huart2,ledOn,sizeof(ledOn),HAL_MAX_DELAY);
		HAL_Delay(delayMs);
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,TIM_MIN_VALUE);
		//HAL_UART_Transmit(&huart2,ledOff,sizeof(ledOff),HAL_MAX_DELAY);
		HAL_Delay(delayMs);
	}
	else if(flags_States.flag_LedOn == FLAG_STATE_SET){
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,TIM_MAX_VALUE);
		//HAL_UART_Transmit(&huart2,ledOn,sizeof(ledOn),HAL_MAX_DELAY);
	}
	else if(flags_States.flag_LedOff == FLAG_STATE_SET){
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,TIM_MIN_VALUE);
		//HAL_UART_Transmit(&huart2,ledOff,sizeof(ledOff),HAL_MAX_DELAY);
	}
	else if(flags_States.flag_LedBlink == FLAG_STATE_SET){
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,TIM_MAX_VALUE);
		//HAL_UART_Transmit(&huart2,ledOn,sizeof(ledOn),HAL_MAX_DELAY);
		HAL_Delay(rxParameter);
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,TIM_MIN_VALUE);
		//HAL_UART_Transmit(&huart2,ledOff,sizeof(ledOff),HAL_MAX_DELAY);
		HAL_Delay(rxParameter);
	}
	else if(flags_States.flag_LedPWM == FLAG_STATE_SET){
		for(int i = TIM_MIN_VALUE ; i < rxParameter ; i++){
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,i);
			HAL_Delay(1);
		}
		for(int i = rxParameter ; i > TIM_MIN_VALUE ; i--){
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,i);
			HAL_Delay(1);
		}
	}
}
