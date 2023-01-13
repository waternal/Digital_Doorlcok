/*
 * nfc.c
 *
 *  Created on: Jan 4, 2023
 *      Author: user
 */

#include "main.h"
#include "stm32f1xx.h"
#include "nfc.h"
#include "flash.h"
#include "touch_keypad.h"
#include "bell.h"
#include "door.h"

extern SPI_HandleTypeDef hspi2;

void NFC_Write(uint8_t reg, uint8_t data) {
	uint8_t pData[2] = {((reg<<1) & 0x7E), data};

	HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, pData, 2, 500);
	HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_SET);
}

uint8_t NFC_Read(uint8_t reg) {
	uint8_t data;
	reg = ((reg<<1) & 0x7E) | 0x80;

	HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &reg, 1, 500);
	HAL_SPI_Receive(&hspi2, &data, 1, 500);
	HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_SET);

	return data;
}

void NFC_SetBit(uint8_t reg, uint8_t mask) {
    NFC_Write(reg, NFC_Read(reg) | mask);
}

void NFC_ClearBit(uint8_t reg, uint8_t mask) {
	NFC_Write(reg, NFC_Read(reg) & (~mask));
}

void NFC_AntennaOn() {
	NFC_SetBit(TxControlReg, 0x03);
}

void NFC_AntennaOff() {
	NFC_ClearBit(TxControlReg, 0x03);
}

void NFC_Reset(void) {
	NFC_Write(CommandReg, PCD_SoftReset);
}

/*
 * NFC 초기화 함수.
 * 100% ASK modulation, CRC Initial value 0x6363, reload time을 25ms로 설정.
 * timer = 6.78MHz/TPreScaler, reload time = 1/timer * reload value
 * TPreScaler = 0x0A9 = 169, timer = 6.78MHz / 169 = 40kHz
 * reload value = 0x3E8 = 1000, reload time = 1/40*1000 = 25ms
 */
void NFC_Init(void) {
	NFC_Reset();

	NFC_Write(TModeReg, 0x80);
	NFC_Write(TPrescalerReg, 0xA9);
	NFC_Write(TReloadRegH, 0x03);
	NFC_Write(TReloadRegL, 0xE8);
	NFC_Write(TxAutoReg, 0x40);
	NFC_Write(ModeReg, 0x3D);

	NFC_AntennaOn();
}


// MFRC522모듈과 NFC카드가 통신하도록 MFRC522에 명령어를 보내는 함수. 성공여부를 반환.
uint8_t NFC_ToCard(uint8_t cmd, uint8_t *send_data, uint8_t send_len,
		uint8_t *back_data, uint16_t *back_bits) {
	uint8_t status = NFC_ERR;
	uint8_t irq_en = 0x00;
	uint8_t wait_irq = 0x00;
	uint8_t last_bits;
	uint8_t n;
	uint16_t i;

	if(cmd == PCD_Transceive) {
		irq_en = 0x77;
		wait_irq = 0x30;
	}

	NFC_Write(CommIEnReg, irq_en | 0x80);
	NFC_ClearBit(CommIrqReg, 0x80);
	NFC_SetBit(FIFOLevelReg, 0x80);
	NFC_Write(CommandReg, PCD_Idle);

	for (i = 0; i < send_len; i++) {
		NFC_Write(FIFODataReg, send_data[i]);
	}

	NFC_Write(CommandReg, cmd);
	if (cmd == PCD_Transceive)
		NFC_SetBit(BitFramingReg, 0x80);

	i = 2000;
	do {
		n = NFC_Read(CommIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&wait_irq));

	NFC_ClearBit(BitFramingReg, 0x80);

	if (i != 0) {
		if(!(NFC_Read(ErrorReg) & 0x1B)) {
			status = NFC_OK;
			if (n & irq_en & 0x01)
			  status = NFC_ERR;

			if(cmd == PCD_Transceive) {
				n = NFC_Read(FIFOLevelReg);
				last_bits = NFC_Read(ControlReg) & 0x07;
				if(last_bits) {
					*back_bits = (n-1)*8 + last_bits;
				}
				else {
					*back_bits = n*8;
				}

				if (n == 0) 		n = 1;
				if (n > MAX_LEN_FROM_CARD)	n = MAX_LEN_FROM_CARD;

				for (i=0; i<n; i++) {
					  back_data[i] = NFC_Read(FIFODataReg);
				}
			}
		}
		else {
			status = NFC_ERR;
		}
	}

	return status;
}

uint8_t NFC_Request(uint8_t cmd, uint8_t *back_data) {
	uint8_t status;
	uint16_t back_bits;

	NFC_Write(BitFramingReg, 0x07);

	back_data[0] = cmd;

	status = NFC_ToCard(PCD_Transceive, back_data, 1, back_data, &back_bits);
	if ((status != NFC_OK) || (back_bits != 0x10)) {
		status = NFC_ERR;
	}

	return status;
}

/*
 * Anticollision을 수행하고 이 과정에서 id를 얻는다.
 * 4byte id만 고려한다.
 */
uint8_t NFC_Anticoll(uint8_t *cmd) {
	uint8_t status;
	uint8_t i;
	uint8_t check_bcc=0;
	uint16_t len;

	NFC_Write(BitFramingReg, 0x00);

	cmd[0] = PICC_Anticollision_CL1_H;
	cmd[1] = PICC_Anticollision_CL1_L;
	status = NFC_ToCard(PCD_Transceive, cmd, 2, cmd, &len);

	// BCC비트로 데이터가 제대로 왔는지 확인
	if (status == NFC_OK) {
		for (i=0; i<4; i++) {
		  check_bcc ^= cmd[i];
		}
		if (check_bcc != cmd[i]) {
		  status = NFC_ERR;
		}
	}

	return status;
}

void NFC_CalulateCRC(uint8_t *pData_in, uint8_t len, uint8_t *pData_out) {
	uint8_t i, n;

	NFC_ClearBit(DivIrqReg, 0x04);
	NFC_SetBit(FIFOLevelReg, 0x80);

	for (i=0; i<len; i++) {
		NFC_Write(FIFODataReg, *(pData_in+i));
	}
		NFC_Write(CommandReg, PCD_CalcCRC);

	// CRC 계산
	i = 200;
	do {
		n = NFC_Read(DivIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x04));

	pData_out[0] = NFC_Read(CRCResultRegL);
	pData_out[1] = NFC_Read(CRCResultRegM);
}

void NFC_Halt(void) {
	uint16_t len;
	uint8_t buff[4];

	buff[0] = PICC_Halt_H;
	buff[1] = PICC_Halt_L;
	NFC_CalulateCRC(buff, 2, &buff[2]);

	NFC_ToCard(PCD_Transceive, buff, 4, buff, &len);
}

/*
 * NFC 통신에 성공하면 id를 얻고 성공유무를 반환하는 함수.
 * 4byte 길이 id만 고려한다.
 */
uint8_t NFC_GetId(uint8_t *id) {
	uint8_t status = NFC_ERR;

	status = NFC_Request(PICC_REQA, id);
	if (status == NFC_OK) {
		status = NFC_Anticoll(id);
	}
	NFC_Halt();

	return status;
}

void NFC_Clear() {
	Flash_Erase_Page(ADDR_FLASH_NFC_ID);
}

/*
 * NFC ID를 차례로 입력받아 Flash memory page 126에 등록하는 함수.
 * NFC ID를 차례로 입력 후 *을 누르면 등록. page 126을 지우고 차례로 등록한다.
 * 최대 등록 가능 개수는 100개(ID_BUFFER_SIZE).
 * timeout은 10초, NFC ID를 인식할 때마다 초기화.
 */
void NFC_Set(uint8_t* id) {
	uint8_t i, count = 0, timeout = 0;
	uint32_t temp_id[ID_BUFFER_SIZE];

	while(Touch_Keypad_Read() != KEY_AST) {
		HAL_Delay(200);
		if(timeout++ > 50) {
			Bell_Wrong();
			break;
		}

		if(NFC_GetId(id) == NFC_OK) {
			if(count > ID_BUFFER_SIZE) {
				Bell_Wrong();
				continue;
			}
			// 중복 인식 방지
			if(count > 0 && *((uint32_t*)id) == temp_id[count-1]) {
				continue;
			}
			// 중복 등록 방지
			for(i=0; i<count; i++) {
				if(*((uint32_t*)id) == temp_id[i]) {
					Bell_Wrong();
					continue;
				}
			}
			temp_id[count++] = *((uint32_t*)id);
			Bell_Touch();
			timeout = 0;
		}
	}

	if(timeout <= 50) {
		Flash_Erase_Page(ADDR_FLASH_NFC_ID);
		for(i=0; i<count; i++) {
			Flash_Write_NFC(temp_id + i, i);
		}
		Bell_Correct();
	}
}

void NFC_Check() {
	uint8_t id[5];
	uint8_t i;

	if(NFC_GetId(id) == NFC_OK) {
		for(i=0; i<ID_BUFFER_SIZE; i++) {
			if(*((uint32_t*)id) == *((uint32_t*)(ADDR_FLASH_NFC_ID + i * 4))) {
				Door_Open();
				break;
			}
			else if(*((uint32_t*)(ADDR_FLASH_NFC_ID + i * 4)) == 0xFFFFFFFF) {
				Bell_Wrong();
				break;
			}
		}
	}
}
