/*
 * serial_interface.h
 *
 *  Created on: Mar 2, 2018
 *      Author: Stewart Nash
 */
#ifndef SERIAL_INTERFACE_H_
#define SERIAL_INTERFACE_H_

#include "visual_studio.h"
#ifdef WINDOWS_IMPLEMENTATION

#else
#include <termios.h>
#endif // WINDOWS_IMPLEMENTATION

int open_port(char *port_name, int baud_rate, int port_mode, struct termios *tty, int *file_descriptor, char *error_description);
int open_port_rts_enabled(char *port_name, int baud_rate, int port_mode, struct termios *tty, int *file_descriptor, char *error_description);
int setup_port(int port_speed, int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description);
int setup_port_rts_enabled(int port_speed, int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description);
int parametrize_port(int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description);
int read_port(char *port_message, int *read_length, struct termios *tty, int file_descriptor, char *error_description);
int timed_read_port(char *port_message, int timer, struct termios *tty, int file_descriptor, char *error_description);
int write_port_string(const char *port_message, struct termios *tty, int file_descriptor, char *error_description);
int write_port(const char *port_message, int size, struct termios *tty, int file_descriptor, char *error_description);
int close_port (char *port_name, struct termios *tty, int file_descriptor, char *error_description);
int set_rts(int file_descriptor);
int clear_rts(int file_descriptor);

#endif /* SERIAL_INTERFACE_H_ */
