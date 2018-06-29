/*
*	Author: Stewart Nash
*	Date:
*	File: xbeemesh.cpp
*	Description: XBee mesh, source
*
*/
#include "xbeemesh.h"
#include <iostream>
#include <vector>
#include "vectorstring.h"

XBeeMesh::XBeeMesh()
{
	isPaused = false;
	openPortRts("/dev/ttymxc2", 9600);
}

void XBeeMesh::apiModeEntry()
{
	enterApiMode();
}

void XBeeMesh::apiModeExit()
{
	exitApiMode();
}

bool XBeeMesh::getIsPaused()
{
	return isPaused;
}

std::vector<char> XBeeMesh::getData(double duration)
{
	std::vector<char> output;

	//enterTransparentMode();
	output = readReply(duration);

	return output;
}

std::string XBeeMesh::getDestinationHigh()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATDH\r"); // Get destination address high
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

std::string XBeeMesh::getDestinationLow()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATDL\r"); // Get destination address low
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

std::string XBeeMesh::getNetworkId()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATID\r"); // Get network ID
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

std::string XBeeMesh::getNodeIdentifier()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATNI\r"); // Get node identifier
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

std::string XBeeMesh::getOperatingChannel()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATCM\r"); // Get operating channel
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

std::string XBeeMesh::getSerialNumberHigh()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATSH\r"); // Get serial number high
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

std::string XBeeMesh::getSerialNumberLow()
{
	std::vector<char> output;

	enterCommandMode();
	writeCommand("ATSL\r"); // Get serial number low
	exitCommandMode();
	output = readReply();

	return vectorToString(output);
}

void XBeeMesh::pauseMesh()
{
	startRts();
	isPaused = true;
}

void XBeeMesh::unpauseMesh()
{
	stopRts();
	isPaused = false;
}

std::vector<char> XBeeMesh::receiveData(double duration)
{
	return getData(duration);
}

void XBeeMesh::sendData(std::vector<char> input)
{
	/*
	std::cout << "in XBeeMesh::sendData(std::vector<char>)" << std::endl;
	enterTransparentMode();
	writeVector(input);
	*/
	std::cout << "in void XBeeMesh::sendData(std::vector<char>)";
	std::cout << std::endl;
	displayHexadecimal(input);
	std::string output;
	output = vectorToString(input);
	sendData(output);
}

void XBeeMesh::sendData(std::string input)
{
	std::cout << "in void XBeeMesh::sendData(std::string)";
	std::cout << std::endl;
	std::cout << input << std::endl;
	enterTransparentMode();
	writeString(input);
}

void XBeeMesh::sendDataTranslation(std::vector<char> input)
{
	int length;
	std::vector<char> packet;
	std::vector<char> output;
	std::vector<char> temporary;
	char nibbleLow;
	char nibbleHigh;
	std::string s;
	
	std::cout << "in void XBeeMesh::sendDataTranslation(std::vector<char>)";
	std::cout << std::endl;
	length = input.size();
	for (int i = 0; i < length; i++) {
		packet.push_back(static_cast<char>(input[i]));
	}
	for (int i = 0; i < length; i++) {
		temporary = spellOutCharacter(packet[i]);
		nibbleLow = temporary[1];
		nibbleHigh = temporary[0];
		output.push_back(nibbleLow);
		output.push_back(nibbleHigh);
	}
	displayHexadecimal(input);
	s = vectorToString(output);
	sendData(s);
}

void XBeeMesh::setDestinationHigh(std::string input)
{
	enterCommandMode();
	writeCommand("ATDH"); // Set destination address high
	writeCommand(input);
	writeCommand("\r");
	exitCommandMode();
}

void XBeeMesh::setDestinationLow(std::string input)
{
	enterCommandMode();
	writeCommand("ATDL"); // Set destination address low
	writeCommand(input);
	writeCommand("\r");
	exitCommandMode();
}

void XBeeMesh::setNetworkId(std::string input)
{
	enterCommandMode();
	writeCommand("ATID"); // Set network ID
	writeCommand(input);
	writeCommand("\r");
	exitCommandMode();
}

void XBeeMesh::setNodeIdentifier(std::string input)
{
	enterCommandMode();
	writeCommand("ATNI"); // Set node identifier
	writeCommand(input);
	writeCommand("\r");
	exitCommandMode();
}

void XBeeMesh::setOperatingChannel(std::string input)
{
	enterCommandMode();
	writeCommand("ATCH"); // Set operating channel
	writeCommand(input);
	writeCommand("\r");
	exitCommandMode();
}
