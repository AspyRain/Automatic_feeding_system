#ifndef _flash_H_
#define _flash_H_

#include "stm32f1xx_hal.h"

/*********************************************************************/
//                        变量定义
/*********************************************************************/
// 使用最后4k 个字节用来存放电机信息
#define APP_MAX_SIZE        0x00010000  // 

// 定义Flash大小，单位KB
#define FMC_FLASH_SIZE      64
#define FMC_SECTOR_SIZE     2048

// Flash 相关地址定义
#define FMC_FLASH_BASE      FLASH_BASE
#define FMC_FLASH_END       (FMC_FLASH_BASE + FMC_FLASH_SIZE * 1024)

#define DEVICE_INFO_ADDRESS  (FMC_FLASH_END - 0x4000) // 从末尾倒数第16个扇区
#define DEVICE_LOG_ADDRESS   (FMC_FLASH_END - 0x2000) // 从末尾倒数第8个扇区

#define MOD_SECTOR_SIZE     0x7FF
#define PAGE_COUNT_BY16     1024

void flash_erase_sector(uint32_t sector_address);
void flash_write_data(uint32_t flash_address, uint32_t data);
uint32_t flash_read_data(uint32_t flash_address);
void flash_write_all_string(const char* str);
char* flash_read_all_string(void);

#endif
