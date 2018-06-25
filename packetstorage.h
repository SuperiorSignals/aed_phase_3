#pragma once
#ifndef PACKETSTORAGE_H_
#define PACKETSTORAGE_H_
#include <vector>
#include "datapacket.h"

bool comparePacket(char *buffer, int indices[4]);
bool comparePacket(std::vector<char> first, std::vector<char> second);
void removeDuplicate(char *buffer, std::vector<long>& indices, std::vector<std::string>& index, int duplicates[2]);

class PacketStorage {
public:
	//int getPacketCount(PacketType type);
	std::vector<char> popPacket(PacketType type);
	void pushPacket(std::vector<char> input, PacketType type);
	void resetPacketStorage(PacketType type);
	//std::vector<char> retrievePacket(int index, PacketType type);
	//void storePacket(std::vector<char> input, PacketType type);

private:
	std::vector<char> packetPopper(const char *indexInput, const char *dataInput);
	void packetPusher(std::vector<char> input, const char *indexInput, const char *dataInput);
	void removeMeshDuplicates();
	void resetStorage(const char *indexInput, const char *dataInput);
};


#endif // !PACKETSTORAGE_H_
