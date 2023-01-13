/*
 * nfc.h
 *
 *  Created on: Jan 4, 2023
 *      Author: user
 */

#ifndef INC_NFC_H_
#define INC_NFC_H_

#define MAX_LEN_FROM_CARD 16
#define ID_BUFFER_SIZE 100

// MFRC522 command
#define PCD_Idle					0x00 // 하던 것을 멈춘다.
#define PCD_CalcCRC					0x03
#define PCD_Transmit				0x04
#define PCD_Receive					0x08
#define PCD_Transceive				0x0C
#define PCD_SoftReset				0x0F

// Mifare_Classic 카드 command
#define PICC_REQA					0x26 // REQA를 수신한 PICC는 ATQA로 회신
#define PICC_Anticollision_CL1_H	0x93
#define PICC_Anticollision_CL1_L	0x20
#define PICC_Halt_H					0x50
#define PICC_Halt_L					0x00

#define NFC_OK                 0
#define NFC_ERR                1

//------------------MFRC522 Register---------------
//Page 0:Command and Status
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivlEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
//Page 1:Command
#define     ModeReg               0x11
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
//Page 2:CFG
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D

void NFC_Write(uint8_t reg, uint8_t data);
uint8_t NFC_Read(uint8_t reg);

void NFC_SetBit(uint8_t reg, uint8_t mask);
void NFC_ClearBit(uint8_t reg, uint8_t mask);

void NFC_AntennaOn();
void NFC_AntennaOff();

void NFC_Reset(void);
void NFC_Init(void);

uint8_t NFC_ToCard(uint8_t cmd, uint8_t *send_data, uint8_t send_len,
		uint8_t *back_data, uint16_t* back_bits);
uint8_t NFC_Request(uint8_t cmd, uint8_t *back_data);
uint8_t NFC_Anticoll(uint8_t *cmd);
void NFC_CalulateCRC(uint8_t *pData_in, uint8_t len, uint8_t *pData_out);
void NFC_Halt(void);
uint8_t NFC_GetId(uint8_t *id);
void NFC_Clear();
void NFC_Set(uint8_t* id);
void NFC_Check();

#endif /* INC_NFC_H_ */
