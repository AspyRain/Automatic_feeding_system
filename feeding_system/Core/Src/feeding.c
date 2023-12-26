#include "feeding.h"
#include <rtthread.h>
#include "main.h"


// 全局变量，标志每个喂食线程的运行状态
int feed_flags[] = {0, 0, 0};

// 定义三个喂食线程
rt_thread_t feeding_thread[3];


// 喂食线程的入口函数
void feeding(void* parameter) {
    int feedNum = (int)parameter;
    GPIO_TypeDef* feed_port = GPIOA;
    uint16_t feed_pin;

    // 根据喂食线程编号选择对应的GPIO引脚
    switch (feedNum) {
    case 1:
        feed_pin = feeding_1_Pin;
        break;
    case 2:
        feed_pin = feeding_2_Pin;
        break;
    case 3:
        feed_pin = feeding_3_Pin;
        break;
    default:
        break;
    }

    while (1) {
        if (feed_flags[feedNum - 1] == 1) {
            // 如果标志位为1，切换GPIO引脚状态
            HAL_GPIO_TogglePin(feed_port, feed_pin);
            rt_thread_mdelay(1000);
        } else {
            // 如果标志位为0，将GPIO引脚状态置为低电平,并删除线程
            HAL_GPIO_WritePin(feed_port, feed_pin, GPIO_PIN_RESET);
            rt_thread_t thread = rt_thread_self(); // 获取当前线程的控制块指针
            rt_thread_delete(thread);
        }
    }
}

// 切换喂食线程的运行状态
void toggle_feed(int feedNum) {
    // 根据标志位切换线程的运行状态
    if (feed_flags[feedNum - 1] == 0) {
        feed_flags[feedNum - 1] = 1;
        char feedingName[32];
        rt_sprintf(feedingName,"feeding%d",feedNum);
        feeding_thread[feedNum-1] = rt_thread_create(feedingName, feeding, (void*)feedNum, 2048, 5, 10);
        rt_thread_startup(feeding_thread[feedNum-1]);
        rt_kprintf("Thread %d started\n", feedNum);
    } else {
        feed_flags[feedNum - 1] = 0;
        rt_free(feeding_thread[feedNum-1]);
        rt_kprintf("Thread %d suspended\n", feedNum);
    }
}

// 初始化喂食线程
void feeding_init() {

}
