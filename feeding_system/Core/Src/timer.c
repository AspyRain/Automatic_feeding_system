#include "timer.h"
#include <stdlib.h>
#include "rtthread.h"


Timer *timerData = NULL; 
int planCount=0;
int deviceCount = 0; 
int timerInit(Date *now_date,Time *now_time,Plan *now_plan){
    
        // 在运行时动态分配内存给 Timer 对象
    timerData = (Timer *)malloc(sizeof(Timer));

    // 检查内存分配是否成功
    if (timerData == NULL) {
        // 处理内存分配失败的情况
        return 0;
    }

    setTimerAll(now_date,now_time,now_plan);
    return 1;
}

void setTimerAll(Date *now_date, Time *now_time,Plan *now_plans) {
    setTimerDate(now_date);
    setTimerTime(now_time);
    setTimerPlan(now_plans);
}

void setTimerDate(Date *now_date) {
    if (now_date != NULL) {
        timerData->date = now_date;
    }
}

void setTimerTime(Time *now_time) {
    if (now_time != NULL) {
        timerData->time = now_time;
    }
}

void setTimerPlan(Plan *now_plans) {
    if (now_plans != NULL) {
        timerData->plans = now_plans;
    }
}

Date *getTimerDate() {
    return timerData->date;
}

Time *getTimerTime() {
    return timerData->time;
}
/* 
返回参数说明：
0   默认值
1-SIZE_MAX-1    
*/
int timerRun(int deviceNum,int *device_status){
            for (int i=0;i<deviceNum;i++){
            device_status[i]=0;
        }
        int result=0;
    timerData->time->s+=1;
    if (timerData->time->s==60){
        timerData->time->s=0;
        timerData->time->m+=1;
    }
    if (timerData->time->m==60){
        timerData->time->m=0;
        timerData->time->h+=1;
    }
    if (timerData->time->h==24) {
        result=0;
    }
    Plan* plans=timerData->plans;
	int plansNum=planCount;
    int planNum=planCount;
    //判断是否有设备到喂食时间
    rt_kprintf("plansNum:%d\nplanNum:%d\n",plansNum,planNum);
    for (int i=0;i<planNum;i++){
        if (isTimeReached(&plans[i])==1){
           device_status[timerData->plans[i].device]=1; 
           result=1;
        }
    }
    return result;
}
int isTimeReached(const Plan *plan) {
    rt_kprintf("Plan %d : beginDate:%d-%d-%d,endDate:%d-%d-%d\n{", plan->id,plan->beginDate.year, plan->beginDate.month, plan->beginDate.day,plan->endDate.year, plan->endDate.month, plan->endDate.day);
    // Check if the current date is within the plan's begin and end date
    if (
        (timerData->date->month>plan->beginDate.month||(timerData->date->month==plan->beginDate.month&&timerData->date->day>=plan->beginDate.day))
        &&
        (timerData->date->month<plan->endDate.month||(timerData->date->month==plan->endDate.month&&timerData->date->day<=plan->endDate.day))
        ) {
        // Check if the current time is equal to the plan's time
        if (timerData->time->h == plan->time.h && timerData->time->m == plan->time.m) {
            return 1; // Time is reached
        }
    }

    return 0; // Time is not reached
}

