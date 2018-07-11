/*
*	Author: Stewart Nash
*	Date: 3/5/2018
*	File: serialport.cpp
*	Description: Serial IO class source
*
*/
#include "serialport.h"
#include <iomanip>
#include <iostream>
#include <string.h>
#include <time.h>
#ifdef WINDOWS_IMPLEMENTATION

#else
#include <termios.h>
#endif // WINDOWS_IMPLEMENTATION
extern "C" {
#include "serial_interface.h"
}

#include "testfunction.h"

const double EMPIRICAL_CLOCK_ADJUSTMENT = 0.3667 * 1000;

SerialPort::SerialPort()
{
	isOpen = false;
	hasName = false;
	isRtsSet = false;

	baudRate = 9600;
	errorStatus = 0;
	fileDescriptor = 0;
	readLength = 0;
	clockAdjustment = EMPIRICAL_CLOCK_ADJUSTMENT;
	//clockAdjustment = clock_test(15);
}

SerialPort::SerialPort(const char *deviceName, int lineSpeed)
{
	isOpen = false;
	strcpy(portName, deviceName);
	hasName = true;

	// Add baud rate error checking?
	baudRate = lineSpeed;
	errorStatus = 0;
	fileDescriptor = 0;
	readLength = 0;
	clockAdjustment = EMPIRICAL_CLOCK_ADJUSTMENT;
	//clockAdjustment = clock_test(15);
}

void SerialPort::clearRts()
{
	errorStatus = clear_rts(fileDescriptor);
	if (errorStatus >= 0) {
		isRtsSet = false;
	} else {
		std::cout << "Error [void SerialPort::clearRts()]" << std::endl;
	}
}

void SerialPort::close()
{
	if (isOpen) {
		errorStatus = close_port(portName, &tty, fileDescriptor, errorDescription);
		if (errorStatus != 0) {
			std::cout << "Error " << errorStatus << " [void SerialPort::close()]: " << errorDescription << std::endl;
		} else {
			isOpen = false;
		}
	}
}

int SerialPort::getBaudRate()

{
	return baudRate;
}

double SerialPort::getClockAdjustment()
{
	return clockAdjustment;
}

bool SerialPort::getIsOpen()
{
	return isOpen;
}

bool SerialPort::getHasName()
{
	return hasName;
}

bool SerialPort::getIsRtsSet()
{
	return isRtsSet;
}

void SerialPort::open()
{
	if (hasName) {
		errorStatus = open_port(portName, baudRate, 0, &tty, &fileDescriptor, errorDescription);
		if (errorStatus == 0) {
			isOpen = true;
		} else {
			isOpen = false;
			std::cout << "Error " << errorStatus << " [void SerialPort::open()]: " << errorDescription << std::endl;
		}
	}
}

void SerialPort::openRts()
{
	if (hasName) {
		errorStatus = open_port_rts_enabled(portName, baudRate, 0, &tty, &fileDescriptor, errorDescription);
		if (errorStatus == 0) {
			isOpen = true;
		} else {
			isOpen = false;
			std::cout << "Error " << errorStatus << " [void SerialPort::open()]: " << errorDescription << std::endl;
		}
	}
}

void SerialPort::open(const char *deviceName, int lineSpeed)
{
	strcpy(portName, deviceName);
	hasName = true;
	errorStatus = open_port(portName, lineSpeed, 0, &tty, &fileDescriptor, errorDescription);
	if (errorStatus == 0) {
		isOpen = true;
	} else {
		isOpen = false;
		std::cout << "Error " << errorStatus << " [void SerialPort::open()]: " << errorDescription << std::endl;
	}
}

void SerialPort::openRts(const char *deviceName, int lineSpeed)
{
	strcpy(portName, deviceName);
	hasName = true;
	errorStatus = open_port_rts_enabled(portName, lineSpeed, 0, &tty, &fileDescriptor, errorDescription);
	if (errorStatus == 0) {
		isOpen = true;
	} else {
		isOpen = false;
		std::cout << "Error " << errorStatus << " [void SerialPort::open()]: " << errorDescription << std::endl;
	}
}

int SerialPort::read(char *buffer)
{
	if (isOpen) {
		errorStatus = read_port(buffer, &readLength, &tty, fileDescriptor, errorDescription);
		if (errorStatus != 0) {
			std::cout << "Error " << errorStatus << " [void SerialPort::read(char *buffer)]: " << errorDescription << std::endl;
		}
	}

	return readLength;
}

int SerialPort::read(std::vector<char> &buffer)
{
	int length;
	char temporary[SERIAL_PORT_BUFFER_SIZE];

	buffer.clear();
	length = this->read(temporary);
	if (length > 0) {
		for (int i = 0; i < length; i++) {
			buffer.push_back(temporary[i]);
		}
	}

	return length;
}

double SerialPort::readTimeCalibration()
{
	double output;

	output = 0; // Implement this function

	return output;
}

void SerialPort::setClockAdjustment(double input)
{
	clockAdjustment = input;
}

void SerialPort::setRts()
{
	errorStatus = set_rts(fileDescriptor);
	if (errorStatus >= 0) {
		isRtsSet = true;
	} else {
		std::cout << "Error [void SerialPort::setRts()]" << std::endl;
	}
}

int SerialPort::timedRead(char *buffer, double maxTime)
{
	clock_t startTime, endTime;
	double timeDifference;
	int size;
	char message[SERIAL_PORT_BUFFER_SIZE];

	size = 0;
	startTime = time(NULL);
	if (isOpen) {
		parametrize_port(0, 1, &tty, fileDescriptor, errorDescription);
		timeDifference = 0.0;
		startTime = clock();
		do {
			read_port(message, &readLength, &tty, fileDescriptor, errorDescription);
			for (int i = 0; i < readLength; i++) {
				buffer[size++] = message[i];
			}
			endTime = clock();
			timeDifference = clockAdjustment * static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC);
		} while (timeDifference < maxTime && size < SERIAL_PORT_BUFFER_SIZE);
		std::cout << "[int SerialPort::timedRead(char *, double)] max time:    ";
		std::cout << maxTime << " seconds" << std::endl;
		std::cout << "[int SerialPort::timedRead(char *, double)] actual time: ";
		std::cout << timeDifference << " seconds" << std::endl;
		std::cout << "[int SerialPort::timedRead(char *, double)] max buffer size: ";
		std::cout << SERIAL_PORT_BUFFER_SIZE << std::endl;
		std::cout << "[int SerialPort::timedRead(char *, double)] actual buffer size: ";
		std::cout << size << std::endl;
	}

	return size;
}

int SerialPort::timedRead(std::vector<char> &buffer, double maxTime)
{
	int length;
	char temporary[SERIAL_PORT_BUFFER_SIZE];

	buffer.clear();
	length = this->timedRead(temporary, maxTime);
	if (length > 0) {
		for (int i = 0; i < length; i++) {
			buffer.push_back(temporary[i]);
		}
	}

	return length;
}

void SerialPort::write(std::vector<char> message) {
	char buffer[SERIAL_PORT_BUFFER_SIZE];
	int length;

	length = message.size();
	if (length <= SERIAL_PORT_BUFFER_SIZE) {
		for (int i = 0; i < length; i++) {
			buffer[i] = message[i];
		}
		errorStatus = write_port(buffer, length, &tty, fileDescriptor, errorDescription);
	} else {
		std::cout << "ERROR [void SerialPort::write(std::vector<char> message)]: message size (" << length;
		std::cout << " bytes) exceeds array length (" << SERIAL_PORT_BUFFER_SIZE << " bytes)" << std::endl;
	}
}

void SerialPort::writeString(const char *message)
{
	if (isOpen) {
		errorStatus = write_port_string(message, &tty, fileDescriptor, errorDescription);
		if (errorStatus != 0) {
			std::cout << "Error " << errorStatus << " [void SerialPort::write(char *message)]: " << errorDescription << std::endl;
		}
	}
}

void convertVectorToArray(std::vector<char> input, char *output)
{
	int length;

	length = input.size();
	for (int i = 0; i < length; i++) {
		output[i] = input[i];
	}
}

std::vector<char> convertArrayToVector(char *input, int length)
{
	std::vector<char> output;

	for (int i = 0; i < length; i++) {
		output.push_back(input[i]);
	}

	return output;
}

std::vector<unsigned char> convertVectorToUnsigned(std::vector<char> input)
{
	int length;
	std::vector<unsigned char> output;

	length = input.size();
	for (int i = 0; i < length; i++) {
		output.push_back(static_cast<unsigned char>(input[i]));
	}

	return output;
}

void displayBuffer(char *buffer, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		if (buffer[i] > 0x1F && buffer[i] < 0x7F) {
			std::cout << buffer[i];
		} else if (buffer[i] == 0xA) {
			std::cout << '\n';
		} else if (buffer[i] == 0xD) {
			std::cout << '\r';
		}
	}
	std::cout << std::endl;
}

void displayHexadecimal(char *buffer, int length)
{
	const int NUMBER_OF_COLUMNS = 0x0F;
	const int NUMBER_OF_DATA = 3;
	const int DATA_WIDTH = 4;
	int numberOfRows;

	numberOfRows = length / NUMBER_OF_COLUMNS;
	if (length % NUMBER_OF_COLUMNS) {
		numberOfRows++;
	}
	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < NUMBER_OF_DATA; j++) {
			if (j == 0) {
				std::cout << std::setw(7) << "Loc: ";
			} else if (j == 1) {
				std::cout << std::setw(7) << "Val: ";
			} else if (j == 2) {
				std::cout << std::setw(7) << "Chr: ";
			}
			for (int k = 0; k < NUMBER_OF_COLUMNS; k++) {
				int l = i * NUMBER_OF_COLUMNS + k;
				if (l < length) {
					if (j == 0) {
						std::cout << std::setw(DATA_WIDTH) << std::uppercase << std::hex << l;
					} else if (j == 1) {
						std::cout << std::setw(DATA_WIDTH) << std::hex << static_cast<int>(buffer[l]);
					} else if (j == 2) {
						printCharacter(buffer[l], DATA_WIDTH);
					}
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void displayHexadecimal(std::vector<unsigned char> buffer)
{
	const int NUMBER_OF_COLUMNS = 0x0F;
	const int NUMBER_OF_DATA = 3;
	const int DATA_WIDTH = 4;
	int numberOfRows;
	int length;

	length = buffer.size();
	for (int i = 0; i < length; i++) {
printStringCharacter(buffer[i]);
	}
	std::cout << std::endl;
	numberOfRows = length / NUMBER_OF_COLUMNS;
	if (length % NUMBER_OF_COLUMNS) {
		numberOfRows++;
	}
	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < NUMBER_OF_DATA; j++) {
			if (j == 0) {
				std::cout << std::setw(7) << "Loc: ";
			} else if (j == 1) {
				std::cout << std::setw(7) << "Val: ";
			} else if (j == 2) {
				std::cout << std::setw(7) << "Chr: ";
			}
			for (int k = 0; k < NUMBER_OF_COLUMNS; k++) {
				int l = i * NUMBER_OF_COLUMNS + k;
				if (l < length) {
					if (j == 0) {
						std::cout << std::setw(DATA_WIDTH) << std::uppercase << std::hex << l;
					} else if (j == 1) {
						std::cout << std::setw(DATA_WIDTH) << std::hex << static_cast<int>(buffer[l]);
					} else if (j == 2) {
						printCharacter(buffer[l], DATA_WIDTH);
					}
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void displayHexadecimal(std::vector<char> input)
{
	std::vector<unsigned char> buffer;
	const int NUMBER_OF_COLUMNS = 0x0F;
	const int NUMBER_OF_DATA = 3;
	const int DATA_WIDTH = 4;
	int numberOfRows;
	int length;

	buffer = convertVectorToUnsigned(input);
	length = buffer.size();
	for (int i = 0; i < length; i++) {
		printStringCharacter(buffer[i]);
	}
	std::cout << std::endl;
	numberOfRows = length / NUMBER_OF_COLUMNS;
	if (length % NUMBER_OF_COLUMNS) {
		numberOfRows++;
	}
	for (int i = 0; i < numberOfRows; i++) {
		for (int j = 0; j < NUMBER_OF_DATA; j++) {
			if (j == 0) {
				std::cout << std::setw(7) << "Loc: ";
			} else if (j == 1) {
				std::cout << std::setw(7) << "Val: ";
			} else if (j == 2) {
				std::cout << std::setw(7) << "Chr: ";
			}
			for (int k = 0; k < NUMBER_OF_COLUMNS; k++) {
				int l = i * NUMBER_OF_COLUMNS + k;
				if (l < length) {
					if (j == 0) {
						std::cout << std::setw(DATA_WIDTH) << std::uppercase << std::hex << l;
					} else if (j == 1) {
						std::cout << std::setw(DATA_WIDTH) << std::hex << static_cast<int>(buffer[l]);
					} else if (j == 2) {
						printCharacter(buffer[l], DATA_WIDTH);
					}
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void printCharacter(char input, int width)
{
	if (input > 0x20 && input < 0x7F) {
		std::cout << std::setw(width) << input;
	} else {
		std::cout << std::setw(width) << " ";
	}
}

void printCharacter(unsigned char input, int width)
{
	if (input > 0x20 && input < 0x7F) {
		std::cout << std::setw(width) << static_cast<char>(input);
	} else {
		std::cout << std::setw(width) << " ";
	}
}

void printCharacterVector(std::vector<char> message)
{
	for (int i = 0; i < message.size(); i++) {
		std::cout << message[i];
	}
}

void printStringCharacter(unsigned char input) 
{
	if (input > 0x1F && input < 0x7F) {
		std::cout << static_cast<char>(input);
	} else if (input == 0xD) {
		std::cout << '\r';
	} else if (input == 0xA) {
		std::cout << '\n';
	} else {
		//std::cout << '\xB1';
	}
}
