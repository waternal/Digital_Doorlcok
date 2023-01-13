/*
 * password.c
 *
 *  Created on: Dec 30, 2022
 *      Author: user
 */

#include "stm32f1xx.h"
#include "password.h"
#include "touch_keypad.h"
#include "bell.h"
#include "led.h"
#include "flash.h"
#include "door.h"

#include <stdio.h>
extern int _write(int file, unsigned char* p, int len);

// page127에 저장된 비밀번호를 가져와 password 배열에 저장하는 함수.
void Password_Init() {
	uint8_t i;

	for(i = 0; i < PASSWORD_BUFFER_LENTH; i++) {
		password[i] = *((uint8_t*)(ADDR_FLASH_PASSWORD + i));
	}

	for(i = 0; i < PASSWORD_BUFFER_LENTH; i++) {
		printf("%d ", password[i]);
	}
}

// 비밀번호를 12345로 초기화하고 page127에 저장하는 함수.
void Password_Reset() {
	uint8_t i;

	password[0] = KEY_1;
	password[1] = KEY_2;
	password[2] = KEY_3;
	password[3] = KEY_4;
	password[4] = KEY_5;
	password[5] = KEY_AST;

	for(i = 6; i < PASSWORD_BUFFER_LENTH; i++) {
		password[i] = INVALID_VAL;
	}

	Flash_Erase_Page(ADDR_FLASH_PASSWORD);
	Flash_Write_Password((uint32_t*)password);
}

/*
 * main함수 INPUT모드 진입 시 비밀번호를 입력받아 문을 여는 함수. 앞 허수기능을 지원한다.
 * 타임아웃 시간 10초가 지나거나 문을 여는데 성공 또는 실패하면 함수를 나간다.
 * 비밀번호를 입력받으면 tmp_password 배열에 순환하며 저장하고
 * *을 입력하면 password에 저장된 비밀번호에서 *의 위치를 찾아서
 * ast_index에 저장 후 그 위치부터 역으로 비교환다.
 */
void Password_Input() {
	uint8_t tmp_password[PASSWORD_MAX_LENTH+1] = {0,};
	uint8_t read, old_read = INVALID_VAL, timeout = 0;
	uint8_t ast_index = 0, tmp_index = 0, i = 0;

	HAL_Delay(100);
	old_read = Touch_Keypad_Read();

	Led_On();
	while(1) {
		read = Touch_Keypad_Read();

		HAL_Delay(200);
		if(timeout++ > 50) {
			Bell_Wrong();
			break;
		}

		if(read == INVALID_VAL || read == old_read) {
			old_read = read;
			continue;
		}
		old_read = read;
		Bell_Touch();

		tmp_password[tmp_index++] = read;
		if(tmp_index > PASSWORD_MAX_LENTH)
			tmp_index = 0;

		if(read == KEY_AST) {
			for(i=0; i<PASSWORD_MAX_LENTH+1; i++) {
				if(password[i] == KEY_AST) {
					ast_index = i;
					break;
				}
			}
			while(ast_index) {
				if(tmp_index == 0) tmp_index = PASSWORD_MAX_LENTH;
				else			   tmp_index--;

				if(password[ast_index] != tmp_password[tmp_index]) {
					Bell_Wrong();
					Led_Off();
					return;
				}
				ast_index--;
			}
			Door_Open();
			break;
		}
	}
	Led_Off();
}

