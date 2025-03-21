# Jetson Orin Nano Build
Setup documentation and backup of the Jetson Linux image backup used on the Orin Nano for the Roaming Demonstrator.

## Orin Nano Specification
- Orin Nano developer kit (8GB Module)
- SD Card or SSD **installed** (min. 64GB)
- JetPack 5.1.3 with Jetson Linux 35.5.0

*Note: At the time of writing essential kernel modules needed for employing the Nano Module als control unit for the RC truck (joydev, uhid, uinput) and ath9k driver support were only present up to JetPack 5.1.3.*

---

## Host System Requirements 
A system running **Ubuntu 20.04.** with internet connection.

*Note: Flashing older JetPack versions with newer ubuntu versions is not supported by Nvidia!*

---

## Orin Nano Setup
The [Setup from Scratch](###setup-from-scratch) section describes how to setup the orin nano for usage in the Roaming Demonstrator from scratch.  
Since the documentation provided by Nvidia is incomplete a fully configured Image to be used with the SDK manager is stored as backup on the fileserver, see [Setup from Backup](###setup-from-backup)

### Setup from Scratch
This guide is based on the official documentation provided by Nvidia, see [Jetson Linux Developer Guide](https://docs.nvidia.com/jetson/archives/r35.5.0/DeveloperGuide/index.html).

#### Nvidia SDK Manager
1. Download and install the [Nvidia SDK manager](https://developer.nvidia.com/sdk-manager).
2. In the SDK Manager application choose 'JetPack 5.1.3' for the SDK version and click continue to step 2.
3. Choose 'Download now. Install later' (bottom right) and click continue to step 3.
4. Wait for the download to complete.

*Note: by default the SDK manager creates the necessary directory structure in the home directory*


#### Bootlin Toolchain
1. Download the [bootlin toolchain binaries](https://developer.nvidia.com/embedded/jetson-linux-r3550)
   *Note: the toolchain for the correct jetson linux version (here: R35.5.0) are needed!*
2. Extract the toolchain
    ```
    mkdir $HOME/l4t-gcc
    cd $HOME/l4t-gcc
    tar xf <toolchain_archive>
    ```
For more information refer to the official documentation, see [Jetson Linux Toolchain](https://docs.nvidia.com/jetson/archives/r35.5.0/DeveloperGuide/AT/JetsonLinuxToolchain.html#at-jetsonlinuxtoolchain)

#### Download Kernel Sources
1. Change to the directory created by the SDK Manager
    ```cd $HOME/nvidia/nvidia_sdk/JetPack_5.1.3_Linux_JETSON_ORIN_NANO_TARGETS/Linux_for_Tegra/```
2. Get the kernel sources by running `source_sync.sh`:
    ```./source_sync.sh -t jetson_35.5```
    The `-t` options specificies the requested jetson linux version, in this case R35.5.0
    Once the kernel sources are downloaded they can be found in the `../sources` directory.

#### Ath9k Kernel Patch
1. Get the patch file `regd_patch.patch` from `.//driver/wifi_drivers/ath9k`.
2. Move the patch file to `$HOME/nvidia/nvidia_sdk/JetPack_5.1.3_Linux_JETSON_ORIN_NANO_TARGETS/Linux_for_Tegra/sources/kernel/kerne-5.10/`
3. Apply the patch:
    ```
    patch -p1 < regd_patch.patch
    ```
4. Enable userspace regulatory domain configuration
   Add `CONFIG_ ATH_USER_REGD=y` to `defconfig` in `.../Linux_for_Tegra/sources/kernel/kerne-5.10/arch/arm64/configs`

#### Building the Kernel
1. Install the jetson linux build utilities:
   ```
   sudo apt install build-essential bc
   ```
2. Export the cross compile environment variables:
   ```
   export CROSS_COMPILE_AARCH64=$HOME/l4t-gcc/bin/aarch64-buildroot-linux-gnu-
   export CROSS_COMPILE_AARCH64_PATH=$HOME/l4t-gcc/
   ``` 
3. Create the target directory for the compiled kernel in the `../sources` directory.
   ```
   mkdir kernel_out
   ```
4. Since the build scripts are not downloaded by the sync script, copy them to `../sources`
   ```
   cp -r $HOME/RoamingDemonstrator/JetsonBuild/scripts/* <nano target directory>/Linux_for_Tegra/sources/
   ```
5. Build the kernel
    ```
    ./nvbuild.sh -o $PWD/kernel_out
    ```
6. Replace Image File in `.../Linux_for_Tegra/kernel`
   ```
   cp kernel_out/arch/arm64/boot/Image <nano target directory>/Linux_for_Tegra/kernel/
   ```
7. Replace ath9k module files in rootfs with the patched versions
   ```
   sudo cp kernel_out/drivers/net/wireless/ath/*.ko <nano target directory>/Linux_for_Tegra/rootfs/lib/modules/kernel/drivers/net/wireless/ath/
   sudo cp kernel_out/drivers/net/wireless/ath/aht9k/*.ko <nano target directory>/Linux_for_Tegra/rootfs/lib/modules/kernel/drivers/net/wireless/ath/ath9k/
   ```

*Note: by default <nano target directory> should be `$HOME/nvidia/nvidia_sdk/JetPack_5.1.3_Linux_JETSON_ORIN_NANO_TARGETS/`*

#### Flashing the Orin Nano

1. Put the board into recovery mode. This is achieved by connecting pins 9 (FC_REC) and 10 (GND) with a jumper (cf. [Orin Nano Pinout](https://jetsonhacks.com/nvidia-jetson-orin-nano-gpio-header-pinout/)).
2. Connect the board via USB with the Ubuntu host.
3. Connect the power adapter. * Note: in recovery mode the fan does not spin!*
4. Launch the SDK manager application. The application should now automatically detect the board.
5. Ensure that the SDK Version is set to JetPack 5.1.3.
6. Start the flashing process by clicking continue.

---

###  Setup from Backup

#### Getting the preconfigured Image
1. Download `jetson_build.tar.gz` from the file server. The tar archive is located at `fileserver/projects/RoamingDemonstrator/JetsonBuild/`.
2. Decompress the tar archive on the ubuntu host system, preferably at the home directory.
   ```
   tar -xvf jetson_build.tar.gz
   ```
#### Nvidia SDK Manager
1. Download and install the [Nvidia SDK manager](https://developer.nvidia.com/sdk-manager).
2. Add the path of preconfigured Image as source path in the SDK Manager. 
   To accomplish this, first launch the SDK Manager application, choose JetPack_5.1.3 as SDK Version and click continue. 
   In the following window change the `Download folder` and `Target HW image folder` to `<extract location>/nvidia/downloads` and `<extract location>/nvidia/nvidia_sdk` respectively. 
3. The SDK Manager should now detect the preconfigured image, indicated by the status of listed components. The status should either be `downloaded` or `OS image ready`.

#### Flashing the pre-configured image
1. Put the board into recovery mode. This is achieved by connecting pins 9 (FC_REC) and 10 (GND) with a jumper.
2. Connect the board via USB with the Ubuntu host.
3. Connect the power adapter. * Note: in recovery mode the fan does not spin!*
4. Launch the SDK manager application. The application should now automatically detect the board.
5. Ensure that the SDK Version is set to JetPack 5.1.3.
6. Start the flashing process by clicking continue.