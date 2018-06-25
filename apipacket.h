/*
*	Author: Stewart Nash
*	Date: 3/15/2018
*	File: apipacket.h
*	Description: XBee api packet header
*/
#pragma once
#ifndef APIPACKET_H_
#define APIPACKET_H_
#include <vector>

#define MINIMUM_PACKET_SIZE 3
#define PACKET_HEADER_SIZE 3
#define PACKET_LENGTH_HIGH 2
#define PACKET_LENGTH_LOW 3

class ApiPacket {
private:
	std::vector<unsigned char> rawPacket;
	//char startDelimiter;
	//char length[2];
	//int dataLength;
	//std::vector<unsigned char> frameData;
	//char checksum;
public:
	ApiPacket();
	ApiPacket(std::vector<unsigned char> input);
	unsigned char getChecksum();
	void setChecksum(unsigned char checksum);
	//int getDataLength();
	//void setDataLength(unsigned short input);
	unsigned short getLength();
	void setLength(unsigned short length);
	void setRawPacket(std::vector<unsigned char> input);
	void setRawPacket(char *input, int length);
	std::vector<unsigned char> getRawPacket();
	unsigned char getStartDelimiter();
	void setStartDelimiter(unsigned char startDelimiter);
	unsigned char generateValidChecksum();
	unsigned short generateValidLength();
	bool isChecksumValid();
	bool isLengthValid();
	void replaceInvalidChecksum();
	void replaceInvalidLength();
};

#endif // !APIPACKET_H_

