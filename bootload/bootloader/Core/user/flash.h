/*
 * flash.h
 *
 *  Created on: Aug 12, 2025
 *      Author: Administrator
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_


#include "main.h"

/**************************************************************************************

Bootloader区  	0x08000000 - 0x08007FFF	32KB	启动逻辑、APP 校验、固件复制、跳转功能
主APP区	        0x08008000 - 0x08047FFF	256KB	正常运行的主应用程序
备份APP区	        0x08048000 - 0x0807FFFF	256KB	接收新固件（OTA 升级时）
参数区	            0x08078000 - 0x0807FFFF	32KB	存储升级标志、版本号、硬件配置等（必须在 Flash 有效地址范围内）


APP应用程序 main 跳转与FLASH_USER_START_ADDR 对应。
SCB->VTOR = FLASH_BASE | 0x8000;
修改 STM32G0B1VETX_FLASH.ld  文件中的 FLASH    (rx)    : ORIGIN = 0x8008000,   LENGTH = 512K -32K

*****************************************************************************************/

/* FLASH起始地址 */
#define STM32_FLASH_SIZE        0x80000         /* STM32 FLASH 总大小 */
#define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH 起始地址 */


/*OTA标记地址*/
#define   FLASH_OTA_ADDR         0x08007800  //2KB

/*OTA保存地址*/
#define   FLASH_USER_START_ADDR  0x08008000
#define   FLASH_USER_EDD_ADDR    0x08047FFF


 /* STM32F103 扇区大小 */
#if STM32_FLASH_SIZE < 256 * 1024
#define STM32_SECTOR_SIZE   1024                /* 容量＜256K */
#else
#define STM32_SECTOR_SIZE   2048                /* 容量≥256K */
#endif


enum
{
  FLASHIF_OK = 0,
  FLASHIF_ERASEKO,
  FLASHIF_WRITINGCTRL_ERROR,
  FLASHIF_WRITING_ERROR,
  FLASHIF_PROTECTION_ERRROR,
  FLASHIF_ADDR_ERROR
};

void Read_Flash(uint32_t raddr, uint16_t *pbuf, uint16_t length);
void write_flash(uint32_t waddr, uint16_t *pbuf, uint16_t length);

#endif /* INC_FLASH_H_ */
