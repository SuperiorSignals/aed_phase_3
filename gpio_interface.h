/*
*	File: gpio_interface.h
*	Date: April 9, 2018
*	Author: Stewart Nash
*	Description: Interface for C library of CC6 GPIOs
*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

#include "visual_studio.h"

#include <libdigiapix/gpio.h>

int gpio_close(int input);
int gpio_open(int input);
int gpio_read(int linux_kernel_id, gpio_t *result);
int gpio_read_ioexpander(int linux_kernel_id, gpio_t *result);
int gpio_write_high(int linux_kernel_id, gpio_t *result);
int gpio_write_low(int linux_kernel_id, gpio_t *result);
int gpio_mode(gpio_t *input);


#endif // !GPIO_INTERFACE_H_


