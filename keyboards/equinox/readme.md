# Equinonx BLE Keyboard Firmware
* [Introduction](#introduction)
  + [Keymap updates](#keymap-updates)
  + [Visual Studio Code support](#visual-studio-code-support)
* [Setting up build environment](#setting-up-build-environment)
  + [Dependencies](#dependencies)
    - [Arm Embedded Toolchain](#arm-embedded-toolchain)
    - [Nordic SDK](#nordic-sdk)
  + [Path configuration](#path-configuration)
    - [Nordic SDK makefile.posix](#nordic-sdk-makefileposix)
    - [Environment script `setevn.sh`](#environment-script--setevnsh-)
  + [Clone Bireme repository](#clone-bireme-repository)
* [Building firmware](#building-firmware)
  + [Command line build](#command-line-build)
  + [Visual Studio Code build](#visual-studio-code-build)
* [Updating firmware](#updating-firmware)
  + [Enter bootloader](#enter-bootloader)
  + [Update firmware](#update-firmware)
* [Using the debug interface](#using-the-debug-interface)
  + [Flashing via debug interface](#flashing-via-debug-interface)
    - [Flashing firmware with OpenOCD](#flashing-firmware-with-openocd)
    - [Flashing firmware with Nordic `nrfjprog`](#flashing-firmware-with-nordic--nrfjprog-)
  + [Debugging support](#debugging-support)
    - [Launch configurations](#launch-configurations)
    - [Starting the debugger](#starting-the-debugger)
  + [OpenOCD installation and configuration](#openocd-installation-and-configuration)
    - [Build and Installation](#build-and-installation)
    - [udev device rules configuration](#udev-device-rules-configuration)
* [Optional tools and software](#optional-tools-and-software)
  + [ST-Link](#st-link)
  + [J-Link](#j-link)
  + [Nordic command line tools](#nordic-command-line-tools)
---
> Note: All instructions provided here assume a linux local machine.

## Introduction
The firmware detailed here is for the Equinox wireless split keyboard system. This system consists of two keyboard units. Each unit is assigned one of two roles: master or slave. The `slave` searches for a `master`. Once connected, the `slave` scans for local key events and sends them to the `master`. The `master` connects the host pc and searches for slaves. The `master` scans its local matrix and combines any local key events with those received from the slave and transmits them to the host pc.

### Keymap updates
The keymap is stored on the `master`, therefore, when updating the keymap only the firmware for the `master` needs to be rebuilt and updated.

To create a new keymap:
1. Create a new folder under `equinox/keymaps` using the name for your keymap
1. Copy the `keymaps.c` file from `equinox/keymaps/default` to your new folder.
1. Edit `keymaps.c` to change the keymap.

To update the `master` with a new keymap:
1. Build the firmware. See [Building firmware](#building-firmware) for information on how to build the `master` using your new keymap.
1. Update the `master` firmware. See [Updating firmware](#updating-firmware) for information on how to update the firmware for the `master`.

### Visual Studio Code support
Visual Studio Code workspace, task and debugging configuration is included in this repository for the Equinox source code. 
> VS Code should be launched by invoking the `vscode.sh` script from the equinox keyboard directory. This script sets up the environment variables using the `setenv.sh` script and starts VS Code in that environment. 

## Setting up build environment

### Dependencies

* ARM Embedded Toolchain
* Nordic nRF5 SDK 15.0.0

#### Arm Embedded Toolchain
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Extract to the local machine: (suggested location: /opt)
Note: This code was developed and tested using the 8-2019-q3-update version

####  Nordic SDK 
https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/Download#infotabs

Download SDK version 15.0.0
Extract to the local machine: ( suggested location: ~/nrf-sdk)

### Path configuration
A few files need to be updated to change configuration paths to coincide with the locations on the machine.

#### Nordic SDK makefile.posix
This file is located in each of the Nordic SDK directories at 
```
<nrf-sdk-path>/components/toolchain/gcc/Makefile.posix
```
Edit this file and change the `GNU_INSTALL_ROOT` define to match the directory where you extracted the ARM Embedded toolchain. For example

```
GNU_INSTALL_ROOT := opt/gcc-arm-none-eabi-8-2019-q3-update
```

#### Environment script `setevn.sh`
The `setenv.sh` script creates the environment variables needed by the Equinox build. The script is located in the equinox keyboard directory. Edit this file and update the following variables to the appropriate paths on the local machine.

 Variable|Usage
--------------|-------------------------------------------
`GCC_ARM`|path to the ARM embedded toolchain
`NRFSDK15_ROOT`|path to Nordic nRF5 SDK version 15.0.0 
`NRF_TOOLS`|path to Nordic command line tools

### Clone Equinox branch of QMK repository
Clone the equinox branch of QMK source repository to the local machine
```
git clone --single-branch --branch equinox_ble https://github.com/QuadDepo/qmk_firmware
```

## Building firmware
The firmware can be built from the command line or from within Visual Studio Code.

### Command line build
From the qmk root directory:

```
make equinox/<role>:<keymap>:uf2
```
where `<role>` is either `master` or `slave` and `<keymap>` is the keymap folder. For example to build the firmware for the `master` using the `default` keymap:

```
make equinox/master:default:uf2
```

### Visual Studio Code build
From the keyboards/equinox directory within the qmk repository folder, lauch VSCode using the `vscode.sh` script:
```
./vscode.sh
```
To build firmware for each role, run the `build` task. There are two ways to do this in Visual Studio Code
1. Press `Ctrl+P` and type `task build` followed by `Enter`
1. Select `Run Task` from the `Terminal` menu and the select `build`

A drop down menu with allow you to select the role you wish to build and followed by the keymap.

Note: If you add a keymap folder, you will have to add the folder name to the `tasks.json` file for vscode. Add it to the `options` for the keymap input to the build task.

## Updating firmware
Equinox uses a USB mass storage bootloader to update firmware. The firware needs to be packaged using the UF2 format. The UF2 file is generated by the build if the `:uf2` tag is added to the build command. Note: A python script `qmk_firmware/util/ufconv.py` is used to convert the .hex file generated buy the build into a UF2 file.

### Enter bootloader
Connect the keyboard to the pc with a usb cable and force the keyboard to enter the bootloader. The bootloader can be entered two ways:
1. Press the physical reset button on the pcb twice rapidly.
2. Pressing a key/combo bound to the `ENT_DFU` keycode.

Once the bootloader starts, the device will appear as a mass storage device to the pc with the volume label `EQNXBOOT`

### Update firmware
Copy the uf2 file generated by the build to the drive. The firmware will update and keyboard will reset and run the new firmware.

## Using the debug interface
The nRF modules can be programmed and debugged using the ARM SWD (Single wire debugging) interface. This requires an external hardware programmer/debugger. Two commonly used programmers are:
1. J-Link from Segger
1. ST-Link from ST Microelectronics
Note: There are a number of ST-Link clones that may work, but I have not tested any.

### Flashing via debug interface
The firmware can be flashed via the swd debug interface by two methods:
1. Use the OpenOCD server with either J-Link or ST-Link programmer.
1. Use the Nordic `nrfjprog` tool with a J-link programmer. The Nordic tools do not support ST-Link.

#### Flashing firmware with OpenOCD

OpenOCD runs a server that exposes a telnet port that can be used for commands.

Start the OpenOCD server by specifying the configuration file with the '-f' flag.
```
openocd -f <config-file>
```
where `<config-file>` is:
* `nrf51-stlink.cfg` for receiver 
* `nrf52-stlink.cfg` for keyboard

Once the OpenOCD server is running, you can send commands via a telnet session:
```
telnet localhost 4444
```
To program the device, issue the following commands via the telnet session:
```
> reset halt
> program <firmware-file> verify
> reset
```
where `<firmware-file>` is the full path to the firmware .hex file. 

#### Flashing firmware with Nordic `nrfjprog`

You will first need to install the [Nordic command line tools](#nordic-command-line-tools)
From the command line:
```
nrfjprog -f <device-family> --eraseall
nrfjprog -f <device-family> --program <firmware-file> --sectorerase
nrfjprog -f <device-family> --reset
```
where `<device-family>` is:
* `nrf51` for receiver 
* `nrf52` for keyboard

where `<firmware-file>` is the firmware .hex file. 

### Debugging support
Debugging support is provided using gdb via the Cortex-Debug extension. https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug

#### Launch configurations
The `launch.json` file in the `.vscode` folder contains configurations for both J-Link and ST-Link debuggers.  The J-Link configuration uses the Segger provided GDB server, while the ST-Link configuration used the OpenOCD GDB server.

#### Starting the debugger
To start debugging, 
1. Select the appropriate launch configuration from `Run` side bar.
1. Select `Start Debugging` from the `Run` menu or press `F5`. The task will prompt for a role, keymap and in the case of the J-Link debugger, the serial number of the debugger.
Note: You can add the serial number of your debugger to the input options in the launch task for convenience.

### OpenOCD installation and configuration
OpenOCD supports flashing/programming and debugging of both the receiver and keyboard using either J-Link or ST-Link. The current release version of OpenOCD (0.10.0) does not include support for the nRF52840 device used by the keyboard halves, therefore a version using the latest updates must be built/installed. 

#### Build and Installation

There are a few dependencies needed to ensure that OpenOCD can function properly. Install these packages

```
sudo apt-get install make libtool pkg-config autoconf automake texinfo libusb-1.0-0-dev
```

Clone the OpenOCD source code repository from the Github mirror
```
git clone --recursive https://github.com/ntfreak/openocd
```

From the OpenOCD repository root directory, execute the build
```    
./bootstrap
./configure
make
```

Install the build version of OpenOCD. (will be installed to `/usr/local/bin`)
```
sudo make install
```


#### udev device rules configuration
The latest udev device rules for OpenOCD compatible devices need to be installed for OpenOCD to access the device. 
The rules file can be found in the Bireme root directory and can be installed as follows:
Copy the rules file:
```
sudo cp utils/60-openocd.rules /etc/udev/rule.d/
```
Add the current user to the plugdev group so that OpenOCD does not have to run as root. Note: The rules file gives access the OpenOCD compatible device to the 'plugdev' group.
```
sudo useradd -G plugdev $(whoami)
```
Reload the rules:
```
sudo udevadm control --reload-rules
```
Restart udev
```
sudo udevadm trigger
```

## Optional tools and software

### ST-Link
ST-Link utilities to query information on ST-Link devices can be found at:
https://github.com/texane/stlink
Note: For Debian based linux distros such as Ubuntu you will need to build the utilities from source.

### J-Link 
The latest J-Link software is recommended and can be downloaded from:
https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack

### Nordic command line tools
The Nordic command line tools can be used to program the devices if you are using J-Link. 
The tools can be download from:
https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools/Download#infotabs

Extract to the local machine: (suggested location: /opt)
Note: Nordic has a Debian install package but it does not seem to update paths correctly.
