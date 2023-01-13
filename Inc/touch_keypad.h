/*
 * touch_keypad.h
 *
 *  Created on: Dec 30, 2022
 *      Author: user
 */

#ifndef INC_TOUCH_KEYPAD_H_
#define INC_TOUCH_KEYPAD_H_

#include "stm32f1xx.h"

#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 9
#define KEY_8 10
#define KEY_9 11
#define KEY_AST 13
#define KEY_0 14
#define KEY_HASH 15

#define INVALID_VAL 0
#define NO_USED_1 4
#define NO_USED_2 8
#define NO_USED_3 12
#define No_USED_4 16

uint8_t Touch_Keypad_Read();

#endif /* INC_TOUCH_KEYPAD_H_ */
