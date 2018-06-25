/*
*	Author: Stewart Nash
*	Date: 3/20/2018
*	File: xbee.h
*	Description: XBee interface class header
*
*/
#pragma once
#ifndef XBEE_H_
#define XBEE_H_
#include <string>
#include "serialport.h"

enum BaudRate {
	BAUD_RATE_UNDEFINED = 0,
	BAUD_RATE_9600 = 9600,
	BAUD_RATE_115200 = 115200
};

enum OperationMode {
	UNDEFINED_MODE = 15,
	TRANSPARENT_MODE = 0,
	API_MODE = 1,
	API_MODE_ESCAPED = 2,
	MICROPYTHON_MODE = 4,
	BYPASS_MODE = 5
};


class XBee {
public:
	XBee();
	int getBaudRate();
	unsigned int getGuardTime();
	OperationMode getOperationMode();
	void openPort(const char *deviceName, int baudRate);
	void openPortRts(const char *deviceName, int baudRate);
	//void setBaudRate(BaudRate input);
	void setGuardTime(unsigned int input);
	void setOperationMode(OperationMode input);
	void enterApiMode();
	void enterTransparentMode();
	void enterCommandMode();
	void exitApiMode();
	void exitCommandMode();
	std::vector<char> readReply();
	std::vector<char> readReply(double input);
	void startRts();
	void stopRts();
	void writeCommand(const char *input);
	void writeCommand(std::string input);
	void writeString(const char *input);
	void writeString(std::string input);
	void writeVector(std::vector<char> input);
private:
	//BaudRate baudRate;
	OperationMode operationMode;
	unsigned int guardTime;
	SerialPort serialPort;
};

void timeDelay(double delayTime);
#endif // !XBEE_H_
