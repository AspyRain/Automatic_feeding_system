#include "timer.h"
#include <stdlib.h>
#include "rtthread.h"
#include "feeding.h"
#include <string.h>
#include "main.h"
Timer *timerData = NULL;
PlanList *planList =NULL;
int timerInit(Date *now_date, Time *now_time, PlanList *planList)
{

    // 在运行时动态分配内存给 Timer 对象
    timerData = (Timer *)malloc(sizeof(Timer));

    // 检查内存分配是否成功
    if (timerData == NULL)
    {
        // 处理内存分配失败的情况
        return 0;
    }

    setTimerAll(now_date, now_time, planList);
    return 1;
}

void setTimerAll(Date *now_date, Time *now_time, PlanList *planList)
{
    setTimerDate(now_date);
    setTimerTime(now_time);
    setTimerPlan(planList);
}

void setTimerDate(Date *now_date)
{
    // 释放原有的日期空间
    if (timerData->date != NULL)
    {
        // 释放原有的日期空间，假设使用 rt_free 进行释放
        rt_free(timerData->date);
    }

    if (now_date != NULL)
    {
        // 分配新的日期空间
        timerData->date = rt_malloc(sizeof(Date));
        
        // 将新的日期数据复制到分配的空间中
        memcpy(timerData->date, now_date, sizeof(Date));
    }
}

void setTimerTime(Time *now_time)
{
    // 释放原有的时间空间
    if (timerData->time != NULL)
    {
        // 释放原有的时间空间，假设使用 rt_free 进行释放
        rt_free(timerData->time);
    }

    if (now_time != NULL)
    {
        // 分配新的时间空间
        timerData->time = rt_malloc(sizeof(Time));
        
        // 将新的时间数据复制到分配的空间中
        memcpy(timerData->time, now_time, sizeof(Time));
    }
}

void setTimerPlan(PlanList *planList)
{
    // 释放原有的计划空间
    if (timerData->planList != NULL)
    {
        // 释放原有的计划空间，假设使用 rt_free 进行释放
        rt_free(timerData->planList);
    }

    if (planList != NULL)
    {
        // 分配新的计划空间
        timerData->planList = rt_malloc(sizeof(PlanList));

        // 将新的计划数据复制到分配的空间中
        memcpy(timerData->planList, planList, sizeof(PlanList));
    }
}


void insertTimerPlan(int device, Time time, int duration, Date beginDate, Date endDate){
    insertPlan(timerData->planList,device,time,duration,beginDate,endDate);
}
Date *getTimerDate()
{
    return timerData->date;
}

Time *getTimerTime()
{
    return timerData->time;
}

int timerRun()
{
    int result = 0;
    timerData->time->s += 1;
    if (timerData->time->s == 60)
    {
        timerData->time->s = 0;
        timerData->time->m += 1;
    }
    if (timerData->time->m == 60)
    {
        timerData->time->m = 0;
        timerData->time->h += 1;
    }
    if (timerData->time->h == 24)
    {
        result = 1;
    }
    rt_kprintf("现在的时间是:%d时%d分%d秒\n",timerData->time->h,timerData->time->m,timerData->time->s);
    Plan *plans = planList->plans;
    int planNum = timerData->planList->length;

    // 判断是否有设备到喂食时间
    if (plans != NULL && planNum > 0)
    {
        for (int i = 0; i < planNum; i++)
        {
            if (isTimeReached(&plans[i]) == 1)
            {
                runFeeding(plans[i].device, plans[i].duration);
            }
        }
    }

    return result;
}

int isTimeReached(Plan *plan)
{
    if (plan->isRuning==-1){
// Check if the current date is within the plan's begin and end date
    if (
        (timerData->date->month > plan->beginDate.month || (timerData->date->month == plan->beginDate.month && timerData->date->day >= plan->beginDate.day)) &&
        (timerData->date->month < plan->endDate.month || (timerData->date->month == plan->endDate.month && timerData->date->day <= plan->endDate.day)))
    {
        // Check if the current time is equal to the plan's time
        if (timerData->time->h == plan->time.h && timerData->time->m == plan->time.m)
        {
            plan->isRuning=0;
            return 1; // Time is reached
        }
    }
    }else {
        plan->isRuning=plan->isRuning+1;
        if (plan->isRuning>=plan->duration)plan->isRuning=0;
    }
    

    return 0; // Time is not reached
}
