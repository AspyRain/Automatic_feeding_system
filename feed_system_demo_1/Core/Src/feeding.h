// feeding.h
#ifndef FEEDING_H
#define FEEDING_H

#include <rtthread.h>

extern rt_thread_t feeding_thread_1;
extern rt_thread_t feeding_thread_2;
extern rt_thread_t feeding_thread_3;

void feeding_init(void);
void toggle_feed(int feedNum);
void feeding(void *parameter);

#endif /* FEEDING_H */
