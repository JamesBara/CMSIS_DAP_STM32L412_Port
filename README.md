# Port of CMSIS-DAP for STM32L412.

This project is a port of CMSIS-DAP for STM32L412 and depends on [STM32L4xx_USB_Device](https://github.com/JamesBara/STM32L4xx_USB_Device), [STM32L4xx](https://github.com/JamesBara/STM32L4xx), [CMSIS-DAP](https://github.com/ARM-software/CMSIS-DAP) and [CMSIS6](https://github.com/ARM-software/CMSIS_6). The bare minimum Microsoft OS 2.0 descriptor has been added to provide PnP capabilities on Windows 8.1 and above, so it should work without need for drivers.

# Tested and working functionality:

2 wire SWDIO (SWCLK and SWDIO) has been tested on Windows 11 with OpenOCD and arm-none-eabi-gdb and it's working properly. 

# Untested functionality:

Hardware reset, LEDs, and JTAG have been added but are untested.

# Functionality that isn't (and won't be) implemented:

SWO over UART (Stream and/or Manchester) and USB to UART. 
The above functionality won't be implemented since it requires either to use the provided drivers from CMSIS and an RTOS, or to heavily modify the provided CMSIS-DAP, in order to fit the current implementation.


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
│    ├───DAP_user_defines.h
│    ├───ms_os_20_request.h
│    ├───usbd_dap.h
│    └───usbd_dap_desc.h
├───src
│    ├───bsp.c
│    ├───main.c
│    ├───usbd_dap.c
│    └───usbd_dap_desc.c
├───STM32L412.ld
├───CMakeLists.txt
├───CMakePresets.json
├───gcc-arm-none-eabi.cmake
├───LICENSE.txt
└───README.md
```


The pinout is configurable and can be modified by changing the definitions in the DAP_user_defines.h file.

Default Hardware Pinout:
```
PA0 nRESET 
PA1 SWCLK_TCK
PA2 SWDIO_TMS
PA3 TDI
PA4 TDO
PA5 nTRST
PA6 LED_CONNECTED
PA7 LED_RUNNING
```