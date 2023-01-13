/*
 * button.c
 *
 *  Created on: 2023. 1. 9.
 *      Author: user
 */

#include "touch_keypad.h"
#include "password.h"
#include "nfc.h"
#include "led.h"
#include "bell.h"
#include "flash.h"

/*
 * 등록 버튼을 누르면 호출되는 함수.
 * 비밀번호 변경 및 초기화, NFC ID 등록 및 삭제 작업을 수행한다.
 *
 * 비밀번호 변경 : 변경할 비밀번호 입력(4~12) -> * -> 변경음
 * 비밀번호 초기화 : ### -> 초기화 모드 진입음 -> *
 * NFC ID 등록 : 등록할 NFC 태그 -> 인식음 -> (다중 등록시 반복) -> * -> 등록음
 * NFC ID 초기화 : ### -> 초기화 모드 진입음 -> #
 */
void Register_Button() {
	uint8_t temp_password[PASSWORD_BUFFER_LENTH];
	uint8_t temp_id[5];
	uint8_t read, old_read = INVALID_VAL, index = 0;
	uint8_t timeout = 0, reset_count = 0;

	Bell_Correct();
	Led_On();

	while(1) {
		read = Touch_Keypad_Read();
		HAL_Delay(200);
		if(timeout++ > 50) {
			Bell_Wrong();
			break;
		}
		// NFC 등록
		if(NFC_Request(PICC_REQA, temp_id) == NFC_OK) {
			NFC_Set(temp_id);
			break;
		}
		// 비밀번호 및 NFC 초기화
		if(read == INVALID_VAL || read == old_read) {
			old_read = read;
			continue;
		}
		old_read = read;

		if(read == KEY_HASH) {
			if(reset_count > 1) {
				Bell_Correct();
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

					if(read == KEY_AST) {
						Password_Reset();
						Bell_Correct();
						break;
					}
					else if(read == KEY_HASH) {
						NFC_Clear();
						Bell_Correct();
						break;
					}
					else {
						Bell_Wrong();
						break;
					}
				}
				break;
			}
			else {
				reset_count++;
				Bell_Touch();
				continue;
			}
		}
		// 비밀번호 변경
		temp_password[index++] = read;

		if(index > PASSWORD_MAX_LENTH + 1) {
			Bell_Wrong();
			break;
		}

		if(temp_password[index - 1] == KEY_AST) {
			if(index < PASSWORD_MIN_LENTH)
			{
				Bell_Wrong();
				break;
			}
			else {
				index = 0;
				while(password[index] != KEY_AST) {
					password[index] = temp_password[index];
					index++;
				}
				password[index + 1] = KEY_AST;
				Bell_Correct();

				Flash_Erase_Page(ADDR_FLASH_PASSWORD);
				Flash_Write_Password((uint32_t*)password);
				break;
			}
		}
		Bell_Touch();
	}

	Led_Off();
	HAL_Delay(2000);
}

