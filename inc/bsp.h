#ifndef BSP_H
#define BSP_H

#include <stdint.h>

/************************************************
 * GPIO macros for STM32L4xx
 ***********************************************/
#define GPIO_MODER_CLEAR(port, pin) ((port)->MODER &= ~(0x3UL << ((pin) << 0x1U)))
#define GPIO_MODER_OUTPUT(port, pin) ((port)->MODER |= (0x1UL << ((pin) << 0x1U)))
#define GPIO_MODER_INPUT(port, pin) GPIO_MODER_CLEAR(port, pin)
#define GPIO_MODER_ANALOG(port, pin) ((port)->MODER |= (0x3UL << ((pin) << 0x1U)))
#define GPIO_OTYPER_CLEAR(port, pin) ((port)->OTYPER &= ~(0x1UL << (pin)))
#define GPIO_OTYPER_OPEN_DRAIN(port, pin) ((port)->OTYPER |= (0x1UL << (pin)))
#define GPIO_OSPEEDR_CLEAR(port, pin) ((port)->OSPEEDR &= ~(0x3UL << ((pin) << 0x1U)))
#define GPIO_OSPEEDR_VERY_HIGH_SPEED(port, pin) ((port)->OSPEEDR |= (0x3UL << ((pin) << 0x1U)))
#define GPIO_PUPDR_CLEAR(port, pin) ((port)->PUPDR &= ~(0x3UL << ((pin) << 0x1U)))
#define GPIO_PUPDR_PU(port, pin) ((port)->PUPDR |= (0x1UL << ((pin) << 0x1U)))
#define GPIO_AFRN_CLEAR(port, pin) (((pin) > 7) ? ((port)->AFRH &= ~(0x7UL << (((pin) >> 3U) << 0x2U))) : ((port)->AFRL &= ~(0x7UL << ((pin) << 0x2U))))

#define GPIO_SET_ODR(port, pin, bit) (port)->BSRR = ((bit) & 0x1U) ? (0x1UL << (pin)) : (0x1UL << ((pin) + 16U)) 
#define GPIO_GET_IDR(port, pin) ((((port)->IDR) & (0x1UL << (pin))) ? (0x1UL) : (0x0UL))  

uint32_t get_tick(void);
void delay(uint32_t timeout);

void set_cpu_max_freq(void);
void set_usbd_clk_src_hsi48(void);

void bsp_init(void);

#endif /*BSP_H*/