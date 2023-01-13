/*
 * led.c
 *
 *  Created on: 2023. 1. 3.
 *      Author: user
 */
#include "main.h"

void Led_On() {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

void Led_Off() {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

