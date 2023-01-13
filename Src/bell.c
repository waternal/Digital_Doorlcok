/*
 * buzzer.c
 *
 *  Created on: Dec 31, 2022
 *      Author: user
 */

#include "main.h"
#include "stm32f1xx.h"
#include "bell.h"

enum notes {
	C = 138, D = 123, E = 109,
	F = 103, G = 92
};

uint16_t bell_correct[3] = {G, F, G};
uint16_t bell_door_open[4] = {F, E, F, G};
uint16_t bell_door_close[3] = {E, F, G};
uint8_t i;

void Bell_Touch() {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	TIM1->PSC = G;
	HAL_Delay(150);

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void Bell_Wrong() {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	TIM1->PSC = C;
	HAL_Delay(150);

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void Bell_Correct() {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	for(i=0; i<3; i++) {
		TIM1->PSC = bell_correct[i];
		HAL_Delay(200);
	}

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void Bell_Door_Open() {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	for(i=0; i<4; i++) {
		TIM1->PSC = bell_door_open[i];
		HAL_Delay(200);
	}

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void Bell_Door_Close() {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	for(i=0; i<3; i++) {
		TIM1->PSC = bell_door_close[i];
		HAL_Delay(200);
	}

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void Bell_Warning() {
	for(i=0; i<4; i++) {
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		TIM1->PSC = G;
		HAL_Delay(150);
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		HAL_Delay(150);
	}
}
