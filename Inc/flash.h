/*
 * flash.h
 *
 *  Created on: Jan 3, 2023
 *      Author: user
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#define ADDR_FLASH_NFC_ID	((uint32_t)0x0801F800) // ADDR_FLASH_PAGE_126
#define ADDR_FLASH_PASSWORD ((uint32_t)0x0801FC00) // ADDR_FLASH_PAGE_127

void Flash_Erase_Page(uint32_t page);
void Flash_Write_NFC(uint32_t *id, int8_t num);
void Flash_Write_Password(uint32_t *pwd);

#endif /* INC_FLASH_H_ */
