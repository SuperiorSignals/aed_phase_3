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

class CellTxPacket {
public:
	const char START = 0x7E;
	const char FRAME_TYPE = 0x20;
	const char TRANSMIT_OPT = 0;
	const char TX_PROTOCOL = 0;

	CellTxPacket();
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

enum RxPacketType { INVALID_RESPONSE, AT_RESPONSE, SMS_RESPONSE };

class CellRxPacket {
public:
	CellRxPacket(std::vector<char> initial);
	RxPacketType categorize();
	std::vector<char> getPacket();
	void setPacket(std::vector<char> input);

	int getLength();
	char getFrameType();
	char getFrameId();
	std::vector<char> getAtCommand();
	char getStatus();
	std::vector<char> getParameterValue();
	char getCheckSum();
	std::vector<char> getPhoneNumber();
	std::vector<char> getMessage();

	const int DATA_SIZE = 512;
	const char START_DEL = 0x7E;

	//Frame Types
	const char RECEIVE_SMS = 0x9F;
	const char RECEIVE_AT = 0x88;

private:
	void parseRxPacket();
	bool validateChecksum();

	void parseAt();
	void parseSms();
	bool verify(); // Verify start delimiter, length, and checksum

	std::vector<char> packet;
	char lengthMsb;
	char lengthLsb;
	char frameType;
	char frameId;
	std::vector<char> atCommand;
	char status;
	std::vector<char> parameterValue;
	char checkSum;
	std::vector<char> phoneNumber;
	std::vector<char> message;
};

class MeshRxPacket {
public:
	MeshRxPacket(std::vector<char> initial);
	std::vector<char> getAddress();
	int getLength();
	std::vector<char> getPacket();
	char getReceiveOptions();
	std::vector<char> getData();
	void setPacket(std::vector<char> input);
	void parseRxPacket();
	bool verify();

private:
	const char START_DEL = 0x7E;
	const int DATA_SIZE = 512;
	//Frame Types
	const char RX_FRAME_TYPE = 0x90;
	const char RESERVED_MSB = 0xFF;
	const char RESERVED_LSB = 0xFE;

	std::vector<char> packetData;
	char lengthMsb;
	char lengthLsb;
	std::vector<char> address;
	char receiveOptions;
	std::vector<char> data;
	std::vector<char> packet;

	bool validateChecksum();


};

class MeshTxPacket {
public:
	MeshTxPacket();
	std::vector<char> getPacket();

	std::vector<char> getAtCommand();
	char getBroadcastRadius();
	std::vector<char> getDestinationAddress();
	char getFrameId();
	int getLength();
	std::vector<char> getData();


	void setAtCommand(std::vector<char> input);
	void setBroadcastRadius(char input);
	void setDestinationAddress(std::vector<char> input);
	void setFrameId(char input);
	void setLength(int input);
	void setData(std::vector<char> input);

	int calculatePreLength();

private:
	void assemblePrePacket();
	void assembleTxRequest();

	//AT Variables
	char frameId;
	std::vector<char> atCommand;
	std::vector<char> data;
	//TX Request Variables
	std::vector<char> destinationAddress;
	char broadcastRadius;
	char lengthMsb;
	char lengthLsb;
	char transmitOptions;

	std::vector<char> packet;

	const char START_DEL = 0x7E;
	const char AT_FRAME_TYPE = 0x08;
	const char TX_FRAME_TYPE = 0x10;

	const char RESERVED_MSB = 0xFF;
	const char RESERVED_LSB = 0xFE;
};

class MeshAtPacket {
public:
	std::vector<char> getPacket();

	std::vector<char> getAtCommand();
	std::vector<char> getDestinationAddress();
	char getFrameId();
	int getLength();
	std::vector<char> getParameter();
	
	void setAtCommand(std::vector<char> input);
	void setDestinationAddress(std::vector<char> input);
	void setFrameId(char input);
	void setLength(int input);
	void setParameter(std::vector<char> input);

	int calculatePreLength();

private:
	void assemblePrePacket();
	void assembleAt();
	
	//AT Variables
	char frameId;
	std::vector<char> atCommand;
	std::vector<char> parameter;
	//TX Request Variables
	std::vector<char> destinationAddress;
	char lengthMsb;
	char lengthLsb;

	std::vector<char> packet;

	const char START_DEL = 0x7E;
	const char AT_FRAME_TYPE = 0x08;
	const char TX_FRAME_TYPE = 0x10;

	const char RESERVED_MSB = 0xFF;
	const char RESERVED_LSB = 0xFE;
};

#endif // !APIPACKET_H_

