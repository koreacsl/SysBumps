# <img src="https://github.com/user-attachments/assets/02f605cf-16bb-4626-83c9-57846ab2a5c9" width="450">
# CVE-2024-54531

## Overview
SysBumps is an exploit designed to break Kernel Address Space Layout Randomization (KASLR) on macOS for Apple Silicon. 

This attack has been successfully tested on the following devices:
- **Apple Silicon**: M1, M1 Pro, M2, M2 Pro, M2 Max, M3, M3 Pro  
- **macOS Versions**: 13.1 - 15.1 (Earlier versions may also be vulnerable but are untested)


## More Details
- **CCS'24**: [https://dl.acm.org/doi/abs/10.1145/3658644.3690189](https://dl.acm.org/doi/abs/10.1145/3658644.3690189)
  
- **BlackHat EU'24**: [https://www.blackhat.com/eu-24/briefings/schedule/#sysbumps-exploiting-speculative-execution-in-system-calls-for-breaking-kaslr-in-macos-for-apple-silicon-42192](https://www.blackhat.com/eu-24/briefings/schedule/#sysbumps-exploiting-speculative-execution-in-system-calls-for-breaking-kaslr-in-macos-for-apple-silicon-42192)


## Prerequisites
- A macOS device with Apple Silicon (M1, M1 Pro, M2, M2 Pro, M2 Max, M3, M3 Pro)
- macOS version 13.1 - 15.1


## Running the Attack
To execute the attack:
1. Set `SYSBUMPS_PATH` in `run.sh` to the path of the `sysbumps` directory.
2. Execute `run.sh` from your terminal.
   ```bash
   ./run.sh
   ```

## Visualizing Attack Outcomes
To visualize the results of the attack:
- Run `graph.py` from the `util` directory.
   ```bash
   python3 graph.py
   ``` 
   
## Verifying Kernel Addresses
To confirm the actual kernel addresses, you have two options:

1. **Using Kernel Extension (`kernel_slide.kext`)**  
   - Build and run `kernel_slide.kext` from the `kext_kernel` folder.  
   - For detailed instructions, refer to the README in that folder.

2. **Using dtrace (alternative method)**  
   - You can generate a kernel panic and examine the debugger message that appears after rebooting:
     ```bash
     sudo dtrace -w -n "BEGIN{ panic();}"
     ```

> **NOTE:**  
> - To execute `kernel_slide.kext`, you must disable **System Integrity Protection (SIP)** and set your startup disk security to **Reduced Security Mode**.  
> - Similarly, disabling SIP is required to execute dtrace commands.


## Disclaimer
This project is for educational and research purposes only. Use of this software for attacking targets without prior mutual consent is illegal. The developer is not responsible for any damage caused by the use of this exploit.


