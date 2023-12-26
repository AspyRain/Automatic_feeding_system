#include "data_structure.h"
#include <stdlib.h>
#include "rtthread.h"
#include "timer.h"
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
    newPlan.id = planCount;
    newPlan.device = device;
    newPlan.time = time;
    newPlan.duration = duration;
    newPlan.beginDate = beginDate;
    newPlan.endDate = endDate;

    // 调整 plans 数组的大小
    *plans = realloc(*plans, (planCount + 1) * sizeof(Plan));

    if (*plans != NULL) {
        // 添加新计划
        (*plans)[planCount] = newPlan;
        (planCount)++;
    } else {
        // 处理内存分配失败
        // 您可能希望记录错误或采取适当的措施
    }
}