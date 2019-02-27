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
#include <stm32f10x.h>

//You have to initialise a structure for enabling the GPIO
/*
 * struct{
 * uint16_t GPIO_Speed;
 * uint16_t GPIO_Mode;
 * uint16_t GPIO_Pin;
 * } GPIO_InitTypeDef;
 *
 */
//Then pass the the structure in function GPIO_Init(port, structure)

int GPIO_Init_local(){
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN, ENABLE);
	GPIO_InitTypeDef GPIO_initstructurePortC;
	GPIO_initstructurePortC.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initstructurePortC.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initstructurePortC.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_initstructurePortC);
	return 0;
}

int I2C_Init_local(int clockSpeed, uint16_t ownAdd){
	//I2C strcuture for defining it's parameters.
	I2C_InitTypeDef i2c_init;

	//Enabling I2C1
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C1EN, ENABLE);

	//Resetting the register.
	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

	i2c_init.I2C_Mode = I2C_Mode_I2C;
	i2c_init.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init.I2C_OwnAddress1 = ownAdd;
	i2c_init.I2C_Ack = I2C_Ack_Enable;
	i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init.I2C_ClockSpeed = clockSpeed; //50KHz

	I2C_Init(I2C1, &i2c_init);
	I2C_Cmd(I2C1, ENABLE);

	/* Defining and enabling PORT B. I2C functions are available on
	 * PB6 - SCL
	 * PB7 - SDA
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN, ENABLE);
	GPIO_InitTypeDef GPIO_initstructurePortB;
	GPIO_initstructurePortB.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_initstructurePortB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_initstructurePortB.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_initstructurePortB);

	return 0;
}

void i2c_start_condition(){
	//wait until I2C isn't busy anymore.
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){;}

	//Generate Start condition.
	I2C_GenerateSTART(I2C1, ENABLE);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

void i2c_stop_condition(){
	//Generates STOP condition
	I2C_GenerateSTOP(I2C1, ENABLE);
	//Waits for it to stop.
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
}

void i2c_setAddressDirection(uint8_t ownAdd, uint8_t direction){
	I2C_Send7bitAddress(I2C1, ownAdd, direction);

	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

void i2c_sendData(uint8_t byte){
	I2C_SendData(I2C1, byte);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t i2c_recDataACK(){
	I2C_AcknowledgeConfig(I2C1, ENABLE);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

	return I2C_ReceiveData(I2C1);
}

uint8_t i2c_recDataNACK(){
	I2C_AcknowledgeConfig(I2C1, DISABLE);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

	return I2C_ReceiveData(I2C1);
}

void i2c_write(uint8_t add, uint8_t data){
	i2c_start_condition();
	i2c_setAddressDirection(add << 1, I2C_Direction_Transmitter);
	i2c_sendData(data);
	i2c_stop_condition();
}

void i2c_read(uint8_t add, uint8_t data){
	i2c_start_condition();
	i2c_setAddressDirection(add << 1, I2C_Direction_Receiver);
	data = i2c_recDataACK();
	i2c_stop_condition();
}


int main(int argc, char* argv[])
{
	GPIO_Init_local();
	I2C_Init_local(100000, 0x37); //I2C speed, own address

	while (1)
    {
		i2c_write(0x08, 0x32);
    }
}
//
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

