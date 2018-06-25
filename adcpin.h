/*
*	Author: Stewart Nash
*	Date: 4/10/2018
*	File: gpiopin.h
*	Description: GPIO pin interface class
*
*/
#pragma once
#ifndef ADCPIN_H_
#define ADCPIN_H_
#include "visual_studio.h"
#ifdef WINDOWS_IMPLEMENTATION

#else
extern "C" {
#include <libdigiapix/adc.h>
}
#endif // WINDOWS_IMPLEMENTATION


#define ANALOG_0_CHIP 0
#define ANALOG_0_CHANNEL 1
#define ANALOG_1_CHIP 0
#define ANALOG_1_CHANNEL 4

#define ANALOG_ONE_CHIP 0
#define ANALOG_ONE_CHANNEL 0
#define ANALOG_TWO_CHIP 0
#define ANALOG_TWO_CHANNEL 1
#define VEHICLE_VOLTAGE_CHIP 0
#define VEHICLE_VOLTAGE_CHANNEL 4

class AdcPin {
private:
	unsigned int adcChipNumber;
	unsigned int adcChannelNumber;

public:
	AdcPin();
	AdcPin(unsigned int chip, unsigned int channel);
	unsigned int getAdcChipNumber();
	unsigned int getAdcChannelNumber();
	int read();
	int readMillivolts();
	void setAdcChipNumber(unsigned int input);
	void setAdcChannelNumber(unsigned int input);
};

#endif // !ADCPIN_H_
