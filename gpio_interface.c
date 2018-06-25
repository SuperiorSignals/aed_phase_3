/*
*	File: gpio_interface.c
*	Date: April 10, 2018
*	Author: Stewart Nash
*	Description: Interface for C library of CC6 GPIOs
*/
#include "gpio_interface.h"
#ifdef WINDOWS_IMPLEMENTATION
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#else
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libgen.h>
#include <unistd.h>
#include <libdigiapix/gpio.h>
#endif // WINDOWS_IMPLEMENTATION



#define TEST_LOOPS			6
#define DEFAULT_USER_LED_ALIAS		"USER_LED"
#define DEFAULT_USER_BUTTON_ALIAS	"USER_BUTTON"

static gpio_t *gpio_input;
static gpio_t *gpio_output;

int gpio_close(int input)
{
	int output;

	return output;
}

int gpio_open(int input)
{
	int output;
	
	return output;
}

int gpio_read(int linux_kernel_id, gpio_t *result)
{
	int output;
	gpio_value_t reading;

	result = ldx_gpio_request((unsigned int)linux_kernel_id, GPIO_INPUT, REQUEST_SHARED);

	if (result == NULL) {
		output = -1;
	} else {
		//ldx_gpio_set_mode(result, GPIO_INPUT);
		reading = ldx_gpio_get_value(result);
		if (reading == GPIO_HIGH) {
			output = 1;
		} else if (reading == GPIO_LOW) {
			output = 0;
		} else if (reading == GPIO_VALUE_ERROR) {
			output = -2;
		} else {
			output = -3;
			printf("ERROR [int gpio_read(int linux_kernel_id)]: unknown read value\n");
		}
		ldx_gpio_free(result);
	}
	return output;
}

int gpio_read_ioexpander(int linux_kernel_id, gpio_t *result)
{
	int output;
	gpio_value_t reading;

	result = ldx_gpio_request((unsigned int)linux_kernel_id, GPIO_INPUT, REQUEST_SHARED);

	if (result == NULL) {
		output = -1;
	} else {
		ldx_gpio_set_mode(result, GPIO_INPUT);
		reading = ldx_gpio_get_value(result);
		if (reading == GPIO_HIGH) {
			output = 1;
		} else if (reading == GPIO_LOW) {
			output = 0;
		} else if (reading == GPIO_VALUE_ERROR) {
			output = -2;
		} else {
			output = -3;
			printf("ERROR [int gpio_read(int linux_kernel_id)]: unknown read value\n");
		}
		ldx_gpio_free(result);
	}
	return output;
}

int gpio_write_low(int linux_kernel_id, gpio_t *result)
{
	int output;

	result = ldx_gpio_request((unsigned int)linux_kernel_id, GPIO_OUTPUT_LOW, REQUEST_SHARED);

	if (result == NULL) {
		output = -1;
	} else {
		output = 0;
		ldx_gpio_set_value(result, GPIO_LOW);
		ldx_gpio_free(result);
	}

	return output;
}

int gpio_write_high(int linux_kernel_id, gpio_t *result)
{
	int output;

	result = ldx_gpio_request((unsigned int)linux_kernel_id, GPIO_OUTPUT_HIGH, REQUEST_SHARED);

	if (result == NULL) {
		output = -1;
	} else {
		output = 0;
		ldx_gpio_set_value(result, GPIO_HIGH);
		ldx_gpio_free(result);
	}

	return output;
}

int gpio_mode(gpio_t *input)
{
	int output;
	gpio_mode_t result;

	if (input == NULL) {
		output = -1;
	} else {
		result = ldx_gpio_get_mode(input);
		if (result == GPIO_INPUT) {
			output = 0;
		} else if (result == GPIO_OUTPUT_LOW) {
			output = 1;
		} else if (result == GPIO_OUTPUT_HIGH) {
			output = 2;
		} else if (result == GPIO_MODE_ERROR) {
			output = -1;
		} else { // Put in additional values
			output = 3;
		}
	}
	
	return output;
}

struct gpio_interrupt_cb_data {
	gpio_t *gpio;
	gpio_value_t value;
	int remaining_loops;
};

/*
 * usage_and_exit() - Show usage information and exit with 'exitval' return
 *					  value
 *
 * @name:	Application name.
 * @exitval:	The exit code.
 */
static void usage_and_exit(char *name, int exitval)
{
	fprintf(stdout,
		"Example application using libdigiapix GPIO support\n"
		"\n"
		"Usage: %s <gpio_in> <gpio_out>\n\n"
		"<gpio_in>     Push-button GPIO number or alias\n"
		"<gpio_out>    LED GPIO number or alias\n"
		"\n"
		"Aliases for GPIO numbers can be configured in the library config file\n"
		"\n", name);

	exit(exitval);
}

/*
 * cleanup() - Frees all the allocated memory before exiting
 */
static void cleanup(void)
{
	// Stop the interrupt handler thread
	//ldx_gpio_stop_wait_interrupt(gpio_input);

	// Free gpios
	ldx_gpio_free(gpio_input);
	ldx_gpio_free(gpio_output);
}

/*
 * sigaction_handler() - Handler to execute after receiving a signal
 *
 * @signum:	Received signal.
 */
static void sigaction_handler(int signum)
{
	// 'atexit' executes the cleanup function
	exit(EXIT_FAILURE);
}

/*
 * register_signals() - Registers program signals
 */
static void register_signals(void)
{
	struct sigaction action;

	action.sa_handler = sigaction_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);

	sigaction(SIGHUP, &action, NULL);
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
}

/*
 * parse_argument() - Parses the given string argument and returns the
 *					  corresponding integer value
 *
 * @argv:	Argument to parse in string format.
 *
 * Return: The parsed integer argument, -1 on error.
 */
static int parse_argument(char *argv)
{
	char *endptr;
	long value;

	errno = 0;
	value = strtol(argv, &endptr, 10);

	if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
	    || (errno != 0 && value == 0))
		return -1;

	if (endptr == argv)
		return ldx_gpio_get_kernel_number(endptr);

	return value;
}

/*
 * gpio_interrupt_cb() - GPIO callback for interrupts
 *
 * @arg:	GPIO interrupt data (struct gpio_interrupt_cb_data).
 */
static int gpio_interrupt_cb(void *arg)
{
	struct gpio_interrupt_cb_data *data = arg;

	printf("Input GPIO interrupt detected; toggling output GPIO\n");

	// Toggle output GPIO
	data->value = data->value ? GPIO_LOW : GPIO_HIGH;
	ldx_gpio_set_value(data->gpio, data->value);

	// Decrease remaining loops
	data->remaining_loops -= 1;

	return 0;
}

int gpio_main(int argc, char *argv[])
{
	time_t start_time, end_time;
	double time_difference;
	int button, led, i;
	gpio_value_t output_value = GPIO_LOW;	// Should match the GPIO request mode
	struct gpio_interrupt_cb_data cb_data;
	char *name = basename(argv[0]);

	// Check input parameters
	if (argc == 1) {
		// Use default values
		button = parse_argument(DEFAULT_USER_BUTTON_ALIAS);
		led = parse_argument(DEFAULT_USER_LED_ALIAS);
	} else if (argc == 3) {
		// Parse command line arguments
		button = parse_argument(argv[1]);
		led = parse_argument(argv[2]);
	} else {
		usage_and_exit(name, EXIT_FAILURE);
	}

	if (button < 0 || led < 0) {
		printf("Unable to parse button and led GPIOs\n");
		return EXIT_FAILURE;
	}
	printf("led value: %d\n", led);

	// Register signals and exit cleanup function
	atexit(cleanup);
	register_signals();

	// Request input GPIO
	gpio_input =
		ldx_gpio_request((unsigned int)button, GPIO_IRQ_EDGE_RISING,
			 REQUEST_SHARED);
	if (!gpio_input) {
		printf("Failed to initialize input GPIO\n");
		return EXIT_FAILURE;
	}

	// Request output GPIO
	gpio_output =
	    ldx_gpio_request((unsigned int)led, GPIO_OUTPUT_LOW, REQUEST_SHARED);
	if (!gpio_output) {
		printf("Failed to initialize output GPIO\n");
		return EXIT_FAILURE;
	}

	// Configure input GPIO to active HIGH
	ldx_gpio_set_active_mode(gpio_input, GPIO_ACTIVE_HIGH);
	ldx_gpio_set_value(gpio_output, GPIO_HIGH);

	time_difference = 0;
	time(&start_time);
	do {
		time(&end_time);
		time_difference = difftime(end_time, start_time);
	} while (time_difference < 5.0);

	ldx_gpio_set_value(gpio_output, GPIO_LOW);

	return EXIT_SUCCESS;
	 //Test blocking interrupt mode
/*
	printf("[INFO] Testing interrupt blocking mode\n");
	printf("Press the button (for %d events):\n", TEST_LOOPS);
	for (i = 0; i < TEST_LOOPS; i++) {
		if (ldx_gpio_wait_interrupt(gpio_input, -1) == GPIO_IRQ_ERROR_NONE) {
			printf("Press %d; toggling output GPIO\n", i + 1);
			output_value = output_value ? GPIO_LOW : GPIO_HIGH;
			ldx_gpio_set_value(gpio_output, output_value);
		}
	}

	 // Test async mode
	printf("[INFO] Testing interrupt asynchronous mode\n");

	// Initialize data to be passed to the interrupt handler
	cb_data.gpio = gpio_output;
	cb_data.value = output_value;
	cb_data.remaining_loops = TEST_LOOPS;

	printf
	    ("Parent process will wait until %d interrupts have been detected\n",
	     TEST_LOOPS);

	if (ldx_gpio_start_wait_interrupt(gpio_input, &gpio_interrupt_cb, &cb_data)
	    != EXIT_SUCCESS) {
		printf("Failed to start interrupt handler thread\n");
		return EXIT_FAILURE;
	}

	// Parent process
	while (cb_data.remaining_loops > 0) {
		printf("Parent process: waiting ...\n");
		sleep(5);
	}
	printf("Parent process: no remaining interrupts. Test finished\n");
*/
	//'atexit' executes the cleanup function

	//return EXIT_SUCCESS;
}
