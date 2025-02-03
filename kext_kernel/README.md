# Extracting Kernel Base Address via KEXT

`kernel_slide.kext` is a kernel extension for macOS used to extract the kernel base address. 
This file helps in identifying the kernel base address.

## Configuration 

### 1. Install Xcode 
If Xcode is not installed, download and install it from the [Apple Developer site](https://developer.apple.com/xcode/).

### 2. Disabling SIP (System Integrity Protection)
1. Boot into macOS [Recovery mode](https://support.apple.com/en-us/102518).
2. Select your account and click **Next**.
2. In the menu bar at the top, select **Utilities > Terminal**.
3. Type the following command:
   ```bash
   csrutil disable
   csrutil authenticated-root disable
   ```

### 3. Changing startup disk security settings 
1. Boot into macOS [Recovery mode](https://support.apple.com/en-us/102518).
2. In the menu bar at the top, select Utilities > Startup Security Utility.
3. Click **Security Policy...**.
4. Check **Reduced Security**.
5. Check the **Allow user management of kernel extensions from identified developers** box.
6. Click **OK**. (NOTE, Run it again from step 3 to check once more whether it is set up properly.)
7. Restart your Mac.


## Building the kext
1. To build the kext, run the following command:
```bash
sudo ./build.sh
```
2. If pop-up appears, click **Open System Setting**. (If not, open **System Settings** from the Apple menu  and click **Privacy & Security** in the sidebar.
3. Scroll down to find the **Security** section.
4. Check **App Store and identified developers**.
5. Click **Enable System Extensions** (or **Allow** or **Details**).
6. When the first pop-up appears, enter your password and click **Modify Setting**.
7. When the second pop-up appears, enter your password and click **OK**.
8. Click **Restart**.

**NOTE**. If you encounter the error '**Domain=KMErrorDomain Code=29 "Authenticating extension failed"**', please perform **Disabling SIP** and **Changing startup disk security settings** again.


## Using the kext
Load the kext using the provided script:
```bash
sudo ./run.sh
```

This script will load the `kernel_slide.kext` and extract the kernel base address, which will be displayed in the terminal. as follows:
```bash
2024-07-xx xx:xx:xx.596346+0900 0xc1e      Default     0x0                  0      0    kernel: (kernel_slide) [SysBumps] base = 0xfffffe002724c000
```

