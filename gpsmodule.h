/*
*	Author: Stewart Nash
*	Date: 3/22/2018
*	File: gpsmodule.h
*	Description: GPS module header
*/
#pragma once
#ifndef GPSMODULE_H_
#define GPSMODULE_H_
#include "serialport.h"
#include <iostream>
#include <vector>

#define MAX_GPS_READ_TIME 3.0

std::vector<char> test_gpsmodule_1();

class GpsModule {
private:
	SerialPort serialPort;
public:
	static std::streampos gpsFilePosition;
	GpsModule();
	GpsModule(const char *portNumber, int baudRate);
	void reportStream();
	std::vector<char> getData();
	std::vector<char> getData(double duration);
};

#endif // !GPSMODULE_H_
