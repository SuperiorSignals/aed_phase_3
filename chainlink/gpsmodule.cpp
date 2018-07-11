/*
*	Author: Stewart Nash
*	Date: 3/22/2018
*	File: gpsmodule.cpp
*	Description: GPS module source
*/
#include "gpsmodule.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "serialport.h"

std::streampos GpsModule::gpsFilePosition = 0;

GpsModule::GpsModule()
{
	serialPort.open("/dev/ttymxc5", 9600);
}

GpsModule::GpsModule(const char *portNumber, int baudRate)
{
	serialPort.open(portNumber, baudRate);
}

void GpsModule::reportStream()
{
	int length;
	std::vector<char> buffer;

	while (1) {
		serialPort.read(buffer);
		length = buffer.size();
		for (int i = 0; i < length; i++) {
			std::cout << buffer[i];
		}
	}
}

std::vector<char> GpsModule::getData()
{
	//return test_gpsmodule_1();
	std::vector<char> output;
	serialPort.timedRead(output, MAX_GPS_READ_TIME);
	return output;
}

std::vector<char> GpsModule::getData(double duration)
{
	std::vector<char> output;
	serialPort.timedRead(output, duration);
	return output;
}

std::vector<char> test_gpsmodule_1()
{
	const int LINE_FEED_NUMBER = 10;
	std::ifstream inputFile;
	std::vector<char> output;
	int newlineCount;
	char input;
		
	inputFile.open("/opt/starlink/gps_sample.log", std::ios::in | std::ios::binary);
	if (inputFile.is_open()) {
		newlineCount = 0;
		inputFile.seekg(GpsModule::gpsFilePosition);
		while (inputFile.good() && newlineCount < LINE_FEED_NUMBER) {
			inputFile.get(input);
			output.push_back(input);
			if (input == '\n') {
				newlineCount++;
			}
		}
		GpsModule::gpsFilePosition = inputFile.tellg();
		inputFile.close();
	} else {
		std::cout << "ERROR [std::vector<char> test_gpsmodule_1()]: Unable to open file.";
		std::cout << std::endl;
	}

	return output;
}
