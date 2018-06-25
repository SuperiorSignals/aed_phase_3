/*
*	Author: Stewart Nash
*	Date: 3/20/2018
*	File: xbee.cpp
*	Description: XBee interface class source
*
*/
#include "xbee.h"
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "serialport.h"

extern "C" {
#include "api_mode.h"
}

#define DEBUG_DELAY 0.05

XBee::XBee()
{
	//baudRate = BAUD_RATE_UNDEFINED;
	operationMode = UNDEFINED_MODE;
	guardTime = 1000;
	//serialPort.open("/dev/ttymxc1", 115200);
	setOperationMode(TRANSPARENT_MODE);
	//setBaudRate(BAUD_RATE_115200);
}

/*
void XBee::setBaudRate(BaudRate input)
{
	std::vector<char> buffer;
	std::vector<unsigned char> message;

	// Add command mode check for baud rate
	switch (input) {
	case BAUD_RATE_9600:
		enterCommandMode();
		writeCommand("ATBD 3\r");	// Set baud rate to 9600
		writeCommand("ATWR\r");		// Write changes
		writeCommand("ATBD 3\r");	// Set baud rate to 9600
		writeCommand("ATAC\r");		// Apply changes
		exitCommandMode();
		baudRate = BAUD_RATE_9600;
		break;
	case BAUD_RATE_115200:
		enterCommandMode();
		writeCommand("ATBD 7\r");	// Set baud rate to 115200
		writeCommand("ATWR\r");		// Write changes
		writeCommand("ATBD 7\r");	// Set baud rate to 115200
		writeCommand("ATAC\r");		// Apply changes
		exitCommandMode();
		baudRate = BAUD_RATE_115200;
		break;
	default:
		break;
	}
}
*/

int XBee::getBaudRate() 
{
	// Add command mode check for baud rate
	return serialPort.getBaudRate();
}

void XBee::setOperationMode(OperationMode input)
{
	std::vector<char> buffer;
	std::vector<unsigned char> message;

	// Add command mode check for modes
	switch (input) {
	case UNDEFINED_MODE:
		break;
	case TRANSPARENT_MODE:
		//enterCommandMode();
		//writeCommand("ATAP 0\r");	// Operate in transparent mode
		//writeCommand("ATAC\r");	// Apply changes
		//exitCommandMode();
		operationMode = TRANSPARENT_MODE;
		break;
	case MICROPYTHON_MODE:
		//enterCommandMode();
		//writeCommand("ATTD D\r");	// Set delimiter to 0x0D (carriage return)
		//writeCommand("ATAP 4\r");	// Operate in raw repl mode
		//writeCommand("ATAC\r");		// Apply changes
		//exitCommandMode();
		operationMode = MICROPYTHON_MODE;
		break;
	case API_MODE:
		//enterCommandMode();
		//writeCommand("ATAP 1\r");	// Enable API mode
		//writeCommand("ATAC\r");		// Apply changes
		//exitCommandMode();
		operationMode = API_MODE;
		break;
	default:
		break;
	}
}

OperationMode XBee::getOperationMode()
{
	// Add command mode check for modes
	return operationMode;
}

void XBee::openPort(const char *deviceName, int baudRate)
{
	serialPort.open(deviceName, baudRate);
}

void XBee::openPortRts(const char *deviceName, int baudRate)
{
	serialPort.openRts(deviceName, baudRate);
}

void XBee::setGuardTime(unsigned int input)
{
	if (input > 1 && input < 0x577) {
		guardTime = input;
	} else {
		std::cout << "ERROR [void XBee::setGuardTime(unsigned int)]: Guard time invalid at ";
		std::cout << input << std::endl;
	}
}

unsigned int XBee::getGuardTime()
{
	// Add command mode check for guard time
	return guardTime;
}

void XBee::enterApiMode()
{
	enterCommandMode();
	writeCommand("ATAP 1\r"); // Enable API mode
	writeCommand("ATAC\r"); // Apply changes
	exitCommandMode();
	setOperationMode(API_MODE);
}

void XBee::enterTransparentMode()
{
	enterCommandMode();
	writeCommand("ATAP 0\r"); // Operate in transparent mode
	writeCommand("ATAC\r"); // Apply changes
	exitCommandMode();
	setOperationMode(TRANSPARENT_MODE);
}

void XBee::enterCommandMode()
{
	double maxTime;
	std::vector<char> buffer;
	std::vector<unsigned char> message;
	

	maxTime = static_cast<double>(guardTime) / 1000.0;
	timeDelay(maxTime);
	serialPort.writeString("+++");
	//timeDelay(maxTime);
	// The time delay was replaced with the following for debugging
	serialPort.timedRead(buffer, 1.0);
	message = convertVectorToUnsigned(buffer);
	displayHexadecimal(message);
}

void XBee::exitApiMode()
{
	char at_command[2] = { 'A','P' };
	char api_frame[API_AT_TOTAL_LENGTH];
	std::vector<char> inputBuffer;
	std::vector<unsigned char> bufferDuplicate;

	generateApiAtCommandNoParam(at_command, 0, api_frame);
	inputBuffer = convertArrayToVector(api_frame, API_AT_NO_PARAM_TOTAL_LENGTH);
	writeVector(inputBuffer);
	readReply(0.5);
}

void XBee::exitCommandMode()
{
	writeCommand("ATCN\r");
}

std::vector<char> XBee::readReply()
{
	std::vector<char> buffer;
	std::vector<unsigned char> message;

	serialPort.timedRead(buffer, DEBUG_DELAY);
	message = convertVectorToUnsigned(buffer);
	displayHexadecimal(message);

	return buffer;
}

std::vector<char> XBee::readReply(double input)
{
	std::vector<char> buffer;
	std::vector<unsigned char> message;

	serialPort.timedRead(buffer, input);
	message = convertVectorToUnsigned(buffer);
	displayHexadecimal(message);

	return buffer;
}

void XBee::startRts()
{
	serialPort.setRts();
}

void XBee::stopRts()
{
	serialPort.clearRts();
}

void XBee::writeCommand(const char *input)
{
	//std::vector<char> buffer;
	//std::vector<unsigned char> message;
	serialPort.writeString(input);
	readReply(0.25);
	//serialPort.timedRead(buffer, DEBUG_DELAY);
	//message = convertVectorToUnsigned(buffer);
	//displayHexadecimal(message);
}

void XBee::writeCommand(std::string input)
{
	writeString(input);
	readReply(0.25);
}

void XBee::writeString(const char *input)
{
	serialPort.writeString(input);
}

void XBee::writeString(std::string input)
{
	char *output = new char[input.size() + 1];

	std::copy(input.begin(), input.end(), output);
	output[input.size()] = '\0';
	serialPort.writeString(output);

	delete[] output;
}

void timeDelay(double delayTime)
{
	clock_t startTime, endTime;
	double timeDifference;

	timeDifference = 0.0;
	startTime = clock();
	do {
		endTime = clock();
		timeDifference = static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC);
	} while (timeDifference < delayTime);
}

void XBee::writeVector(std::vector<char> input)
{
	serialPort.write(input);
}
