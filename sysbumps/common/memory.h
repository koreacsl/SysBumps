#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "config.h"
#include "timing.h"

#define memory_access(x) __asm__ volatile("LDR x10, [%[addr]]" :: [addr] "r" (x) : "x10", "memory")
#define memory_prefetch(x) __asm__ volatile("PRFM PSTL1STRM,[%[addr]]" :: [addr] "r" (x) : "x10", "memory")
#define memory_fence() __asm__ volatile("DMB SY\nISB SY" ::: "memory")
#define nop() __asm__ volatile("MOV x10, x10" ::: "x10", "memory")

static inline __attribute__((always_inline)) uint64_t probe_access(void* address){
    register uint64_t start, end;
    memory_fence();
    timer_read(start);
    memory_fence();
    memory_access(address);
    memory_fence();
    timer_read(end);
    memory_fence();
    return end - start;
}

static inline __attribute__((always_inline)) uint64_t probe_prefetch(void* address){
    register uint64_t start, end;
    memory_fence();
    timer_read(start);
    memory_fence();
    memory_prefetch(address);
    memory_fence();
    timer_read(end);
    memory_fence();
    return end - start;
}


static inline __attribute__((always_inline)) void prime(void ** eset){
    for(int i = 0 ; i < ESET_SIZE ; i++ ){
        memory_fence();
        memory_access(eset[i]);
        memory_fence();
    }
}


static inline __attribute__((always_inline)) uint64_t probe(void ** eset){
    uint64_t time = 0;
    for(int i = 0 ;i < ESET_SIZE ; i++){
        memory_fence();
        time += probe_prefetch(eset[i]);
        memory_fence();
    }
    return time;
}

void gen_eset(void * target, void ** eset, void * addr);

#endif
