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

char generateApiChecksum(std::vector<char> input);

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


// Author: Dane Rodriguez
const int ARBITRARY_MAX_DATA = 512;

class TxrqPacket {
public:
	const char START = 0x7E;
	const char FRAME_TYPE = 0x20;
	const char TRANSMIT_OPT = 0;
	const char TX_PROTOCOL = 0;

	TxrqPacket();
	//Assign Nonconstant Variables with outside Data
	void setDestinationAddress(std::vector<char> a);
	void setDestinationPort(std::vector<char> d);
	void setFrameId(const char x);
	void setPayLoad(std::vector<char> p);
	void setSourcePort(std::vector<char> s);
	void setTxPacket(std::vector<char> input);

	//pass a std::vector and set as equal to existing member variable
	char getFrameId();
	std::vector<char> getDestinationAddress();
	std::vector<char> getDestinationPort();
	std::vector<char> getPayload();
	std::vector<char> getSourcePort();
	std::vector<char> getTxPacket();

	//Takes all prepared data and assembles Packet
	void buildPacket();

private:
	char frameId;
	std::vector<char> destinationAddress;
	std::vector<char> destinationPort;
	std::vector<char> sourcePort;
	std::vector<char> payLoad;
	std::vector<char> txPacket;    //Raw, no checksum
};

// Author: Dane Rodriguez

void parseRxPacket(RxPacket input);

enum RxPacketType { INVALID_RESPONSE, AT_RESPONSE, SMS_RESPONSE };

class RxPacket {
public:
	 RxPacketType categorize(std::vector<char> &P, char &FT);
	std::vector<char> getPacket();
	void parseAt(std::vector<char> &P, char SFID, char ATCMMD, char STS, char PV);
	void parseSms(std::vector<char> &P, char PN[], char MSSGE[]);
	void setPacket(std::vector<char> input);
	bool verify(std::vector<char> &P, char &LSB, char &MSB);

private:
	std::vector<char> packet;
};

#endif // !APIPACKET_H_

