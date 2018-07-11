/*
*	Author: Stewart Nash
*	Date: 3/5/2018
*	File: serialport.h
*	Description: Serial IO class header
*
*/
#pragma once
#ifndef SERIALPORT_H_
#define	SERIALPORT_H_
#include <vector>
#include "visual_studio.h"
#ifdef WINDOWS_IMPLEMENTATION

#else
#include <termios.h>
#endif // WINDOWS_IMPLEMENTATION
/*
extern "C" {
#include "serial_interface.h"
}
*/
#define SERIAL_PORT_BUFFER_SIZE 0x1000 // 0x1000 = 4096

//const double EMPIRICAL_CLOCK_ADJUSTMENT = 1000;

void convertVectorToArray(std::vector<char> input, char *output);
std::vector<char> convertArrayToVector(char *input, int length);
std::vector<unsigned char> convertVectorToUnsigned(std::vector<char> input);
void displayBuffer(char *buffer, int length);
void displayHexadecimal(char *message, int length);
void displayHexadecimal(std::vector<unsigned char> message);
void displayHexadecimal(std::vector<char> message);
void printCharacter(char buffer, int length);
void printCharacter(unsigned char buffer, int length);
void printCharacterVector(std::vector<char> message);
void printStringCharacter(unsigned char buffer);

class SerialPort {
public:
	SerialPort();
	SerialPort(const char *deviceName, int lineSpeed);
	void clearRts();
	void close();
	int getBaudRate();
	double getClockAdjustment();
	bool getIsOpen();
	bool getHasName();
	bool getIsRtsSet();
	void open();
	void openRts();
	void open(const char *deviceName, int lineSpeed);
	void openRts(const char *deviceName, int lineSpeed);
	int read(std::vector<char> &buffer);
	double readTimeCalibration();				// Return clock adjustment
	void setClockAdjustment(double input);		// Adjust clock
	void setRts();
	int timedRead(std::vector<char> &buffer, double maxTime);
	void writeString(const char *message);
	void write(std::vector<char> message);

private:
	struct termios tty;
	char portName[256];
	char errorDescription[512];
	char portMessage[SERIAL_PORT_BUFFER_SIZE];
	int baudRate;
	int errorStatus;
	int fileDescriptor;
	int readLength;
	bool isOpen;
	bool hasName;
	bool isRtsSet;
	int read(char *buffer);
	int timedRead(char *buffer, double maxTime);
	double clockAdjustment;
};

#endif // !SERIALPORT_H_

