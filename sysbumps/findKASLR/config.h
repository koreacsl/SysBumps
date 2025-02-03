#ifndef CONFIG_H
#define CONFIG_H

#define __DEBUG
#define __TIME


#define RESULT_FILE "../util/res"

#define KER_START 0xfffffe000f004000
#define KER_SIZE 0x67E8000
#define KER_END   ((0xfffffe002f000000 + KER_SIZE + 0x4000))
#define DATA_END_OFFSET 0x52D8000
#define CNT 1

#define ALIGN_SIZE 0x4000
#define NUM_SLOT ((KER_END - KER_START)/ALIGN_SIZE)
#define ITERATION 10
#define ESET_OFFSET 0x400000
#define ESET_SIZE 12

#endif

