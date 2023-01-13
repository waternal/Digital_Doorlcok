/*
 * flash.c
 *
 *  Created on: Jan 3, 2023
 *      Author: user
 */
#include "stm32f1xx.h"
#include "flash.h"
#include "password.h"

/*
 * 인수로 받은 page를 지우는 함수.
 * page126과 page127을 지우는데에만 사용하자.
 */
void Flash_Erase_Page(uint32_t page) {
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.NbPages = 1;
	EraseInitStruct.PageAddress = page;

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	HAL_FLASH_Lock();
}

/*
 * page126에 NFC ID를 등록하는 함수.
 * 인수 num은 (num+1)번째 NFC ID. NFC ID는 4byte로 가정.
 */
void Flash_Write_NFC(uint32_t* id, int8_t num) {
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(TYPEPROGRAM_WORD, ADDR_FLASH_NFC_ID + num * 4, *id);
	HAL_FLASH_Lock();
}

// page126에 비밀번호를 등록하는 함수.
void Flash_Write_Password(uint32_t* pwd) {
	uint8_t i;
	uint32_t end = PASSWORD_BUFFER_LENTH / 4;

	HAL_FLASH_Unlock();
	for(i = 0; i < end; i++) {
		HAL_FLASH_Program(TYPEPROGRAM_WORD, ADDR_FLASH_PASSWORD + i * 4, pwd[i]);
	}

	HAL_FLASH_Lock();
}

