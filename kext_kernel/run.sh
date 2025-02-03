#!/bin/bash

if [ "$(id -u)" != "0" ]; then
    echo "This script must be run with root privileges!!"
    exit 1
fi

kmutil load -p kernel_slide.kext
log show --last 20s | grep "SysBumps_log"
kmutil unload -p kernel_slide.kext
