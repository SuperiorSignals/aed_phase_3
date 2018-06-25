/*
*	File: adc_interface.h
*	Date: May 4, 2018
*	Author: Stewart Nash
*	Description: Interface for C library of CC6 ADCs
*/
#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#include "visual_studio.h"

#include <libdigiapix/adc.h>

int read_adc(int chip, int channel);

#endif // !ADC_INTERFACE_H_
