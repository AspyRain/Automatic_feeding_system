#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include <stdlib.h>
#include "data_structure.h"
Time *time=NULL;
Date *date=NULL;
Plan *plans=NULL;
void setAll(Date *now_date,Time *now_time,Plan *now_plan);
void setDate(Date *now_date);
void setTime(Time *now_time);
void setPlan(Plan *now_plans);
Date* getDate();
Time* getTime();
int* timer(int deviceNum);
int isTimeReached(const Plan *plan) ;
#endif /* TIMER_H */