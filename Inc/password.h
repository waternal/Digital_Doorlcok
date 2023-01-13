/*
 * password.h
 *
 *  Created on: Dec 30, 2022
 *      Author: user
 */

#ifndef INC_PASSWORD_H_
#define INC_PASSWORD_H_

#define PASSWORD_BUFFER_LENTH 16
#define PASSWORD_MIN_LENTH 4
#define PASSWORD_MAX_LENTH 12

extern uint8_t password[PASSWORD_BUFFER_LENTH];

void Password_Init();
void Password_Reset();
void Password_Input();

#endif /* INC_PASSWORD_H_ */
