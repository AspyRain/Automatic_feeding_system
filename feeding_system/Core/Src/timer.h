#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdlib.h>
#include "data_structure.h"

typedef struct
{
    Time *time;
    Date *date;
    PlanList *planList;
} Timer;

extern Timer *timerData; 
extern PlanList* planList;
int timerInit(Date *now_date, Time *now_time, PlanList *planList);
void setTimerAll(Date *now_date, Time *now_time, PlanList *planList);
void setTimerDate(Date *now_date);
void setTimerTime(Time *now_time);
void setTimerPlan(PlanList *planLists);
Date *getTimerDate();
Time *getTimerTime();
int timerRun();
int isTimeReached( Plan *plan);
void insertTimerPlan(int device, Time time, int duration, Date beginDate, Date endDate);
#endif /* TIMER_H */
