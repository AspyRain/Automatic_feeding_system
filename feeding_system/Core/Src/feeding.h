// feeding.h
#ifndef FEEDING_H
#define FEEDING_H

#include <rtthread.h>

extern rt_thread_t feeding_thread_1;
extern rt_thread_t feeding_thread_2;
extern rt_thread_t feeding_thread_3;

typedef struct 
{
    int feedNum;
    int duration;
} FeedParameter;


void feeding_init(void);
void toggle_feed(int feedNum);
void feeding(void *parameter);
void runFeeding(int feedNum,int duration) ;
void stopFeeding(int feedNum);
#endif /* FEEDING_H */
