#include "flash.h"
#include <string.h>
#include <stdlib.h>

void flash_erase_sector(uint32_t sector_address) {
    while (FLASH->SR & FLASH_SR_BSY);  // 等待Flash不忙
    FLASH->CR |= FLASH_CR_PER;         // 设置擦除扇区位
    FLASH->AR = sector_address;        // 设置要擦除的扇区地址
    FLASH->CR |= FLASH_CR_STRT;        // 启动擦除操作
    while (FLASH->SR & FLASH_SR_BSY);  // 等待擦除完成
    FLASH->CR &= ~FLASH_CR_PER;        // 清除擦除扇区位
}

void flash_write_data(uint32_t flash_address, uint32_t data) {
    while (FLASH->SR & FLASH_SR_BSY);  // 等待Flash不忙
    FLASH->CR |= FLASH_CR_PG;          // 设置编程位

    // 写入数据到指定地址
    *(__IO uint16_t*)flash_address = (uint16_t)data;
    while (FLASH->SR & FLASH_SR_BSY);  // 等待写入完成

    *(__IO uint16_t*)(flash_address + 2) = (uint16_t)(data >> 16);
    while (FLASH->SR & FLASH_SR_BSY);  // 等待写入完成

    FLASH->CR &= ~FLASH_CR_PG;         // 清除编程位
}

uint32_t flash_read_data(uint32_t flash_address) {
    // 直接读取Flash地址的数据
    return *(__IO uint32_t*)flash_address;
}

void flash_write_all_string(const char* str) {
    int len = strlen(str);

    // 限制字符串长度，确保不超过 Flash 扇区的容量
    if (len > FMC_SECTOR_SIZE - 1) {
        len = FMC_SECTOR_SIZE - 1;
    }

    // 擦除存储字符串的 Flash 扇区
    flash_erase_sector(FMC_FLASH_BASE);

    // 写入字符串长度到 Flash
    flash_write_data(FMC_FLASH_BASE, (uint32_t)len);

    // 写入字符串数据到 Flash
    for (int i = 0; i < len; ++i) {
        flash_write_data(FMC_FLASH_BASE + 4 + i, str[i]);
    }
}

char* flash_read_all_string(void) {
    int len = (int)flash_read_data(FMC_FLASH_BASE);  // 读取字符串长度

    // 分配足够的空间来存储字符串
    char* buffer = (char*)malloc(len + 1);  // 加 1 是为了存储字符串结尾的 null 字符

    if (buffer == NULL) {
        return NULL;
    }

    // 从 Flash 读取数据到缓冲区
    for (int i = 0; i < len; ++i) {
        buffer[i] = (char)flash_read_data(FMC_FLASH_BASE + 4 + i);
    }

    // 确保字符串以 null 终止
    buffer[len] = '\0';

    return buffer;
}
