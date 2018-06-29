#include <vector>

#include "apipacket.h"
//  Assembly For TX and AT For XBEE
MeshTxPacket::MeshTxPacket()
{
	transmitOptions = 0x01;
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
	lengthMsb = input & 0x00F0;
	lengthLsb = input & 0x000F;
}
void MeshTxPacket::setData(std::vector<char> input) { data = input; }

std::vector<char> MeshTxPacket::getPacket()
{

	assemblePrePacket();
	assembleTxRequest();
    
    return packet; 
}

std::vector<char> MeshAtPacket::getPacket()
{
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
char MeshAtPacket::getBroadcastRadius() { return broadcastRadius; }
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
void MeshAtPacket::setBroadcastRadius(char input) { broadcastRadius = input; }
void MeshAtPacket::setDestinationAddress(std::vector<char> input) { destinationAddress = input; }
void MeshAtPacket::setFrameId(char input) { frameId = input; }
void MeshAtPacket::setLength(int input)
{
	lengthMsb = input & 0x00F0;
	lengthLsb = input & 0x000F;
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
	packet.insert(packet.end(), parameter.begin(), parameter.end());
	length = calculatePreLength();
	setLength(length);
	packet[1] = lengthMsb;
	packet[2] = lengthLsb;
	c = generateApiChecksum(packet);
	packet.push_back(c);
}


