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
void CellTxPacket::setDestinationAddress(std::vector<char> a) { destinationAddress = a; }
void CellTxPacket::setDestinationPort(std::vector<char> d) { destinationPort = d; }
void CellTxPacket::setFrameId(const char x) { frameId = x; }
void CellTxPacket::setPayLoad(std::vector<char> p) { payLoad = p; }
void CellTxPacket::setSourcePort(std::vector<char> s) { sourcePort = s; }
void CellTxPacket::setTxPacket(std::vector<char> input) { txPacket = input; }

//pass a std::vector and set as equal to existing member variable
char CellTxPacket::getFrameId() { return frameId; }
std::vector<char> CellTxPacket::getDestinationAddress() { return destinationAddress; }
std::vector<char> CellTxPacket::getDestinationPort() { return destinationPort; }
std::vector<char> CellTxPacket::getPayload() { return payLoad; }
std::vector<char> CellTxPacket::getSourcePort() { return sourcePort; }
std::vector<char> CellTxPacket::getTxPacket() { return txPacket; }

int accessory_main()
{
    std::vector<char> Address;
    std::vector<char> Destination;
    std::vector<char> Source;
    std::vector<char> Payload;    
    CellTxPacket MyPacket;    

	std::cout << "Starting TX Request" << std::endl;
    MyPacket.setFrameId(1);
    MyPacket.setDestinationAddress(Address);
    MyPacket.setDestinationPort(Destination);
    MyPacket.setSourcePort(Source);
    MyPacket.setPayLoad(Payload);    
    MyPacket.buildPacket();    
    
    return 0; 
}

CellTxPacket::CellTxPacket()
{
	frameId = 1;
}
    
void CellTxPacket::buildPacket()
{
	char c;
	int length;
	int temporary;
	char upperByte;
	char lowerByte;
	
	length = 1 + 1 + 4 + 2 + 2 + 1 + 1 + payLoad.size();
	temporary = 0xFF00;
	temporary = temporary & length;
	temporary = temporary >> 8;
	upperByte = temporary;
	temporary = 0x00FF;
	temporary = temporary & length;
	lowerByte = temporary;

txPacket.push_back(START);
txPacket.push_back(upperByte);
txPacket.push_back(lowerByte);
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
	for (i = 0; i < (length - 3); i++) {
		j += input[i + 3];
	}
	j = j & 0x00FF;
	output = j;
	output = 0xFF - output;

	return output;
}


// Author: Dane Rodriguez
// Packet Parser Program (PPP)

CellRxPacket::CellRxPacket(std::vector<char> initial)
{
	packet = initial;
	parseRxPacket();
}

void CellRxPacket::parseRxPacket()
{
	if (this->verify()) {
		switch (this->categorize()) {
		case INVALID_RESPONSE:
			std::cout << "Error [void CellRxPacket::parseRxPacket()]: ";
			std::cout << "unknown packet type" << std::endl;
			break;
		case AT_RESPONSE:
			this->parseAt();
			//ValidateChecksum;
			//DisplayAT();
			break;
		case SMS_RESPONSE:
			this->parseSms();
			//DisplaySMS();
			break;
		}
	} else {
		std::cout << "ERROR [void CellRxPacket::parseRxPacket()]: ";
		std::cout << "failed to verify packet." << std::endl;
	}
}

RxPacketType CellRxPacket::categorize()
{
	frameType = packet[3];

	if (frameType == RECEIVE_AT) {
		return AT_RESPONSE;
	}
	if (frameType == RECEIVE_SMS) {
		return SMS_RESPONSE;
	}

	return INVALID_RESPONSE;
}

std::vector<char> CellRxPacket::getPacket()
{
	return packet;
}

void CellRxPacket::parseAt()
{
	if (packet.size() > 8) {
		frameId = packet[4];
		atCommand.clear();
		atCommand.push_back(packet[5]);
		atCommand.push_back(packet[6]);
		status = packet[7];
		for (int i = 8; i < (packet.size() - 1); i++) {
			parameterValue.push_back(packet[i]);
		}
	} else {
		std::cout << "ERROR [void RxPacket::parseAt()]: invalid packet length";
		std::cout << std::endl;
	}
}

void CellRxPacket::parseSms()
{
	if (packet.size() > 23) {
		phoneNumber.clear();
		for (int i = 4; i < 24; i++) {
			phoneNumber.push_back(packet[i]);
		}
		message.clear();
		for (int i = 24; i < (packet.size() - 1); i++) {
			message.push_back(packet[i]);
		}
	} else {
		std::cout << "ERROR [void RxPacket::parseSms()]: invalid packet length";
		std::cout << std::endl;
	}
}

void CellRxPacket::setPacket(std::vector<char> input)
{
	packet = input;
	parseRxPacket();
}

bool CellRxPacket::verify()
{
	int length;

	if (packet[0] == START_DEL) {
		length = packet[1];
		length = length << 8;
		length += packet[2];
		if (length == this->getLength()) {
			if (this->validateChecksum()) {
				return true;
			} else {
				std::cout << "ERROR [bool CellRxPacket::verify()]: ";
				std::cout << "invalid checksum" << std::endl;
			}
		} else {
			std::cout << "ERROR [bool CellRxPacket::verify()]: ";
			std::cout << "invalid length " << length;
			std::cout << " vs. " << this->getLength() << std::endl;
		}
	} else {
		std::cout << "ERROR [bool CellRxPacket::verify()]: ";
		std::cout << "invalid start delimiter" << std::endl;
	}
	
	return false;
}

/*
bool CellRxPacket::validateChecksum()
{
	std::vector<char> temporary;
	char checkValue;

	temporary = packet;
	temporary.pop_back();
	checkValue = generateApiChecksum(temporary);
	if (checkValue == checkSum) {
		return true;
	}

	return false;
}
*/

bool CellRxPacket::validateChecksum()
{
	unsigned int temporary;

	temporary = 0;
	if (packet.size()  > 3) {
		for (int i = 3; i < packet.size(); i++)	{
			temporary += packet[i];
		}
		temporary &= 0x00FF;
		if (temporary == 0xFF) {
			return true;
		}
	}

	return false;
 }

int CellRxPacket::getLength()
{
	int output;

	output = packet.size() - 4;

	return output; 
}

char CellRxPacket::getFrameType() { return frameType; }
char CellRxPacket::getFrameId() { return frameId; }
std::vector<char> CellRxPacket::getAtCommand() { return atCommand; }
char CellRxPacket::getStatus() { return status; }
std::vector<char> CellRxPacket::getParameterValue() { return parameterValue; }
char CellRxPacket::getCheckSum() { return checkSum; }
std::vector<char> CellRxPacket::getPhoneNumber() { return phoneNumber; }
std::vector<char> CellRxPacket::getMessage() { return message; }

//DIGIMESH Packet Retrieval

MeshRxPacket::MeshRxPacket(std::vector<char> initial)
{
	packet = initial;
	parseRxPacket();
}

RxPacketType MeshRxPacket::categorize()
{
	frameType = packet[3];

	if (frameType == RECEIVE_AT) {
		return AT_RESPONSE;
	}
	if (frameType == RECEIVE_TXSTATUS) {
		return TX_RESPONSE;
	}

	return INVALID_RESPONSE;
}

std::vector<char> MeshRxPacket::getAddress() { return address; }
std::vector<char> MeshRxPacket::getData() { return data; }
std::vector<char> MeshRxPacket::getPacket() { return packet; }
char MeshRxPacket::getReceiveOptions() { return receiveOptions; }

void MeshRxPacket::setPacket(std::vector<char> input)
{
	packet = input;
	parseRxPacket();
}

bool MeshRxPacket::verify()
{
	int length;

	if (packet[0] == START_DEL) {
		if (packet[3] == RX_FRAME_TYPE) {
			length = packet[1];
length = length << 8;
length += packet[2];
if (length == this->getLength()) {
	if (this->validateChecksum()) {
		return true;
	} else {
		std::cout << "ERROR [bool CellRxPacket::verify()]: ";
		std::cout << "invalid checksum" << std::endl;
	}
} else {
	std::cout << "ERROR [bool CellRxPacket::verify()]: ";
	std::cout << "invalid length " << length;
	std::cout << " vs. " << this->getLength() << std::endl;
}
		} else {
		std::cout << "WARNING [bool MeshRxPacket::verify()]: ";
		std::cout << "wrong packet type" << std::endl;
		}
	} else {
	std::cout << "ERROR [bool CellRxPacket::verify()]: ";
	std::cout << "invalid start delimiter" << std::endl;
	}

	return false;
}

void MeshRxPacket::parseRxPacket()
{
	if (this->verify()) {
		// Make sure this isn't backwards?
		if (packet.size() > 14) {
			for (int i = 4; i < 12; i++) {
				address.push_back(packet[i]);
			}
			receiveOptions = packet[13];
			for (int i = 14; i < (packet.size() - 1); i++) {
				data.push_back(packet[i]);
			}
		} else {
			std::cout << "ERROR [void MeshRxPacket::parseRxPacket()]: ";
			std::cout << "invalid packet length" << std::endl;
		}
	} else {
		std::cout << "ERROR [void MeshRxPacket::parseRxPacket()]: ";
		std::cout << "unable to parse packet" << std::endl;
	}

}

bool MeshRxPacket::validateChecksum()
{
	unsigned int temporary;

	temporary = 0;
	if (packet.size() > 3) {
		for (int i = 3; i < packet.size(); i++) {
			temporary += packet[i];
		}
		temporary &= 0x00FF;
		if (temporary == 0xFF) {
			return true;
		}
	}

	return false;
}

int MeshRxPacket::getLength()
{
	int output;

	output = packet.size() - 4;

	return output;
}

char MeshRxPacket::getTransmitRetryCount()
{
	if (categorize() == TX_RESPONSE) {
		if (data.size() > 6) {
			return data[4];
		}
		std::cout << "Error [char MeshRxPacket::getTransmitRetryCount()]: Data size too small.";
		std::cout << std::endl;
	} else {
		std::cout << "Error [char MeshRxPacket::getTransmitRetryCount()]: Packet type is not TX_STATUS.";
		std::cout << std::endl;
	}

	return 0;
}

char MeshRxPacket::getDeliveryStatus()
{
	if (categorize() == TX_RESPONSE) {
		if (data.size() > 6) {
			return data[5];
		}
		std::cout << "Error [char MeshRxPacket::getDeliveryStatus()]: Data size too small.";
		std::cout << std::endl;
	} else {
		std::cout << "Error [char MeshRxPacket::getDeliveryStatus()]: Packet type is not TX_STATUS.";
		std::cout << std::endl;
	}

	return 0;
}

char MeshRxPacket::getDiscoveryStatus()
{
	if (categorize() == TX_RESPONSE) {
		if (data.size() > 6) {
			return data[6];
		}
		std::cout << "Error [char MeshRxPacket::getDiscoveryStatus()]: Data size too small.";
		std::cout << std::endl;
	} else {
		std::cout << "Error [char MeshRxPacket::getDiscoveryStatus()]: Packet type is not TX_STATUS.";
		std::cout << std::endl;
	}

	return 0;
}

//  Assembly For TX and AT For XBEE
MeshTxPacket::MeshTxPacket()
{
	broadcastRadius = 0x00;
	transmitOptions = 0x01;
	for (int i = 0; i < 6; i++) {
		destinationAddress.push_back(0x00);
	}
	destinationAddress.push_back(0xFF);
	destinationAddress.push_back(0xFF);
	if (destinationAddress.size() != 8) {
		std::cout << "Error [MeshTxPacket::MeshTxPacket()]: ";
		std::cout << "destination address size " << destinationAddress.size();
		std::cout << std::endl;
	}
}


std::vector<char> MeshTxPacket::getAtCommand() { return atCommand; }
char MeshTxPacket::getBroadcastRadius() { return broadcastRadius; }
std::vector<char> MeshTxPacket::getDestinationAddress() { return destinationAddress; }
char MeshTxPacket::getFrameId() { return frameId; }
int MeshTxPacket::getLength()
{
	unsigned int output;

	output = lengthMsb;
	output = output << 8;
	output += lengthLsb;

	return static_cast<int>(output);
}
std::vector<char> MeshTxPacket::getData() { return data; }

int MeshTxPacket::calculatePreLength()
{
	return packet.size() - 3;
}

void MeshTxPacket::setAtCommand(std::vector<char> input) { atCommand = input; }
void MeshTxPacket::setBroadcastRadius(char input) { broadcastRadius = input; }
void MeshTxPacket::setDestinationAddress(std::vector<char> input) { destinationAddress = input; }
void MeshTxPacket::setFrameId(char input) { frameId = input; }
void MeshTxPacket::setLength(int input)
{
	lengthMsb = input & 0xFF00;
	lengthLsb = input & 0x00FF;
}
void MeshTxPacket::setData(std::vector<char> input) { data = input; }

std::vector<char> MeshTxPacket::getPacket()
{
	packet.clear();
	assemblePrePacket();
	assembleTxRequest();

	return packet;
}

std::vector<char> MeshAtPacket::getPacket()
{
	packet.clear();
	assemblePrePacket();
	assembleAt();

	return packet;
}

void MeshTxPacket::assemblePrePacket()
{
	packet.push_back(START_DEL);
	packet.push_back(lengthMsb);
	packet.push_back(lengthLsb);
	packet.push_back(TX_FRAME_TYPE);
	packet.push_back(frameId);
}

void MeshTxPacket::assembleTxRequest()
{
	char c;
	int length;

	packet.insert(packet.end(), destinationAddress.begin(), destinationAddress.end());
	packet.push_back(RESERVED_MSB);
	packet.push_back(RESERVED_LSB);
	packet.push_back(broadcastRadius);
	packet.push_back(transmitOptions);
	packet.insert(packet.end(), data.begin(), data.end());
	length = calculatePreLength();
	setLength(length);
	packet[1] = lengthMsb;
	packet[2] = lengthLsb;
	c = generateApiChecksum(packet);
	packet.push_back(c);
}

std::vector<char> MeshAtPacket::getAtCommand() { return atCommand; }
std::vector<char> MeshAtPacket::getDestinationAddress() { return destinationAddress; }
char MeshAtPacket::getFrameId() { return frameId; }
int MeshAtPacket::getLength()
{
	unsigned int output;

	output = lengthMsb;
	output = output << 8;
	output += lengthLsb;

	return static_cast<int>(output);
}
std::vector<char> MeshAtPacket::getParameter() { return parameter; }

int MeshAtPacket::calculatePreLength()
{
	return packet.size() - 3;
}

void MeshAtPacket::setAtCommand(std::vector<char> input) { atCommand = input; }
void MeshAtPacket::setDestinationAddress(std::vector<char> input) { destinationAddress = input; }
void MeshAtPacket::setFrameId(char input) { frameId = input; }

void MeshAtPacket::setLength(int input)
{
	lengthMsb = input & 0xFF00;
	lengthLsb = input & 0x00FF;
}

void MeshAtPacket::setParameter(std::vector<char> input) { parameter = input; }

void MeshAtPacket::assemblePrePacket()
{
	packet.push_back(START_DEL);
	packet.push_back(lengthMsb);
	packet.push_back(lengthLsb);
	packet.push_back(AT_FRAME_TYPE);
	packet.push_back(frameId);
}

void MeshAtPacket::assembleAt()
{
	char c;
	int length;

	packet.insert(packet.end(), atCommand.begin(), atCommand.end());
	if (parameter.size() > 0) {
		packet.insert(packet.end(), parameter.begin(), parameter.end());
	}
	length = calculatePreLength();
	setLength(length);
	packet[1] = lengthMsb;
	packet[2] = lengthLsb;
	c = generateApiChecksum(packet);
	packet.push_back(c);
}
