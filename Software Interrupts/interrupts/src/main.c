#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include <stm32f10x.h>
// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


void initLED(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	uint32_t bitmask = GPIO_CRH_MODE13 | GPIO_CRH_CNF13 ;
	uint32_t tmpReg = GPIOC ->CRH;
	tmpReg &= ~(bitmask);
	tmpReg |= (GPIO_CRH_MODE13_1);
	GPIOC ->CRH = tmpReg;
}

void TIM2_IRQHandler(){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~(TIM_SR_UIF);
		GPIOC->ODR ^= GPIO_ODR_ODR13;
	}
}

void buffer(){

}



int
main(int argc, char* argv[])
{
	initLED();
	RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST);
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	TIM2->PSC |= 36000;
	TIM2->ARR = 2000;
	TIM2->DIER = TIM_DIER_UIE;
	TIM2->CR1 = TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM2_IRQn);


  while (1)
    {
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
