#include "data_structure.h"
#include <stdlib.h>
#include "rtthread.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

// 初始化结构体数据
void setDate(Date *date, int year, int month, int day) {
    date->year = year;
    date->month = month;
    date->day = day;
}

void setTime(Time *time, int h, int m, int s) {
    time->h = h;
    time->m = m;
    time->s = s;
}

Date* newDate(int year, int month, int day) {
    Date *date = (Date*)malloc(sizeof(Date));
    if (date != NULL) {
        setDate(date, year, month, day);
    }
    return date;
}

Time* newTime(int h, int m, int s) {
    Time *time = (Time*)malloc(sizeof(Time));
    if (time != NULL) {
        setTime(time, h, m, s);
    }
    return time;
}
void insertPlan(Plan** plans, int device, Time time, int duration, Date beginDate, Date endDate) {
    // 为新计划分配内存
    Plan newPlan;
    newPlan.id = planCount++;  // 让 id 自增
    newPlan.device = device;
    newPlan.time = time;
    newPlan.duration = duration;
    newPlan.beginDate = beginDate;
    newPlan.endDate = endDate;

    // 调整 plans 数组的大小
    *plans = realloc(*plans, planCount * sizeof(Plan));

    if (*plans != NULL) {
        // 添加新计划
        (*plans)[planCount - 1] = newPlan;  // 注意索引是 planCount - 1
    } 
}
void insertDevice(Device** devices, const char* name) {
    // 为新设备分配内存
    Device newDevice;
    newDevice.id = ++deviceCount;  // 增加设备的 ID
    newDevice.name = strdup(name); // 为名称分配内存并复制数据

    // 调整 devices 数组的大小
    *devices = realloc(*devices, deviceCount * sizeof(Device));

    if (*devices != NULL) {
        (*devices)[deviceCount - 1] = newDevice;  // 注意索引是 deviceCount - 1
    }
}

void planToString(Plan** plans, int planCount, char** string) {
    // 计算 JSON 字符串所需的大小
    size_t requiredSize = 1; // 用于初始 '['
    for (int i = 0; i < planCount; ++i) {
        Plan* plan = plans[i];
        // 添加每个计划所需的大小
        requiredSize += snprintf(NULL, 0, "{\"id\": %d, \"device\": %d, \"time\": {\"h\": %d, \"m\": %d, \"s\": %d}, \"duration\": %d, \"beginDate\": {\"year\": %d, \"month\": %d, \"day\": %d}, \"endDate\": {\"year\": %d, \"month\": %d, \"day\": %d}}%s",
                                 plan->id, plan->device, plan->time.h, plan->time.m, plan->time.s,
                                 plan->duration, plan->beginDate.year, plan->beginDate.month, plan->beginDate.day,
                                 plan->endDate.year, plan->endDate.month, plan->endDate.day,
                                 (i < planCount - 1) ? ", " : "");
    }
    requiredSize += 2; // 用于闭合 ']' 和空终止符

    // 动态分配 JSON 字符串的内存
    *string = (char*)malloc(requiredSize);
    if (*string == NULL) {
        // 处理分配失败
        // ...
        return;
    }

    // 开始构建 JSON 字符串
    sprintf(*string, "[");

    for (int i = 0; i < planCount; ++i) {
        Plan* plan = plans[i];
        // 将每个计划追加到 JSON 字符串中
        sprintf(*string + strlen(*string), "{\"id\": %d, \"device\": %d, \"time\": {\"h\": %d, \"m\": %d, \"s\": %d}, \"duration\": %d, \"beginDate\": {\"year\": %d, \"month\": %d, \"day\": %d}, \"endDate\": {\"year\": %d, \"month\": %d, \"day\": %d}}%s",
                plan->id, plan->device, plan->time.h, plan->time.m, plan->time.s,
                plan->duration, plan->beginDate.year, plan->beginDate.month, plan->beginDate.day,
                plan->endDate.year, plan->endDate.month, plan->endDate.day,
                (i < planCount - 1) ? ", " : "");
    }

    // 结束 JSON 字符串
    sprintf(*string + strlen(*string), "]\n");
    rt_kprintf("plan字符串:%s\n",*string);
    rt_thread_mdelay(1000);
}
void deviceToString(Device** devices, int deviceCount, char** string) {
    // 计算 JSON 字符串所需的大小
    size_t requiredSize = 1; // 用于初始 '['
    for (int i = 0; i < deviceCount; ++i) {
        Device* device = &((*devices)[i]);
        // 添加每个设备所需的大小
        requiredSize += snprintf(NULL, 0, "{\"id\": %d, \"name\": \"%s\"}%s",
                                 device->id, device->name,
                                 (i < deviceCount - 1) ? ", " : "");
    }
    requiredSize += 2; // 用于闭合 ']' 和空终止符

    // 动态分配 JSON 字符串的内存
    *string = (char*)malloc(requiredSize);
    if (*string == NULL) {
        // 处理分配失败
        // ...
        return;
    }

    // 开始构建 JSON 字符串
    snprintf(*string, requiredSize, "[");

    for (int i = 0; i < deviceCount; ++i) {
        Device* device = &((*devices)[i]);
        // 将每个设备追加到 JSON 字符串中
        snprintf(*string + strlen(*string), requiredSize - strlen(*string), 
                "{\"id\": %d, \"name\": \"%s\"}%s",
                device->id, device->name,
                (i < deviceCount - 1) ? ", " : "");
    }

    // 结束 JSON 字符串
    snprintf(*string + strlen(*string), requiredSize - strlen(*string), "]\n");
    rt_kprintf("devices字符串:%s", *string);
}
