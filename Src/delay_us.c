/*
 * delay_us.c
 *
 *  Created on: Dec 30, 2022
 *      Author: user
 */

#include "delay_us.h"

void Delay_us(__IO uint32_t nCount) {
	uint32_t count = nCount*8-6;
	while(count--);
}
