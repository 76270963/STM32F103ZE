/*
 * bootload.c
 *
 *  Created on: Aug 13, 2025
 *      Author: Administrator
 */

#include "bootload.h"


pFunction JumpToApplication;
uint32_t JumpAddress;


uint16_t crc_table[256]={               /* CRC余式表 */
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0  };


/*******************************************************************************
*   函数名称: fun
*   功能描述: 十六进制转十进制
*   输入参数: s -- 十六进制数
*   返 回 值: 校验值
*******************************************************************************/
uint8_t fun(uint16_t s)
{
     int re=0;   // 保存转换为10进制的结果
     int k=16;   // 16进制
     int n=1;    // 位权
     while(s != 0)
     {
         re += (s%10)*n;  // 取出各位位码值，并乘以对应的位权值
         s /= 10;   // 去掉16进制数的最低位，次低位变为最低位
         n *= k;     // 位权乘以16
     }
    return re;
}


/*******************************************************************************
*   函数名称: CRC16
*   功能描述: 查表法计算CRC16.
*   输入参数: pData -- 待校验数据
*             nLength -- 数据长度
*   返 回 值: 校验值
*******************************************************************************/


uint16_t GetCrc_16(uint8_t * pData, uint16_t nLength)
{
	uint16_t cRc_16 = 0x0000;    // 初始化
    while(nLength>0)
    {
        cRc_16 = (cRc_16 << 8) ^ crc_table [((cRc_16>>8) ^ *pData) & 0xff];
        nLength--;
        pData++;
    }
    return cRc_16;
}


void SendBootLoad(uint8_t cmd)
{
	uint16_t size = 0;
	uint16_t Crc16 = 0;
	buffer_TX1[0] = buffer_RX1[0];
	buffer_TX1[1] = buffer_RX1[1];
	buffer_TX1[2] = cmd;
	if(cmd == 0x01) //回复握手
	{
		buffer_TX1[3] = 0x00;
		buffer_TX1[4] = 0x02;
		buffer_TX1[5] = buffer_RX1[5];
		buffer_TX1[6] = buffer_RX1[6];
		Crc16 = GetCrc_16(buffer_TX1,7);
		buffer_TX1[8] = Crc16 >> 8;              //高位
		buffer_TX1[7] = Crc16 & 0xFF;            //低位
		size = 9;
	}
	else if(cmd == 0x02)
	{
		buffer_TX1[3] = 0x00;
		buffer_TX1[4] = 0x02;
		buffer_TX1[5] = buffer_RX1[5];
		buffer_TX1[6] = buffer_RX1[6];
		Crc16 = GetCrc_16(buffer_TX1,7);
		buffer_TX1[8] = Crc16 >> 8;              //高位
		buffer_TX1[7] = Crc16 & 0xFF;            //低位
		size = 9;
	}
	else if(cmd == 0x03)
	{
		buffer_TX1[3] = 0x04;
		buffer_TX1[4] = 0x02;
		buffer_TX1[5] = buffer_RX1[5];
		buffer_TX1[6] = buffer_RX1[6];
		Crc16 = GetCrc_16(buffer_TX1,1031);
		buffer_TX1[1031] = Crc16 >> 8;              //高位
		buffer_TX1[1032] = Crc16 & 0xFF;            //低位
		size = 1033;
	}
	usartSend(buffer_TX1,size);
}


void BootLoad(void)
{
	uint8_t   cmd;
	uint16_t  blocks;
	uint32_t  maxAdder;
	uint8_t   bin[1024];
	cmd = buffer_RX1[2];
	if(cmd == 0x01) //握手
	{
		blocks = (buffer_RX1[6] << 8) + buffer_RX1[5];
		maxAdder = blocks * 1024;
		SendBootLoad(cmd);
	}
	else if(cmd == 0x02) //写
	{
		blocks = ((buffer_RX1[6] << 8) + buffer_RX1[5]) - 1;  //块数
		maxAdder = FLASH_USER_START_ADDR + (blocks * 1024);
		for(uint16_t i=0; i<1024; i++)
		{
			bin[i] = buffer_RX1[7 + i];
		}
		write_flash(maxAdder, (uint16_t*)&bin, 1024/2);
		SendBootLoad(cmd);
	}
	/*
	else if(cmd == 0x03)
	{
		uint16_t flash_buf[512];
		blocks = ((buffer_RX1[6] << 8) + buffer_RX1[5]) - 1;  //块数
		maxAdder = FLASH_USER_START_ADDR + (blocks * 1024);
		Read_Flash(maxAdder, flash_buf, 512);
		for(uint16_t i=0; i<512; i++)
		{
			buffer_TX1[7 + (2 * i)] = flash_buf[i] >> 8;
			buffer_TX1[8 + (2 * i)] = flash_buf[i] & 0xFF;
		}
		SendBootLoad(cmd);
	}
	else if(cmd == 0x04)
	{
		uint32_t OTASIGN[4] = {0x22222222,0x22222222,0x22222222,0x22222222};
		Write_Flash(FLASH_OTA_ADDR,OTASIGN,4);
		JumpAddress = *(__IO uint32_t*) (FLASH_USER_START_ADDR + 4);
		JumpToApplication = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) FLASH_USER_START_ADDR);
		JumpToApplication();
	}
	*/
}
