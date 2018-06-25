/*
 *	File: serial_interface.c
 *	Date: March 2, 2018
 *	Author: Stewart Nash
 *	Description: interface to serial C library
 */
#include "serial_interface.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#ifdef WINDOWS_IMPLEMENTATION
#include <stdio.h>

int open_port(char *port_name, int baud_rate, int port_mode, struct termios *tty, int *file_descriptor, char *error_description)
{
	return 0;
}

int setup_port(int port_speed, int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description)
{
	return 0;
}

int parametrize_port(int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description)
{
	return 0;
}

int read_port(char *port_message, int *read_length, struct termios *tty, int file_descriptor, char *error_description)
{
	return 0;
}

int timed_read_port(char *port_message, int timer, struct termios *tty, int file_descriptor, char *error_description)
{
	return 0;
}

int write_port_string(char *port_message, struct termios *tty, int file_descriptor, char *error_description)
{
	printf(port_message);
	return 0;
}

int write_port(const char *port_message, int size, struct termios *tty, int file_descriptor, char *error_description)
{
	for (int i = 0; i < size; i++) {
		printf("%c", port_message[i]);
	}
	return 0;
}

int close_port(char *port_name, struct termios *tty, int file_descriptor, char *error_description)
{
	return 0;
}
#else
#include <termios.h>
#include <unistd.h>

int open_port(char *port_name, int baud_rate, int port_mode, struct termios *tty, int *file_descriptor, char *error_description)
{
	*file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_SYNC);
	if (file_descriptor < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	setup_port(baud_rate, 0, 5, tty, *file_descriptor, error_description);

	return 0;
}

int open_port_rts_enabled(char *port_name, int baud_rate, int port_mode, struct termios *tty, int *file_descriptor, char *error_description)
{
	*file_descriptor = open(port_name, O_RDWR | O_NOCTTY | O_SYNC);
	if (file_descriptor < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	setup_port_rts_enabled(baud_rate, 0, 5, tty, *file_descriptor, error_description);

	return 0;
}

int setup_port(int port_speed, int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description)
{
	speed_t speed;

	switch (port_speed) {
	case 1200:
		speed = B1200;
		break;
	case 2400:
		speed = B2400;
		break;
	case 4800:
		speed = B4800;
		break;
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	case 38400:
		speed = B38400;
		break;
	case 57600:
		speed = B57600;
		break;
	case 115200:
		speed = B115200;
		break;
	default:
		//printf("ERROR [int setup_port(int port_speed, int, int, struct termios *, int, char *)]: Unrecognized port speed %d.\n", port_speed);
		speed = B9600;
		break;
	}

	if (tcgetattr(file_descriptor, tty) < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	cfsetospeed(tty, speed);
	cfsetispeed(tty, speed);
	tty->c_cflag |= (CLOCAL | CREAD);	// ignore modem controls
	tty->c_cflag &= ~CSIZE;
	tty->c_cflag |= CS8; 				// 8-bit characters
	tty->c_cflag &= ~PARENB;			// no parity bit
	tty->c_cflag &= ~CSTOPB;			// only need 1 stop bit
	tty->c_cflag &= ~CRTSCTS;			// no hardware flow control
	//tty->c_cflag |= CRTSCTS;			// enable hardware flow control
										// setup for non-canonical mode
	tty->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty->c_oflag &= ~OPOST;
	// fetch bytes as they become available
	//tty->c_cc[VMIN] = 1;
	tty->c_cc[VMIN] = minimum_count;
	tty->c_cc[VTIME] = maximum_time;
	if (tcsetattr(file_descriptor, TCSANOW, tty) != 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}

	return 0;
}

int setup_port_rts_enabled(int port_speed, int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description)
{
	speed_t speed;

	switch (port_speed) {
	case 1200:
		speed = B1200;
		break;
	case 2400:
		speed = B2400;
		break;
	case 4800:
		speed = B4800;
		break;
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	case 38400:
		speed = B38400;
		break;
	case 57600:
		speed = B57600;
		break;
	case 115200:
		speed = B115200;
		break;
	default:
		//printf("ERROR [int setup_port(int port_speed, int, int, struct termios *, int, char *)]: Unrecognized port speed %d.\n", port_speed);
		speed = B9600;
		break;
	}

	if (tcgetattr(file_descriptor, tty) < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	cfsetospeed(tty, speed);
	cfsetispeed(tty, speed);
	tty->c_cflag |= (CLOCAL | CREAD);	// ignore modem controls
	tty->c_cflag &= ~CSIZE;
	tty->c_cflag |= CS8; 				// 8-bit characters
	tty->c_cflag &= ~PARENB;			// no parity bit
	tty->c_cflag &= ~CSTOPB;			// only need 1 stop bit
	//tty->c_cflag &= ~CRTSCTS;			// no hardware flow control
	tty->c_cflag |= CRTSCTS;			// enable hardware flow control
										// setup for non-canonical mode
	tty->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty->c_oflag &= ~OPOST;
	// fetch bytes as they become available
	//tty->c_cc[VMIN] = 1;
	tty->c_cc[VMIN] = minimum_count;
	tty->c_cc[VTIME] = maximum_time;
	if (tcsetattr(file_descriptor, TCSANOW, tty) != 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}

	return 0;
}

int parametrize_port(int minimum_count, int maximum_time, struct termios *tty, int file_descriptor, char *error_description)
{
	if (tcgetattr(file_descriptor, tty) < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	tty->c_cc[VMIN] = minimum_count ? 1 : 0;
	tty->c_cc[VTIME] = maximum_time;				// one-tenths of a second
	return 0;
}

int read_port(char *port_message, int *read_length, struct termios *tty, int file_descriptor, char *error_description)
{
	int length;

	length = read(file_descriptor, port_message, sizeof(port_message) - 1);
	*read_length = length;
	if (length < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}

	return 0;
}

int timed_read_port(char *port_message, int timer, struct termios *tty, int file_descriptor, char *error_description)
{
	int length;

	length = read(file_descriptor, port_message, sizeof(port_message) - 1);
	if (length < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}

	return 0;
}

int write_port(const char *port_message, int size, struct termios *tty, int file_descriptor, char *error_description)
{
	int length;

	length = write(file_descriptor, port_message, size);
	if (length != size) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	tcdrain(file_descriptor);		// delay for output

	return 0;
}

int write_port_string(const char *port_message, struct termios *tty, int file_descriptor, char *error_description)
{
	int size, length;
	size_t string_size;

	string_size = strlen(port_message);
	size = (int)string_size;
	length = write(file_descriptor, port_message, size);
	if (length != size) {
		strcpy(error_description, strerror(errno));
		return -1;
	}
	tcdrain(file_descriptor);		// delay for output

	return 0;
}

int close_port(char *port_name, struct termios *tty, int file_descriptor, char *error_description)
{
	int result;

	result = close(file_descriptor);
	if (result < 0) {
		strcpy(error_description, strerror(errno));
		return -1;
	}

	return 0;
}

int set_rts(int file_descriptor)
{
	int status;

	if (ioctl(file_descriptor, TIOCMGET, &status) == -1) { // Error: with TIOCMGET
		return -1;
	}
	status |= TIOCM_RTS;
	if (ioctl(file_descriptor, TIOCMSET, &status) == -1) { // Error: with TIOCMSET
		return -2;
	}

	return 0;
}

int clear_rts(int file_descriptor)
{
	int status;

	if (ioctl(file_descriptor, TIOCMGET, &status) == -1) { // Error: with TIOCMGET
		return -1;
	}
	status &= ~TIOCM_RTS;
	if (ioctl(file_descriptor, TIOCMSET, &status) == -1) { // Error: with TIOCMSET
		return -2;
	}

	return 0;
}

#endif // WINDOWS_IMPLEMENTATION
