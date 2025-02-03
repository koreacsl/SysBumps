#include "timing.h"

void* counting(void* ctx){
    __asm__ volatile(
        "LDR x10, [%[ctx]]\n"       
        "loop:\n"                      
            "ADD x10, x10, #1\n"      
            "STR x10, [%[ctx]]\n"   
        "B loop\n"                      
        : 
        : [ctx] "r" (ctx) 
        : "x10", "memory" 
    );
}

void start_timer(){
    pthread_create(&counting_thread, NULL, counting, &timestamp);
}

void stop_timer(){
    pthread_cancel(counting_thread);
}
