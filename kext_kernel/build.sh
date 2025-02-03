#!/bin/zsh

if [ "$(id -u)" != "0" ]; then
    echo "This script must be run with root privileges!!"
    exit 1
fi


xcodebuild -project kernel_slide.xcodeproj -configuration Debug
cp -r ./build/Debug/kernel_slide.kext ./


chown -R root:wheel kernel_slide.kext
chmod -R 0644 kernel_slide.kext
kmutil load -p kernel_slide.kext
