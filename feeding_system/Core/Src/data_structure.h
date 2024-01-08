#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdint.h>

typedef struct
{
    int h;
    int m;
    int s;
} Time;

typedef struct
{
    int year;
    int month;
    int day;
} Date;

// 结构体表示每个计划
typedef struct
{
    int id;
    int device;
    Time time; // 假设时间是字符串，以便于处理
    int duration;
    Date beginDate;
    Date endDate;
    int isRuning;
} Plan;

typedef struct
{
    Plan *plans;
    int length;
    int planIdCounter;
} PlanList;

// 结构体表示每个记录
typedef struct
{
    int id;
    char *name;
} Device;
typedef struct
{
    Device *devices;
    int length;
    int deviceIdCounter;
} DeviceList;
typedef struct
{
    int plans;
    int device;
} Record;
// 主结构体，包含整个数据

typedef struct
{
    Plan plans;
    Record records;
    Device Device;
} FeedingDatas;

// 函数声明
void setDate(Date *date, int year, int month, int day);
void setTime(Time *time, int h, int m, int s);
Date *newDate(int year, int month, int day);
Time *newTime(int h, int m, int s);

void insertPlan(PlanList *planList, int device, Time time, int duration, Date beginDate, Date endDate);

void planToString(Plan* plan, char* string);

void insertDevice(DeviceList *deviceList, const char *name);
void deviceToString(DeviceList *deviceList, char **string);

void deletePlanById(PlanList *planList, int id);
void deleteDeviceById(DeviceList *deviceList, int id);
void newDeviceList(DeviceList **deviceList);
void newPlanList(PlanList **planList);
#endif /* DATA_STRUCTURE_H */
