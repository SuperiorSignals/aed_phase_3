/*
*	Author: Stewart Nash
*	Date: 5/4/2018
*	File: gpiopin.cpp
*	Description: ADC pin interface class
*
*/
#include "adcpin.h"
#include <iostream>
extern "C" {
#include "adc_interface.h"
}

AdcPin::AdcPin()
{
	adcChipNumber = 0;
	adcChannelNumber = 0;
}

AdcPin::AdcPin(unsigned int chip, unsigned int channel)
{
	adcChipNumber = chip;
	adcChannelNumber = channel;
}

unsigned int AdcPin::getAdcChannelNumber() { return adcChannelNumber; }
unsigned int AdcPin::getAdcChipNumber() { return adcChipNumber; }

int AdcPin::read()
{
	int output;

	output = read_adc(adcChipNumber, adcChannelNumber);

	return 0;
}

int AdcPin::readMillivolts()
{
	return 0;
}

void AdcPin::setAdcChannelNumber(unsigned int input) { adcChannelNumber = input; }
void AdcPin::setAdcChipNumber(unsigned int input) { adcChipNumber = input; }
