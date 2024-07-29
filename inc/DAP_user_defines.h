#ifndef DAP_USER_DEFINES_H
#define DAP_USER_DEFINES_H

#include "stm32l4xx.h"

/************************************************
 * Peripheral clock selection for each pin.
 * User configurable.
 ***********************************************/
#define RCC_nRESET RCC_AHB2ENR_GPIOAEN
#define RCC_SWDIO_TMS RCC_AHB2ENR_GPIOAEN
#define RCC_SWCLK_TCK RCC_AHB2ENR_GPIOAEN
#define RCC_TDI RCC_AHB2ENR_GPIOAEN
#define RCC_TDO RCC_AHB2ENR_GPIOAEN
#define RCC_nTRST RCC_AHB2ENR_GPIOAEN
#define RCC_LED_CONNECTED RCC_AHB2ENR_GPIOAEN
#define RCC_LED_RUNNING RCC_AHB2ENR_GPIOAEN

/************************************************
 * GPIO port selection for each pin.
 * User configurable.
 ***********************************************/
#define GPIO_nRESET GPIOA
#define GPIO_SWDIO_TMS GPIOA
#define GPIO_SWCLK_TCK GPIOA
#define GPIO_TDI GPIOA
#define GPIO_TDO GPIOA
#define GPIO_nTRST GPIOA
#define GPIO_LED_CONNECTED GPIOA
#define GPIO_LED_RUNNING GPIOA

/************************************************
 * GPIO pin number selection for each pin.
 * User configurable.
 ***********************************************/
#define PIN_nRESET 0
#define PIN_SWCLK_TCK 1
#define PIN_SWDIO_TMS 2
#define PIN_TDI 3
#define PIN_TDO 4
#define PIN_nTRST 5
#define PIN_LED_CONNECTED 6
#define PIN_LED_RUNNING 7




#endif /*DAP_USER_DEFINES_H*/