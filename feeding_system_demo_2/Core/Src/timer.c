#include "timer.h"
#include <stdlib.h>

void setAll(Date *now_date, Time *now_time,Plan *now_plans) {
    setDate(now_date);
    setTime(now_time);
    setPlan(now_plans);
}

void setDate(Date *now_date) {
    if (now_date == NULL) {
        date = now_date;
    }
}

void setTime(Time *now_time) {
    if (now_time == NULL) {
        time = now_time;
    }
}

void setPlan(Plan *now_plans){
    if (now_plans == NULL) {
        plans =now_plans;
    }
}

Date *getDate() {
    return date;
}

Time *getTime() {
    return time;
}
/* 
返回参数说明：
0   默认值
1-SIZE_MAX-1    
*/
int* timer(int deviceNum){
    int *device_status = (int *)malloc(deviceNum * sizeof(int));
    time->s+=1;
    if (time->s==60){
        time->s=0;
        time->m+=1;
    }
    if (time->m==60){
        time->m=0;
        time->h+=1;
    }
    if (time->h==24) {
        for (int i=0;i<deviceNum;i++){
            device_status[i]=0;
        }
        return device_status;
    }
    //判断是否有设备到喂食时间
    for (int i=0;i<sizeof(plans)/sizeof(Plan);i++){
        if (isTimeReached(&plans[i]))
        device_status[plans[i].device]=1;
    }
    return device_status;
}
int isTimeReached(const Plan *plan) {
    // 忽略年份，只比较月份和日期
    if (
        (date->month > plan->beginDate.month || (date->month == plan->beginDate.month && date->day >= plan->beginDate.day)) &&
        (date->month < plan->endDate.month || (date->month == plan->endDate.month && date->day <= plan->endDate.day))
    ) {
        if (time->h==plan->time.h&&time->m==plan->time.m){
            return 1;
        }
        else return 0;
    } else {
        return 0; // 不在范围内
    }
    return 0;
}
