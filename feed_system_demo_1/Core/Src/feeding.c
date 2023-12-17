#include "feeding.h"
#include <rtthread.h>
#include "main.h"

// 全局变量，标志每个喂食线程的运行状态
int feed_flags[] = {0, 0, 0};

// 定义三个喂食线程
rt_thread_t feeding_thread_1;
rt_thread_t feeding_thread_2;
rt_thread_t feeding_thread_3;

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
            // 如果标志位为0，将GPIO引脚状态置为低电平
            HAL_GPIO_WritePin(feed_port, feed_pin, GPIO_PIN_RESET);
            rt_thread_mdelay(50);
        }
    }
}

// 切换喂食线程的运行状态
void toggle_feed(int feedNum) {
    rt_thread_t feed_thread;

    // 根据喂食线程编号选择对应的线程
    switch (feedNum) {
    case 1:
        feed_thread = feeding_thread_1;
        break;
    case 2:
        feed_thread = feeding_thread_2;
        break;
    case 3:
        feed_thread = feeding_thread_3;
        break;
    default:
        return;  // 无效的 feedNum
    }

    // 防止线程尝试切换自身
    if (rt_thread_self() == feed_thread) {
        rt_kprintf("Cannot toggle own thread\n");
        return;
    }

    // 根据标志位切换线程的运行状态
    if (feed_flags[feedNum - 1] == 0) {
        feed_flags[feedNum - 1] = 1;
        rt_kprintf("Thread %d started\n", feedNum);
    } else {
        feed_flags[feedNum - 1] = 0;
        rt_kprintf("Thread %d suspended\n", feedNum);
    }
}

// 初始化喂食线程
void feeding_init() {
    // 创建三个喂食线程
    feeding_thread_1 = rt_thread_create("feeding1", feeding, (void*)1, 2048, 5, 10);
    feeding_thread_2 = rt_thread_create("feeding2", feeding, (void*)2, 2048, 5, 10);
    feeding_thread_3 = rt_thread_create("feeding3", feeding, (void*)3, 2048, 5, 10);

    // 启动三个喂食线程
    rt_thread_startup(feeding_thread_1);
    rt_thread_startup(feeding_thread_2);
    rt_thread_startup(feeding_thread_3);
}
