/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

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

void systickInit(uint16_t frequency){
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);
	(void) SysTick_Config(RCC_Clocks.HCLK_Frequency / frequency);
}

static volatile uint32_t ticks;

void SysTick_Handler(void) {
  ticks++;
}

// return the system clock as milliseconds

inline uint32_t millis(void) {
  return ticks;
}
void delay_ms(uint32_t t) {
  uint32_t elapsed;
  uint32_t start = millis();
  do {
    elapsed = millis() - start;
  } while (elapsed < t) ;
}

void bitSet()
{


}

struct node{
	int data;
	int head;
	int last;
	struct node *next;
};


int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	systickInit(1000);
	RCC->APB2ENR |= (1<<4);
	GPIOC->CRH |= 0x300000;

	//GPIOC->CRH |= ((1<<20)|(1<<21));
	//GPIOC->CRH &= ~((1<<22)|(1<<23));

  // Infinite loop
  while (1)
    {
       // Add your code here.
	  GPIOC->BSRR |= (1<<13);
	  delay_ms(1000);
	  GPIOC->BSRR |= (1<<29);
	  delay_ms(1000);
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
