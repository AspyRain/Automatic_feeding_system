#include "data_structure.h"
#include <stdlib.h>
#include "rtthread.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

// 初始化结构体数据
void setDate(Date *date,int year,int month,int day) {
    date->year = year;
    date->month = month;
    date->day = day;
}

void setTime(Time *time,int h,int m,int s) {
    time->h = h;
    time->m = m;
    time->s = s;
}

Date* newDate(int year,int month,int day) {
    Date *date = (Date*)malloc(sizeof(Date));
    if (date != NULL) {
        setDate(date,year,month,day);
    }
    return date;
}

Time* newTime(int h,int m,int s) {
    Time *time = (Time*)malloc(sizeof(Time));
    if (time != NULL) {
        setTime(time,h,m,s);
    }
    return time;
}
void newPlanList(PlanList **planList) {
    // Allocate memory for PlanList
    *planList = (PlanList*)malloc(sizeof(PlanList));

    if (*planList != NULL) {
        // Initialize PlanList members
        (*planList)->plans = NULL;
        (*planList)->length = 0;
        (*planList)->planIdCounter = 0;
    } else {
        // Handle memory allocation failure
        // ...
    }
}

void newDeviceList(DeviceList **deviceList) {
    // Allocate memory for DeviceList
    *deviceList = (DeviceList*)malloc(sizeof(DeviceList));

    if (*deviceList != NULL) {
        // Initialize DeviceList members
        (*deviceList)->devices = NULL;
        (*deviceList)->length = 0;
        (*deviceList)->deviceIdCounter = 0;
    } else {
        // Handle memory allocation failure
        // ...
    }
}

void insertPlan(PlanList* planList,int device,Time time,int duration,Date beginDate,Date endDate) {
    // 为新计划分配内存
    Plan newPlan;
    newPlan.id = planList->planIdCounter++;  // 让 id 自增
    newPlan.device = device;
    newPlan.time = time;
    newPlan.duration = duration;
    newPlan.beginDate = beginDate;
    newPlan.endDate = endDate;
    newPlan.isRuning=-1;
    planList->length++;

    // 临时指针分配内存
    Plan* tempPlans = realloc(planList->plans,planList->length * sizeof(Plan));

    if (tempPlans != NULL) {
        // 分配内存成功，将新指针赋给 planList->plans
        planList->plans = tempPlans;

        // 添加新计划
        planList->plans[planList->length - 1] = newPlan;  // 注意索引是 length - 1
    } else {
        // 处理内存分配失败
        // ...
    }
}

void insertDevice(DeviceList* deviceList,const char* name) {
    // 为新设备分配内存
    Device newDevice;
    newDevice.id = ++(*deviceList).deviceIdCounter;  // 增加设备的 ID
    newDevice.name = strdup(name); // 为名称分配内存并复制数据
    deviceList->length++;
    // 调整 devices 数组的大小
    // 临时指针分配内存
    Device* tempDevice = realloc(deviceList->devices,deviceList->length * sizeof(Device));

    if (tempDevice != NULL) {
        // 分配内存成功，将新指针赋给 deviceList->devices
        deviceList->devices = tempDevice;

        // 添加新计划
        deviceList->devices[deviceList->length - 1] = newDevice;  // 注意索引是 length - 1
    } else {
        // 处理内存分配失败
        // ...
    }
}
void planToString(Plan* plan,char* string) {
    // 计算 JSON 字符串所需的大小
    rt_kprintf("计算 JSON 字符串所需的大小\n");
    size_t requiredSize = snprintf(NULL,0,"{\"id\": %d,\"device\": %d,\"time\": {\"h\": %d,\"m\": %d,\"s\": %d},\"duration\": %d,\"beginDate\": {\"year\": %d,\"month\": %d,\"day\": %d},\"endDate\": {\"year\": %d,\"month\": %d,\"day\": %d}}",
                             plan->id,plan->device,plan->time.h,plan->time.m,plan->time.s,
                             plan->duration,plan->beginDate.year,plan->beginDate.month,plan->beginDate.day,
                             plan->endDate.year,plan->endDate.month,plan->endDate.day);
    rt_kprintf("requiredSize:%d\n",requiredSize);
    // 开始构建 JSON 字符串
    rt_kprintf("开始构建 JSON 字符串\n");
    snprintf(string,requiredSize + 1,"{\"id\": %d,\"device\": %d,\"time\": {\"h\": %d,\"m\": %d,\"s\": %d},\"duration\": %d,\"beginDate\": {\"year\": %d,\"month\": %d,\"day\": %d},\"endDate\": {\"year\": %d,\"month\": %d,\"day\": %d}}",
            plan->id,plan->device,plan->time.h,plan->time.m,plan->time.s,
            plan->duration,plan->beginDate.year,plan->beginDate.month,plan->beginDate.day,
            plan->endDate.year,plan->endDate.month,plan->endDate.day);
}



void deviceToString(DeviceList* deviceList,char** string) {
    // 计算 JSON 字符串所需的大小
    size_t requiredSize = 1; // 用于初始 '['
    for (int i = 0; i < deviceList->length; ++i) {
        Device* device = &(deviceList->devices[i]);
        // 添加每个设备所需的大小
        requiredSize += snprintf(NULL,0,"{\"id\": %d,\"name\": \"%s\"}%s",
                                 device->id,device->name,
                                 (i < deviceList->length - 1) ? "," : "");
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
    snprintf(*string,requiredSize,"[");

    for (int i = 0; i < deviceList->length; ++i) {
        Device* device = &(deviceList->devices[i]);
        // 将每个设备追加到 JSON 字符串中
        snprintf(*string + strlen(*string),requiredSize - strlen(*string),
                "{\"id\": %d,\"name\": \"%s\"}%s",
                device->id,device->name,
                (i < deviceList->length - 1) ? "," : "");
    }

    // 结束 JSON 字符串
    snprintf(*string + strlen(*string),requiredSize - strlen(*string),"]\n");
    rt_kprintf("devices字符串:%s",*string);
}

void deletePlanById(PlanList* planList,int id) {
    int index = -1;

    // Find the index of the plan with the specified ID
    for (size_t i = 0; i < planList->length; ++i) {
        if (planList->plans[i].id == id) {
            index = i;
            break;
        }
    }

    // If the plan is found,remove it
    if (index != -1) {
        memmove(planList->plans + index,planList->plans + index + 1,(planList->length - index - 1) * sizeof(Plan));
        planList->length -= 1;
        planList->plans = realloc(planList->plans,planList->length * sizeof(Plan));
    }
}


void deleteDeviceById(DeviceList* deviceList,int id) {
    int index = -1;

    // Find the index of the device with the specified ID
    for (int i = 0; i < deviceList->length; ++i) {
        if (deviceList->devices[i].id == id) {
            index = i;
            break;
        }
    }

    // If the device is found,remove it
    if (index != -1) {
        free(deviceList->devices[index].name);
        memmove(deviceList->devices + index,deviceList->devices + index + 1,(deviceList->length - index - 1) * sizeof(Device));
        deviceList->length -= 1;
        deviceList->devices = realloc(deviceList->devices,deviceList->length * sizeof(Device));
    }
}

