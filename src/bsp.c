#include <stdint.h>
#include "assert_stm32l4xx.h"
#include "spinlock_stm32l4xx.h"
#include "stm32l4xx.h"
#include "bsp.h"

static __IO uint32_t tick;
extern uint32_t SystemCoreClock;  /* System Core Clock Frequency */


/**
 * @brief Enable the hsi48 oscillator and set it as 
 * select it as usb clock source.
 * @param  
 */
void set_usbd_clk_src_hsi48(void)
{
	uint32_t s = 0;
	/*Enable HSI48*/
	if (!GET(RCC->CRRCR, RCC_CRRCR_HSI48ON))
	{
		SET(RCC->CRRCR, RCC_CRRCR_HSI48ON);
		s = tick;
		while (!GET(RCC->CRRCR, RCC_CRRCR_HSI48RDY))
		{
			ASSERT((tick - s) < 5000);
		}
	}
	/*Select hsi48 as clock source.*/
	CLEAR(RCC->CCIPR, RCC_CCIPR_CLK48SEL);
}

/**
 * @brief Enable the external oscillator 
 * and set the cpu frequency to 80Mhz.
 * @param  
 */
void set_cpu_max_freq(void)
{
	uint32_t s = 0;
	/*Enable HSE.*/
	if (!GET(RCC->CR, RCC_CR_HSEON))
	{
		SET(RCC->CR, RCC_CR_HSEON);
		s = tick;
		while (!GET(RCC->CR, RCC_CR_HSERDY))
		{
			ASSERT((tick - s) < 5000);
		}
	}
	/*Enable and configure PLL*/
	if (GET(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
	{
		if (GET(RCC->CR, RCC_CR_PLLON))
		{
			CLEAR(RCC->CR, RCC_CR_PLLON);

			s = tick;
			while (GET(RCC->CR, RCC_CR_PLLRDY))
			{
				ASSERT((tick - s) < 5000);
			}
		}
		RCC->PLLCFGR = 0x0UL;
		SET(RCC->PLLCFGR, (RCC_PLLCFGR_PLLSRC_HSE | (20 << RCC_PLLCFGR_PLLN_Pos) | RCC_PLLCFGR_PLLR_DIV_2));
		SET(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);
		SET(RCC->CR, RCC_CR_PLLON);
		s = tick;
		while (!GET(RCC->CR, RCC_CR_PLLRDY))
		{
			ASSERT((tick - s) < 5000);
		}
	}
	/*Set the flash latency.*/
	SET(FLASH->ACR, FLASH_ACR_LATENCY_WAIT_STATE_4);
	s = tick;
	while (GET(FLASH->ACR, FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_WAIT_STATE_4)
	{
		ASSERT((tick - s) < 5000);
	}
	/*Select the pll as the main clock source and use HSI16 as the backup clock source.*/
	SET(RCC->CFGR, (RCC_CFGR_SW_PLL | RCC_CFGR_STOPWUCK));

	s = tick;
	while (GET(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
	{
		ASSERT((tick - s) < 5000);
	}

	SystemCoreClock = 80000000;
}

/**
 * @brief Initalize clocks, gpio and interrupts.
 * @param  
 */
void bsp_init(void)
{
	/*Enable Syscfg clock*/
	SET(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	__spinlock(2);
	/*Enable pwr clock*/
	SET(RCC->APB1ENR1, RCC_APB1ENR1_PWREN);
	__spinlock(2);
	/*Configure the systick*/
	tick = 0;
	SysTick_Config(SystemCoreClock / 1000);
	/*Set the max frequency.*/
	set_cpu_max_freq();
	/*Reconfgure systick.*/
	tick = 0;
	SysTick_Config(SystemCoreClock / 1000);
	/*Enable prefetch.*/
	SET(FLASH->ACR, FLASH_ACR_PRFTEN);
	/*Enable gpioa clock.*/
	SET(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);
	__spinlock(2);
	/*Clear PA11 and PA12.*/
	CLEAR(GPIOA->MODER, (GPIO_MODER_MODE11 | GPIO_MODER_MODE12));
	CLEAR(GPIOA->OTYPER, (GPIO_OTYPER_OT11 | GPIO_OTYPER_OT12));
	CLEAR(GPIOA->OSPEEDR, (GPIO_OSPEEDR_OSPEED11 | GPIO_OSPEEDR_OSPEED12));
	CLEAR(GPIOA->PUPDR, (GPIO_PUPDR_PUPD11 | GPIO_PUPDR_PUPD12));
	CLEAR(GPIOA->AFRH, (GPIO_AFRH_AFSEL11 | GPIO_AFRH_AFSEL12));
	/*Configure PA11 and PA12 to act as DM and DP.*/
	SET(GPIOA->MODER, (GPIO_MODER_MODE11_ALTERNATE_FUNCTION | GPIO_MODER_MODE12_ALTERNATE_FUNCTION));
	SET(GPIOA->OSPEEDR, (GPIO_OSPEEDR_OSPEED11_VERY_HIGH | GPIO_OSPEEDR_OSPEED12_VERY_HIGH));
	SET(GPIOA->AFRH, ((10 << GPIO_AFRH_AFSEL11_Pos) | (10 << GPIO_AFRH_AFSEL12_Pos)));
	/*Enable HSI48 and use it as USB clock.*/
	set_usbd_clk_src_hsi48();
	/*Enable the usb clock.*/
	SET(RCC->APB1ENR1, RCC_APB1ENR1_USBFSEN);
	__spinlock(2);
	/*Turn on the usb power supply.*/
	SET(PWR->CR2, PWR_CR2_USV);
	/*Set usb interrupt priority to the lowest.*/
	NVIC_SetPriority(USB_IRQn, 0xFU);
	/*Enable usb interrupt.*/
	NVIC_EnableIRQ(USB_IRQn);
}

/**
 * @brief Increments the tick variable on each call.
 * @param  
 */
void SysTick_Handler(void)
{
	++tick;
}

/**
 * @brief Get the tick variable value.
 * @param  
 * @return tick value.
 */
uint32_t get_tick(void)
{
	return tick;
}

/**
 * @brief Do nothing for a specified amount of time.
 * @param timeout Amount of time to wait.
 */
void delay(uint32_t timeout)
{
	uint32_t s = tick;
	while (tick - s < timeout);
}
