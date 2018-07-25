/*
*	Author: Stewart Nash
*	Date: 3/28/2018
*	File: xbeecell.h
*	Description: XBee cellular modem, header
*
*/
#pragma once
#ifndef XBEECELL_H_
#define XBEECELL_H_
#include "xbee.h"
#include <string>
#include <vector>

#define INSTRUCTION_LENGTH 5

class XBeeCell : private XBee {
public:
	XBeeCell();
	XBeeCell(const char *portNumber, int baudRate);
	void apiModeOperation();
	void apiModeExit();
	void dispatchSmsPython(std::vector<char> textMessage, std::vector<char> phoneNumber);
	void dispatchUdpAt(std::vector<char> input);
	void dispatchUdpAt(std::vector<char> input, std::string address, std::string port);
	void dispatchUdpPython(std::vector<char> input);
	void enterPythonMode();
	//std::string getConnection();
	int getConnection();
	std::string getImei();
	std::string getText();
	void setBaudRate();

private:
	void parseMessage(std::string message);
	int connection;
	//void translateMessage(std::string message);
};


#endif // !XBEECELL_H_

