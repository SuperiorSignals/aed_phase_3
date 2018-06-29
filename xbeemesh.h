/*
*	Author: Stewart Nash
*	Date:
*	File: xbeemesh.h
*	Description: XBee mesh, header
*
*/
#pragma once
#ifndef XBEEMESH_H_
#define XBEEMESH_H_
#include "xbee.h"
#include <string>
#include <vector>

class XBeeMesh : private XBee {
private:
	bool isPaused;
public:
	XBeeMesh();
	void apiModeEntry();
	void apiModeExit();
	bool getIsPaused();
	std::vector<char> getData(double duration);
	std::string getDestinationHigh();
	std::string getDestinationLow();
	std::string getNetworkId();
	std::string getNodeIdentifier();
	std::string getOperatingChannel();
	std::string getSerialNumberHigh();
	std::string getSerialNumberLow();
	void pauseMesh();
	void unpauseMesh();
	std::vector<char> receiveData(double duration);
	void sendData(std::vector<char> input);
	void sendData(std::string input);
	void sendDataTranslation(std::vector<char> input);
	void setDestinationHigh(std::string input);
	void setDestinationLow(std::string input);
	void setNetworkId(std::string input);
	void setNodeIdentifier(std::string input);
	void setOperatingChannel(std::string input);
};

#endif // !XBEEMESH_H_
