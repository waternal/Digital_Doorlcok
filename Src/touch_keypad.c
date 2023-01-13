/*
 * touch_keypad.c
 *
 *  Created on: Dec 30, 2022
 *      Author: user
 */

#include "main.h"
#include "touch_keypad.h"
#include "delay_us.h"

/*
 * 터치패드 인식값을 반환하는 함수.
 * TTP229 16x16 터치패드에서 사용하지 않는 값을 제외하고 반환한다.
 * 아무것도 인식하지 않았을 때는 INVALID_VAL(0)을 반환.
 */
uint8_t Touch_Keypad_Read() {
	uint8_t count = 0;

	HAL_GPIO_WritePin(TOUCH_KEYPAD_SCL_GPIO_Port, TOUCH_KEYPAD_SCL_Pin, GPIO_PIN_SET);

	while(count < 16) {
		HAL_GPIO_WritePin(TOUCH_KEYPAD_SCL_GPIO_Port, TOUCH_KEYPAD_SCL_Pin, GPIO_PIN_RESET);
		Delay_us(1);
		count++;
		if(!HAL_GPIO_ReadPin(TOUCH_KEYPAD_SDO_GPIO_Port, TOUCH_KEYPAD_SDO_Pin)) {
			if(count == NO_USED_1 || count == NO_USED_2
					|| count == NO_USED_3 || count == No_USED_4) {
				return INVALID_VAL;
			}
			else {
				return count;
			}
		}

		HAL_GPIO_WritePin(TOUCH_KEYPAD_SCL_GPIO_Port, TOUCH_KEYPAD_SCL_Pin, GPIO_PIN_SET);
		Delay_us(1);
	}

	return INVALID_VAL;
}
