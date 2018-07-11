/*
*	Author: Stewart Nash
*	Date: 4/10/2018
*	File: gpiopin.cpp
*	Description: GPIO pin interface class
*
*/
#include "gpiopin.h"
#include <iostream>
extern "C" {
#include "gpio_interface.h"
}

GpioPin::GpioPin()
{
	linuxKernelNumber = 0;
	result = NULL;
	mode = GPIO_PIN_ERROR;
	pinType = UNKNOWN_PIN_TYPE;
}

int GpioPin::getLinuxKernelNumber()
{
	return linuxKernelNumber;
}

GpioPinOutput GpioPin::getMode()
{
	/*
	int output;

	output = gpio_mode(result);
	if (output == 0) {
		return GPIO_PIN_INPUT;
	} else if (output == 1) {
		return GPIO_PIN_OUTPUT_LOW;
	} else if (output == 2) {
		return GPIO_PIN_OUTPUT_HIGH;
	}

	return GPIO_PIN_ERROR;
	*/
	return mode;
}

GpioPinOutput GpioPin::read()
{
	int output;

	if (pinType == IO_EXPANDER){
		if (linuxKernelNumber) {
			// -1 result is NULL
			// 1 GPIO_HIGH
			// 0 GPIO_LOW
			// -2 GPIO_VALUE_ERROR
			// -3 other
			output = gpio_read_ioexpander(linuxKernelNumber, result);
		} else {
			mode = GPIO_PIN_ERROR;
			return GPIO_PIN_ERROR;
		}

		if (result == NULL) { // Could not allocate gpio
			mode = GPIO_PIN_ERROR;
		} else {
			mode = GPIO_PIN_INPUT;
		}

		if (output == 0) {
			return GPIO_PIN_OUTPUT_LOW;
		} else if (output == 1) {
			return GPIO_PIN_OUTPUT_HIGH;
		}
	} else {
		if (linuxKernelNumber) {
			// -1 result is NULL
			// 1 GPIO_HIGH
			// 0 GPIO_LOW
			// -2 GPIO_VALUE_ERROR
			// -3 other
			output = gpio_read(linuxKernelNumber, result);
		} else {
			mode = GPIO_PIN_ERROR;
			return GPIO_PIN_ERROR;
		}

		if (result == NULL) { // Could not allocate gpio
			mode = GPIO_PIN_ERROR;
		} else {
			mode = GPIO_PIN_INPUT;
		}

		if (output == 0) {
			return GPIO_PIN_OUTPUT_LOW;
		} else if (output == 1) {
			return GPIO_PIN_OUTPUT_HIGH;
		}
	}
	return GPIO_PIN_ERROR;
}

GpioPinOutput GpioPin::read(int kernelNumber)
{
	GpioPinOutput output;

	linuxKernelNumber = kernelNumber;
	output = read();

	return output;
}

void GpioPin::setLinuxKernelNumber(int input)
{
	linuxKernelNumber = input;
}

void GpioPin::setLinuxKernelNumber(GpioType type, int port, int pin)
{
	linuxKernelNumber = 0;
	if (type == EXPANSION_GPIO) {
		if (pin > 0 && port > 0) {
			linuxKernelNumber = 32 * (port - 1) + pin;
		}
	} else if (type == IO_EXPANDER) {
		linuxKernelNumber = 465 + pin;
	} else if (type == MICROCONTROLLER_ASSIST) {

	} else {

	}
	pinType = type;
}

void GpioPin::write(GpioPinOutput input) {
	if (input == GPIO_PIN_OUTPUT_LOW) {
		gpio_write_low(linuxKernelNumber, result);
		if (result == NULL) {
			mode = GPIO_PIN_ERROR;
		} else {
			mode = GPIO_PIN_OUTPUT_LOW;
		}
	} else if (input == GPIO_PIN_OUTPUT_HIGH) {
		gpio_write_high(linuxKernelNumber, result);
		if (result == NULL) {
			mode = GPIO_PIN_ERROR;
		} else {
			mode = GPIO_PIN_OUTPUT_HIGH;
		}
	} else {
		std::cout << "ERROR [void GpioPin::write(GpioPinOutput input)]: input not recognized";
		std::cout << std::endl;
	}
}

void GpioPin::write(GpioPinOutput input, int kernelNumber) {
	linuxKernelNumber = kernelNumber;
	write(input);
}
