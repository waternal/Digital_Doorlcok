/*
 * motor.c
 *
 *  Created on: 2023. 1. 3.
 *      Author: user
 */
#include "main.h"
#include "stm32f1xx.h"
#include "door.h"
#include "bell.h"
#include "delay_us.h"

extern uint8_t door_state;
extern ADC_HandleTypeDef hadc1;

void Door_Open() {
	Bell_Door_Open();
	HAL_GPIO_WritePin(MOTOR_SLEEP_GPIO_Port, MOTOR_SLEEP_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(MOTOR_SLEEP_GPIO_Port, MOTOR_SLEEP_Pin, GPIO_PIN_RESET);
	door_state = DOOR_OPEN;
}

void Door_Close() {
	uint16_t battery_val;

	Bell_Door_Close();
	HAL_GPIO_WritePin(MOTOR_SLEEP_GPIO_Port, MOTOR_SLEEP_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(MOTOR_SLEEP_GPIO_Port, MOTOR_SLEEP_Pin, GPIO_PIN_RESET);
	door_state = DOOR_CLOSE;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	battery_val = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// 알카라인 AAx4 전압이 1.3*4=5.2V 이하로 내려갔을 때 5.2/2/3.3*4096 = 3227(1:1전압분배)
	if(battery_val < 3227) {
		Bell_Warning();
	}
}
