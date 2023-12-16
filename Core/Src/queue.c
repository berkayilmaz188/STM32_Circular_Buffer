/* BEGIN Header */
/**
  ******************************************************************************
  * @file    circular_buffer.c
  * @brief   Circular buffer operations.
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
/* END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "queue.h"
#include "circular_buffer.h"
#include <stdlib.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern QUEUE_InitTypeDef queue;

/******************************************************************************/
/*           					 Queue Functions	  				          */
/******************************************************************************/

/**
  * @brief This function initialize Circular Buffer struct.
  */
void Queue_Init(QUEUE_InitTypeDef *queue, uint8_t size){
	queue->size = size;
	queue->indexData = 0;
	queue->indexQueue = 0;
	queue->queue = (DATA_InitTypeDef*)calloc(queue->size,sizeof(DATA_InitTypeDef));
	for (int i = 0; i < queue->size; i++) {
		queue->queue[i].flag = QUEUE_STATE_EMPTY;
	}
}

/**
  * @brief This function initialize Circular Buffer struct.
  */
void Queue_Check(QUEUE_InitTypeDef *queue){
	char string[100];
	for (int i = 0; i < QUEUE_SIZE; i++) {
		if(queue->queue[i].flag == QUEUE_STATE_FULL){
			//Adress Print
			sprintf(string, "Queue[%d] Address: %p\n",i,&(queue->queue[i]));
			HAL_UART_Transmit(&huart2, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
			HAL_Delay(DELAY_MS);


			// Data
			char dataStr[21]; // Null için 1 ekledim
			strncpy(dataStr, (char*)queue->queue[i].datas, 20); // que size i 20 oldugundan
			dataStr[20] = '\0';

			sprintf(string, "Data: %s\n", dataStr);
			HAL_UART_Transmit(&huart2, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
			HAL_Delay(DELAY_MS);
		}
	}
}

void TryData(QUEUE_InitTypeDef *queue){

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



            if(!strcmp(token, "Blink")){
            	  HAL_UART_Transmit(&huart2,(uint8_t *)"Blink geldiiii.\r\n", 22,HAL_MAX_DELAY);
            }
            else if(!strcmp(token, "LedOn")){
            	  HAL_UART_Transmit(&huart2,(uint8_t *)"LedOn geldiiii.\r\n", 22,HAL_MAX_DELAY);

            }

            }
        token = strtok(NULL, " ");
        queue->queue[i].flag = QUEUE_STATE_EMPTY; // queue yi boşalt
}
}
/**
  * @brief This function initialize Circular Buffer struct.
  */


