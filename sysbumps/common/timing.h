#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdint.h>

uint64_t timestamp;
pthread_t counting_thread;

#define timer_read(x) x = timestamp
//#define timer_read(x) __asm__ volatile("mrs %[time], S3_2_c15_c0_0" : [time]"=r"(x));
void start_timer();
void stop_timer();

#endif
