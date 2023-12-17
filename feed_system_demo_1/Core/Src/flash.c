
#include "flash.h"
#include "stm32f1xx_hal_flash.h"

#include <stdlib.h>

// 不检查的写入
// WriteAddr:起始地址
// pBuffer:  数据指针
// NumToWrite:字节数
void FlashWriteNoCheck( uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite )
{
    uint16_t i;

    for( i=0; i<NumToWrite; i+=4 )
    {
        while( HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr+i,*(uint32_t *)(pBuffer+i) ) );
    }

}

extern void FLASH_PageErase(uint32_t PageAddress);

void FlashWriteBuff( const uint32_t destination_address, uint8_t *const buffer,uint32_t length )
{
    uint16_t i;
    uint8_t FlashBuff[FMC_SECTOR_SIZE];
    uint32_t StartAddress = destination_address - destination_address%FMC_SECTOR_SIZE;
    uint16_t Offset = destination_address - StartAddress;
    uint8_t *pBuf = buffer;
    uint32_t remaindNum = length;

    HAL_StatusTypeDef status = HAL_ERROR;

    // 地址检查
    if( (destination_address < FMC_FLASH_BASE) || ( destination_address + length >= FMC_FLASH_END) || (length <= 0) )
        return;

    HAL_FLASH_Unlock();	// 解锁
    do
    {
        // 读出一页数据
        for(i=0; i < FMC_SECTOR_SIZE; i += 4 )
            *(uint32_t *)(FlashBuff+i) = *(uint32_t *)(StartAddress+i);

        // 修改要改入的数据
        for ( i=0; (i+Offset)<FMC_SECTOR_SIZE && i< remaindNum; i++ )
            *(FlashBuff+Offset+i) = *(pBuf+i);


        // 擦除一ROW数据
        FLASH_PageErase( StartAddress );

        // HAL库 FLASH_PageErase有BUFF,要加上下面三行代码
        while( status != HAL_OK )
            status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
        CLEAR_BIT(FLASH->CR, FLASH_CR_PER);

        // 写入数据
        FlashWriteNoCheck( StartAddress,FlashBuff,FMC_SECTOR_SIZE );

        // 为下一页做准备
        StartAddress +=  FMC_SECTOR_SIZE;
        remaindNum -= i;
        pBuf += i;
        Offset = 0;

    } while( remaindNum > 0 );

    HAL_FLASH_Lock();  // 上锁
		
}




// 从FLASH中读指定长度数据
void FlashReadBuff(const uint32_t source_address,uint8_t *const buffer,uint16_t length)
{
    uint16_t i;
    uint8_t Offset = 0;
    uint32_t StartAddress = source_address;
    uint16_t data;

    // 地址检测
    if( source_address + length > FMC_FLASH_END ) return;

    // 如果没有对16齐
    if( source_address & 1 )
    {
        Offset = 1;
        StartAddress = source_address-1;
    }

    // flash的操作要求16对齐 最小读写操作16个比特
    if ( Offset )
    {
        data = *(uint16_t *)(StartAddress);
        buffer[0] = data >> 8;
        StartAddress += 2;
    }

    for ( i = 0; i < (length-Offset); i += 2)
    {
        data = *(uint16_t *)(StartAddress+i);
        buffer[i+Offset] = (data & 0xFF);
        if ( (i+Offset) < (length - 1) )
            buffer[i + Offset + 1] = (data >> 8);
    }

}



// 写入字符串到Flash
void flash_write_string(char* data)
{
    uint32_t length = 0;

    // 计算字符串长度
    while (data[length] != '\0')
    {
        length++;
    }

    // 解锁Flash
    HAL_FLASH_Unlock();

    // 擦除Flash扇区
    FLASH_PageErase(DEVICE_INFO_ADDRESS);
    FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    CLEAR_BIT(FLASH->CR, FLASH_CR_PER);

    // 写入字符串长度到第一个地址
    FlashWriteNoCheck(DEVICE_INFO_ADDRESS, (uint8_t*)&length, sizeof(uint32_t));

    // 写入字符串内容从第二个地址开始
    FlashWriteNoCheck(DEVICE_INFO_ADDRESS + sizeof(uint32_t), (uint8_t*)data, length);

    // 上锁Flash
    HAL_FLASH_Lock();
}


// 读取字符串从Flash
char* flash_read_string()
{
    // 读取字符串长度
    uint32_t length;
    FlashReadBuff(DEVICE_INFO_ADDRESS, (uint8_t*)&length, sizeof(uint32_t));

    // 分配内存用于存储字符串
    char* buffer = (char*)malloc(length + 1);

    if (buffer != NULL)
    {
        // 从第二个地址开始读取字符串内容
        FlashReadBuff(DEVICE_INFO_ADDRESS + sizeof(uint32_t), (uint8_t*)buffer, length);

        // 添加字符串结束符
        buffer[length] = '\0';
    }

    return buffer;
}
