/*
*	Author: Stewart Nash
*	Date: 3/15/2018
*	File: apipacket.cpp
*	Description: XBee api packet source
*/
#include "apipacket.h"
#include <iostream>

ApiPacket::ApiPacket()
{

}
 
ApiPacket::ApiPacket(std::vector<unsigned char> input)
{
	rawPacket = input;
}

unsigned char ApiPacket::getChecksum()
{
	int size;
	unsigned char checksum;

	size = rawPacket.size();
	if (size >= MINIMUM_PACKET_SIZE) {
		checksum = rawPacket[size - 1];
	} else {
		std::cout << "ERROR [unsigned char Apipacket::getChecksum()]: API frame has no checksum. Length is " << size << "." << std::endl;
	}

	return checksum;
}

void ApiPacket::setChecksum(unsigned char checksum)
{
	int size;

	size = rawPacket.size();
	if (size >= MINIMUM_PACKET_SIZE) {
		rawPacket[size - 1] = checksum;
	} else {
		std::cout << "ERROR [void Apipacket::setChecksum(unsigned char checksum)]: API frame has no checksum. Size is " << size << "." << std::endl;
	}
}

unsigned short ApiPacket::getLength()
{
	int size;
	unsigned short length;

	length = 0;
	size = rawPacket.size();
	if (size > MINIMUM_PACKET_SIZE) {
		length += static_cast<unsigned short>(rawPacket[PACKET_LENGTH_HIGH]);
		length = length << 8;
		length += static_cast<unsigned short>(rawPacket[PACKET_LENGTH_LOW]);
	} else {
		std::cout << "ERROR [unsigned short Apipacket::getLength()]: API frame has no length. Size is " << size << "." << std::endl;
	}

	return length;
}

void ApiPacket::setLength(unsigned short length)
{
	int size;

	size = rawPacket.size();
	if (size > MINIMUM_PACKET_SIZE) {
		rawPacket[PACKET_LENGTH_HIGH] = static_cast<unsigned char>((length >> 8) & 0xFF);
		rawPacket[PACKET_LENGTH_LOW] = static_cast<unsigned char>(length & 0xFF);
	} else {
		std::cout << "ERROR [void Apipacket::setLength()]: API frame has no length. Size is " << size << "." << std::endl;
	}
}

void ApiPacket::setRawPacket(std::vector<unsigned char> input)
{
	rawPacket = input;
}

void ApiPacket::setRawPacket(char *input, int length)
{
	rawPacket.clear();

	for (int i = 0; i < length; i++) {
		rawPacket.push_back(static_cast<unsigned char>(input[i]));
	}
}

std::vector<unsigned char> ApiPacket::getRawPacket()
{
	return rawPacket; // This should return a copy and not the member?
}

unsigned char ApiPacket::getStartDelimiter()
{
	int size;
	unsigned char startDelimiter;

	size = rawPacket.size();
	if (size > 0) {
		startDelimiter = rawPacket[0];
	} else {
		startDelimiter = 0;
		std::cout << "ERROR [unsigned char Apipacket::getStartDelimiter()]: API frame is empty." << std::endl;
	}

	return startDelimiter;
}

void ApiPacket::setStartDelimiter(unsigned char startDelimiter)
{

}

unsigned char ApiPacket::generateValidChecksum()
{
	int size;
	unsigned char checksum;

	checksum = 0;
	size = rawPacket.size();
	if (size > MINIMUM_PACKET_SIZE) {
		for (int i = 0; i < size - 1; i++) {
			checksum += rawPacket[i];
		}
		checksum = 0xFF - checksum;
	} else {
		checksum = 0;
		std::cout << "ERROR [void Apipacket::generateValidChecksum()]: API frame data size is zero." << std::endl;
	}

	return checksum;
}

unsigned short ApiPacket::generateValidLength()
{
	unsigned short length;
	int size;

	size = rawPacket.size();
	length = static_cast<unsigned short>(size);
	if (length > MINIMUM_PACKET_SIZE) {
		length = length - PACKET_HEADER_SIZE;
	} else {
		// Should you report an error, or is 0 length sufficient?
		length = 0;
	}

	return length;
}

bool ApiPacket::isChecksumValid()
{
	int size;
	unsigned char checksum;

	checksum = 0;
	size = rawPacket.size();
	if (size > 3) {
		for (int i = 0; i < size; i++) {
			checksum += rawPacket[i];
		}
	} else {
		std::cout << "ERROR [bool ApiPacket::isChecksumValid()]: API frame data size is zero and no checksum is present." << std::endl;
	}

	if (checksum == 0xFF) {
		return true;
	} //else
	return false;
}

bool ApiPacket::isLengthValid()
{
	int size;
	unsigned short lengthOne, lengthTwo;

	lengthOne = 0;
	size = rawPacket.size();
	if (size > MINIMUM_PACKET_SIZE) {
		lengthOne += static_cast<unsigned short>(rawPacket[PACKET_LENGTH_HIGH]);
		lengthOne = lengthOne << 8;
		lengthOne += static_cast<unsigned short>(rawPacket[PACKET_LENGTH_LOW]);
	} else {
		// Should you report an error, or is false sufficient?
		return false;
	}

	size = rawPacket.size();
	lengthTwo = static_cast<unsigned short>(size);
	if (lengthTwo > MINIMUM_PACKET_SIZE) {
		lengthTwo = lengthTwo - PACKET_HEADER_SIZE;
	} else {
		// Should you report an error, or is false sufficient?
		return false;
	}

	if (lengthOne == lengthTwo) {
		return true;
	} //else
	return false;
}

void ApiPacket::replaceInvalidChecksum()
{
	int size;
	unsigned char checksum;

	checksum = 0;
	size = rawPacket.size();
	if (size > MINIMUM_PACKET_SIZE) {
		for (int i = 0; i < size - 1; i++) {
			checksum += rawPacket[i];
		}
		checksum = 0xFF - checksum;
		rawPacket[size - 1] = checksum;
	} else {
		std::cout << "ERROR [void Apipacket::replaceInvalidChecksum()]: API frame data size is zero." << std::endl;
	}
}

void ApiPacket::replaceInvalidLength()
{
	unsigned short length;
	int size;

	size = rawPacket.size();
	length = static_cast<unsigned short>(size);
	if (length > MINIMUM_PACKET_SIZE) {
		length = length - PACKET_HEADER_SIZE;
		rawPacket[PACKET_LENGTH_HIGH] = static_cast<unsigned char>((length >> 8) & 0xFF);
		rawPacket[PACKET_LENGTH_LOW] = static_cast<unsigned char>(length & 0xFF);
	} else {
		// Should you try to assign length if no data?
		std::cout << "ERROR [void Apipacket::replaceInvalidLength()]: API frame data size is zero." << std::endl;
	}
}


// Author: Dane Rodriguez
//Assign Nonconstant Variables with outside Data
void TxrqPacket::setDestinationAddress(std::vector<char> a) { destinationAddress = a; }
void TxrqPacket::setDestinationPort(std::vector<char> d) { destinationPort = d; }
void TxrqPacket::setFrameId(const char x) { frameId = x; }
void TxrqPacket::setPayLoad(std::vector<char> p) { payLoad = p; }
void TxrqPacket::setSourcePort(std::vector<char> s) { sourcePort = s; }
void TxrqPacket::setTxPacket(std::vector<char> input) { txPacket = input; }

//pass a std::vector and set as equal to existing member variable
char TxrqPacket::getFrameId() { return frameId; }
std::vector<char> TxrqPacket::getDestinationAddress() { return destinationAddress; }
std::vector<char> TxrqPacket::getDestinationPort() { return destinationPort; }
std::vector<char> TxrqPacket::getPayload() { return payLoad; }
std::vector<char> TxrqPacket::getSourcePort() { return sourcePort; }
std::vector<char> TxrqPacket::getTxPacket() { return txPacket; }

int accessory_main()
{
    std::vector<char> Address;
    std::vector<char> Destination;
    std::vector<char> Source;
    std::vector<char> Payload;    
    TxrqPacket MyPacket;    

	std::cout << "Starting TX Request" << std::endl;
    MyPacket.setFrameId(1);
    MyPacket.setDestinationAddress(Address);
    MyPacket.setDestinationPort(Destination);
    MyPacket.setSourcePort(Source);
    MyPacket.setPayLoad(Payload);    
    MyPacket.buildPacket();    
    
    return 0; 
}

TxrqPacket::TxrqPacket()
{
	frameId = 1;
}
    
void TxrqPacket::buildPacket()
{
	char c;
	int length;
	int temporary;
	char upperNibble;
	char lowerNibble;
	
	length = 1 + 1 + 4 + 2 + 2 + 1 + 1 + payLoad.size();
	temporary = 0b11110000;
	temporary = temporary & length;
	temporary >> 4;
	upperNibble = temporary;
	temporary = 0b00001111;
	temporary = temporary & length;
	lowerNibble = temporary;

    txPacket.push_back(START);
	txPacket.push_back(upperNibble);
	txPacket.push_back(lowerNibble);
    txPacket.push_back(FRAME_TYPE);
    txPacket.push_back(frameId);
    txPacket.insert(txPacket.end(), destinationAddress.begin(), destinationAddress.end());
    txPacket.insert(txPacket.end(), destinationPort.begin(), destinationPort.end());
	txPacket.push_back(TX_PROTOCOL);
	txPacket.push_back(TRANSMIT_OPT);
    txPacket.insert(txPacket.end(), sourcePort.begin(), sourcePort.end());
    txPacket.insert(txPacket.end(), payLoad.begin(), payLoad.end());
	c = generateApiChecksum(txPacket);
	txPacket.push_back(c);
}   
    
char generateApiChecksum(std::vector<char> input)
{
	int length;
	char output;
	int i, j;

	length = input.size();
	j = 0;
	for (i = 0; i < length; i++) {
		j += input[i + 3];
	}
	j = j & 0x00FF;
	output = j;
	output = 0xFF - output;

	return output;
}