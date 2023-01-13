/*
 * buzzer.h
 *
 *  Created on: Dec 31, 2022
 *      Author: user
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

// TIM1 APB2 72MHz, Prescaler = 144 - 1, Period = 1000 - 1

extern TIM_HandleTypeDef htim1;

void Bell_Touch();
void Bell_Wrong();
void Bell_Correct();
void Bell_Door_Open();
void Bell_Door_Close();
void Bell_Warning();

#endif /* INC_BUZZER_H_ */
