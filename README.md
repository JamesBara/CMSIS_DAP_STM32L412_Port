# Port of CMSIS-DAP for STM32L412.

CMSIS-DAP has been stripped down to only 2 pin Serial Wire Debugging and the nReset pin.
The bare minimum Microsoft OS 2.0 descriptor has been added to provide PnP capabilities on Windows 8.1 and above.

The project enumerates properly on Windows 11. It should work with OpenOCD and arm-none-eabi-gdb.

This project depends on [STM32L4xx_USB_Device](https://github.com/JamesBara/STM32L4xx_USB_Device), [STM32L4xx](https://github.com/JamesBara/STM32L4xx), [CMSIS-DAP](https://github.com/ARM-software/CMSIS-DAP) and [CMSIS6](https://github.com/ARM-software/CMSIS_6).

Hardware Pinout:
```
PA0 nRESET 
PA1 SWCLK
PA2 SWDIO
```

File tree:
```
STM32L412_CMSIS_DAP
├───CMSIS-DAP
├───STM32L4xx_USB_Device
│      └───STM32L4xx
│            └───CMSIS_6
├───inc
│    ├───bsp.h
│    ├───DAP_config.h
│    └───ms_os_20_request.h
├───src
│    ├───bsp.c
│    └───main.c
├───STM32L412.ld
├───CMakeLists.txt
├───CMakePresets.json
├───gcc-arm-none-eabi.cmake
├───LICENSE.txt
└───README.md
```
