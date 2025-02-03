
#include "memory.h"

void gen_eset(void * target, void ** eset, void * addr){
    void * base = addr + ((uint64_t)target & (ESET_OFFSET - 1));

    for( int i = 0 ; i < ESET_SIZE ; i++){
        eset[i] = base + (ESET_OFFSET+128)* (i + 1); 
    }
    memory_fence();
}
