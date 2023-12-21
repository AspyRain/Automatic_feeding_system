#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdint.h>

typedef struct {
    uint32_t h;
    uint32_t m;
    uint32_t s;
} Time;

typedef struct {
    uint32_t year;
    uint32_t month;
    uint32_t day;
} Date;

// 结构体表示每个计划
typedef struct {
    uint32_t id;
    uint32_t device;
    Time time;  // 假设时间是字符串，以便于处理
    uint32_t duration;
    Date beginDate;
    Date endDate;
} Plan;

// 结构体表示每个记录
typedef struct {
    uint32_t plans;
    uint32_t device;
} Record;

// 主结构体，包含计划和记录
#define MAX_PLANS 3
#define MAX_RECORDS 3

typedef struct {
    Plan plans[MAX_PLANS];
    Record record[MAX_RECORDS];
} DataStructure;

// 函数声明
DataStructure structInit();

#endif /* DATA_STRUCTURE_H */
