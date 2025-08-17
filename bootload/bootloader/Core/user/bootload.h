/*
 * bootload.h
 *
 *  Created on: Aug 13, 2025
 *      Author: Administrator
 */

#ifndef BOOTLOAD_BOOTLOAD_H_
#define BOOTLOAD_BOOTLOAD_H_

#include "main.h"

typedef  void (*pFunction)(void);
void JumpProgram(uint32_t FlashAddr);
void BootLoad(void);

#endif /* BOOTLOAD_BOOTLOAD_H_ */
