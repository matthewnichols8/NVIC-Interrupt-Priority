/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define IRQNO_TIMER2 28
#define IRQNO_I2C1 31

uint32_t* pNVIC_IPR_Base = (uint32_t*) 0xE000E400;
uint32_t* pNVIC_ISER_Base = (uint32_t*) 0xE000E100;
uint32_t* pNVIC_ISPR_Base = (uint32_t*) 0xE000E200;

void set_priority_for_irq(uint8_t irqNum, uint8_t priorityNum) {
	uint8_t iprx = irqNum / 4;
	uint8_t* ipr = (uint8_t*) (pNVIC_IPR_Base + iprx);

	uint8_t pos = (irqNum % 4) * 8; //Position in iprx

	*ipr &= ~(0xFF << pos); // Clears bit position
	*ipr |= (priorityNum << pos); //Sets bits
}

int main(void)
{
	//1) Set priority for both peripherals

	set_priority_for_irq(IRQNO_TIMER2, 0x80);
	set_priority_for_irq(IRQNO_I2C1, 0x80);

	//2) Set the Interrupt pending bit in NVIC PR
	*pNVIC_ISPR_Base |= (1 << IRQNO_TIMER2); //sets pending bit

	//3) Enable the IRQs in NVIC ISER
	*pNVIC_ISER_Base |= (1 << IRQNO_I2C1);
	*pNVIC_ISER_Base |= (1 << IRQNO_TIMER2);


    /* Loop forever */
	for(;;);
}

/**
 * TIM2 Handler method
 */
void TIM2_IRQHandler() {
	printf("This is the TIM2_IRQHandler!\n");
	*pNVIC_ISPR_Base |= (1 << IRQNO_I2C1); //Trigger interrupt for I2C1
}

/**
 * I2C1 Handler method
 */
void I2C1_EV_IRQHandler() {
	printf("This is the I2C1_EV_IRQHandler!\n");
}
