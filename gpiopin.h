/*
*	Author: Stewart Nash
*	Date: 4/10/2018
*	File: gpiopin.h
*	Description: GPIO pin interface class
*
*/
#pragma once
#ifndef GPIOPIN_H_
#define GPIOPIN_H_
#include "visual_studio.h"
#ifdef WINDOWS_IMPLEMENTATION

#else
extern "C" {
#include <libdigiapix/gpio.h>
}
#endif // WINDOWS_IMPLEMENTATION


enum GpioPinOutput {GPIO_PIN_OUTPUT_LOW, GPIO_PIN_OUTPUT_HIGH, GPIO_PIN_INPUT, GPIO_PIN_ERROR};
enum GpioType {IO_EXPANDER, MICROCONTROLLER_ASSIST, EXPANSION_GPIO, UNKNOWN_PIN_TYPE};

class GpioPin {
private:
	int linuxKernelNumber;
	gpio_t *result;
	//GpioPinOutput operation;
	GpioPinOutput mode;
	GpioType pinType;
public:
	GpioPin();
	int getLinuxKernelNumber();
	GpioPinOutput getMode();
	//GpioPinOutput getOperation();
	GpioPinOutput read();
	GpioPinOutput read(int kernelNumber);
	void setLinuxKernelNumber(int input);
	void setLinuxKernelNumber(GpioType type, int port, int pin);
	void write(GpioPinOutput input);
	void write(GpioPinOutput input, int kernelNumber);
};

#endif // !GPIOPIN_H_

