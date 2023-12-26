// globals.c
#include "main.h"

char usart1_rx_buffer[BUFFER_SIZE];
int usart1_rx_index = 0;
int feedNumFlag = 0;
int esp_flag = 0;
int timeout_flag = 0;