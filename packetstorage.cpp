#include "packetstorage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define PACKET_SENT 0x1C
#define PACKET_UNSENT 0x35

/*
int PacketStorage::getPacketCount(PacketType type)
{
	std::fstream indexFile;
	std::fstream dataFile;
	std::string temporary;
	std::string::size_type sz;
	int packetCount;

	packetCount = 0;
	switch (type) {
	case NORMAL:
		indexFile.open("C:/opt/chainlink/normalpacketindex.txt");
		if (!indexFile.is_open()) {
			std::cout << "ERROR [int PacketStorage::getPacketCount(PacketType)]: ";
			std::cout << "Unable to open file C:/opt/chainlink/normalpacketindex.txt";
			std::cout << std::endl;
			break;
		}
		while (!indexFile.eof()) {
			std::getline(indexFile, temporary);
			if (temporary.size() > 0) {
				packetCount++;
			}
		}
		packetCount /= 2;
		break;
	case EVENT:
		indexFile.open("C:/opt/chainlink/eventpacketindex.txt");
		if (!indexFile.is_open()) {
			std::cout << "ERROR [int PacketStorage::getPacketCount(PacketType)]: ";
			std::cout << "Unable to open file C:/opt/chainlink/normalpacketindex.txt";
			std::cout << std::endl;
			break;
		}
		while (!indexFile.eof()) {
			std::getline(indexFile, temporary);
			if (temporary.size() > 0) {
				packetCount++;
			}
		}
		packetCount /= 2;
		break;
	case MESH:
		indexFile.open("C:/opt/chainlink/meshpacketindex.txt");
		if (!indexFile.is_open()) {
			std::cout << "ERROR [int PacketStorage::getPacketCount(PacketType)]: ";
			std::cout << "Unable to open file C:/opt/chainlink/normalpacketindex.txt";
			std::cout << std::endl;
			break;
		}
		while (!indexFile.eof()) {
			std::getline(indexFile, temporary);
			if (temporary.size() > 0) {
				packetCount++;
			}
		}
		packetCount /= 2;
		break;
	default:
		break;
	}

	dataFile.close();
	indexFile.close();

	return packetCount;
}
*/

void PacketStorage::consolidateQueuedPackets()
{
	std::vector<std::vector<char>> packets;
	std::vector<char> comparator;
	std::vector<char> temporary;
	char compSentStatus;
	char tempSentStatus;
	bool compUnsentTempSent;

	do {
		temporary = popPacket(QUEUED);
		if (temporary.size() > 0) {
			packets.push_back(temporary);
		}
	} while (temporary.size() > 0);

	for (int i = 0; i < packets.size(); i++) {
		compUnsentTempSent = false;
		comparator = packets[i];
		compSentStatus = comparator[0];
		comparator.erase(comparator.begin());
		for (int j = i + 1; j < packets.size(); j++) {
			temporary = packets[j];
			tempSentStatus = temporary[0];
			temporary.erase(temporary.begin());
			if (comparator == temporary) {
				if (compSentStatus == PACKET_SENT) {
					packets.erase(packets.begin() + j);
				} else if (tempSentStatus == PACKET_SENT) {
					compUnsentTempSent = true;
				} else {
					packets.erase(packets.begin() + j);
				}
			}
		}
		if (compUnsentTempSent) {
			packets.erase(packets.begin() + i);
		}
	}
}

std::vector<char> PacketStorage::popPacket(PacketType type)
{
	std::vector<char> output;

#ifdef WINDOWS_IMPLEMENTATION
	switch (type) {
	case NORMAL:
		output = packetPopper("normalpacketindex.txt", "normalpacketdata.txt");
		break;
	case EVENT:
		output = packetPopper("eventpacketindex.txt", "eventpacketdata.txt");
		break;
	case MESH:
		output = packetPopper("meshpacketindex.txt", "meshpacketdata.txt");
		break;
	case QUEUED:
		output = packetPopper("queuedpacketindex.txt", "queuedpacketdata.txt");
		break;
	default:
		break;
}
#else
	switch (type) {
	case NORMAL:
		output = packetPopper("/opt/chainlink/normalpacketindex.txt", "/opt/chainlink/normalpacketdata.txt");
		break;
	case EVENT:
		output = packetPopper("/opt/chainlink/eventpacketindex.txt", "/opt/chainlink/eventpacketdata.txt");
		break;
	case MESH:
		output = packetPopper("/opt/chainlink/meshpacketindex.txt", "/opt/chainlink/meshpacketdata.txt");
		break;
	case QUEUED:
		output = packetPopper("/opt/chainlink/queuedpacketindex.txt", "/opt/chainlink/queuedpacketdata.txt");
		break;
	default:
		break;
	}
#endif // WINDOWS_IMPLEMENTATION

	return output;
}

void PacketStorage::pushPacket(std::vector<char> input, PacketType type)
{
#ifdef WINDOWS_IMPLEMENTATION
	switch (type) {
	case NORMAL:
		packetPusher(input, "normalpacketindex.txt", "normalpacketdata.txt");
		break;
	case EVENT:
		packetPusher(input, "eventpacketindex.txt", "eventpacketdata.txt");
		break;
	case MESH:
		packetPusher(input, "meshpacketindex.txt", "meshpacketdata.txt");
		//removeMeshDuplicates();
		break;
	case QUEUED:
		packetPusher(input, "queuedpacketindex.txt", "queuedpacketindex.txt");
		break;
	default:
		break;
	}
#else
	switch (type) {
	case NORMAL:
		packetPusher(input, "/opt/chainlink/normalpacketindex.txt", "/opt/chainlink/normalpacketdata.txt");
		break;
	case EVENT:
		packetPusher(input, "/opt/chainlink/eventpacketindex.txt", "/opt/chainlink/eventpacketdata.txt");
		break;
	case MESH:
		packetPusher(input, "/opt/chainlink/meshpacketindex.txt", "/opt/chainlink/meshpacketdata.txt");
		removeMeshDuplicates();
		break;
	case QUEUED:
		packetPusher(input, "/opt/chainlink/queuedpacketindex.txt", "/opt/chainlink/queuedpacketdata.txt");
		removeQueuedDuplicates();
		break;
	default:
		break;
	}
#endif // WINDOWS_IMPLEMENTATION
}

/*
std::vector<char> PacketStorage::retrievePacket(int index, PacketType type)
{
	std::fstream indexFile;
	std::fstream dataFile;
	std::vector<char> output;

	switch (type) {
	case NORMAL:
		break;
	case EVENT:
		break;
	case MESH:
		break;
	case QUEUED:
		break;
	default:
		break;
	}
	
	return output;
}

void PacketStorage::storePacket(std::vector<char> input, PacketType type)
{
	switch (type) {
	case NORMAL:
		break;
	case EVENT:
		break;
	case MESH:
		break;
	case QUEUED:
		break;
	default:
		break;
	}
}
*/

void PacketStorage::resetPacketStorage(PacketType type)
{
#ifdef WINDOWS_IMPLEMENTATION
	switch (type) {
	case NORMAL:
		resetStorage("normalpacketindex.txt", "normalpacketdata.txt");
		break;
	case EVENT:
		resetStorage("eventpacketindex.txt", "eventpacketdata.txt");
		break;
	case MESH:
		resetStorage("meshpacketindex.txt", "meshpacketdata.txt");
		break;
	case QUEUED:
		resetStorage("queuedpacketindex.txt", "queuedpacketindex.txt");
		break;
	default:
		break;
	}
#else
	switch (type) {
	case NORMAL:
		resetStorage("/opt/chainlink/normalpacketindex.txt", "/opt/chainlink/normalpacketdata.txt");
		break;
	case EVENT:
		resetStorage("/opt/chainlink/eventpacketindex.txt", "/opt/chainlink/eventpacketdata.txt");
		break;
	case MESH:
		resetStorage("/opt/chainlink/meshpacketindex.txt", "/opt/chainlink/meshpacketdata.txt");
		break;
	case QUEUED:
		resetStorage("/opt/chainlink/queuedpacketindex.txt", "/opt/chainlink/queuedpacketindex.txt");
	default:
		break;
	}
#endif // WINDOWS_IMPLEMENTATION
}

std::vector<char> PacketStorage::packetPopper(const char *indexInput, const char *dataInput)
{
	char *bufferMemoryAlloc;
	char *dataMemoryAlloc;
	std::fstream indexFile;
	std::fstream dataFile;
	std::vector<char> output;
	std::vector<long> indices;
	std::vector<std::string> index;
	std::string temporary;
	std::string::size_type sz;
	long startAddress;
	long endAddress;
	long size;
	long fileSize;

	indexFile.open(indexInput, std::ios::in);
	// If cannot open, create new index file and data file
	// then exit as there is no data in these new files
	if (!indexFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::popPacket(PacketType)]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;
		std::cout << "New file will be created." << std::endl;
		indexFile.open(indexInput, std::ios::out | std::ios::trunc);
		indexFile.close();
		dataFile.open(dataInput, std::ios::out | std::ios::binary | std::ios::trunc);
		dataFile.close();
		return output;
	}
	dataFile.open(dataInput, std::ios::in | std::ios::binary);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::popPacket(PacketType)]: ";
		std::cout << "Unable to open file " << dataInput;
		std::cout << std::endl;
		std::cout << "New file will be created." << std::endl;
		dataFile.open(dataInput, std::ios::out | std::ios::binary);
		dataFile.close();
		indexFile.open(indexInput, std::ios::out | std::ios::trunc);
		indexFile.close();
		return output;
	}
	while (std::getline(indexFile, temporary)) {
		if (temporary.size() > 0) {
			index.push_back(temporary);
			indices.push_back(std::stol(temporary, &sz));
		}
	}
	if (indices.size() > 1) {
		startAddress = indices[0];
		endAddress = indices[1];
		size = endAddress - startAddress + 1;
		bufferMemoryAlloc = new char[size];
		dataFile.seekg(startAddress, std::ios::beg);
		dataFile.read(bufferMemoryAlloc, size);
		for (long i = 0; i < size; i++) {
			output.push_back(bufferMemoryAlloc[i]);
		}
		indexFile.close();
		indexFile.open(indexInput, std::ios::out | std::ios::trunc);
		if (!indexFile.is_open()) {
			std::cout << "ERROR [int PacketStorage::pushPacket(PacketType)]: ";
			std::cout << "Unable to open file " << indexInput;
			std::cout << std::endl;

			delete[] bufferMemoryAlloc;

			return output;
		}
		for (int i = 2; i < indices.size(); i++) {
			//indices[i] -= size;
			indexFile << indices[i] << std::endl;
		}
		dataFile.seekg(0, std::ios::end);
		fileSize = dataFile.tellg();
		dataMemoryAlloc = new char[fileSize - size];
		dataFile.seekg(0, std::ios::beg);
		dataFile.read(dataMemoryAlloc, fileSize - size);
		dataFile.close();
		dataFile.open(dataInput, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!dataFile.is_open()) {
			std::cout << "ERROR [int PacketStorage::popPacket(PacketType)]: ";
			std::cout << "Unable to open file " << dataInput;
			std::cout << std::endl;

			delete[] bufferMemoryAlloc;
			delete[] dataMemoryAlloc;

			return output;
		}
		dataFile.write(dataMemoryAlloc, fileSize - size);

		delete[] bufferMemoryAlloc;
		delete[] dataMemoryAlloc;
	}

	dataFile.close();
	indexFile.close();

	return output;
}

void PacketStorage::packetPusher(std::vector<char> input, const char *indexInput, const char *dataInput)
{
	char *bufferMemoryAlloc;
	char *dataMemoryAlloc;
	std::fstream indexFile;
	std::fstream dataFile;
	std::vector<char> output;
	std::vector<long> indices;
	std::vector<std::string> index;
	std::string temporary;
	std::string::size_type sz;
	long startAddress;
	long endAddress;
	long size;
	long fileSize;
	bool newIndexCreated;
	bool newDataCreated;

	if (input.size() == 0) {
		return; // Return if empty
	}
	
	newIndexCreated = false;
	newDataCreated = false;
	indexFile.open(indexInput, std::ios::in);
	if (!indexFile.is_open()) {
		indexFile.open(indexInput, std::ios::out | std::ios::trunc);
		indexFile.close();
		newIndexCreated = true;
		indexFile.open(indexInput, std::ios::in);
	}
	if (!indexFile.is_open()) {
		std::cout << "ERROR [void PacketStorage::pushPacket(PacketType)]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;

		return;
	}
	if (newIndexCreated == true) {
		dataFile.open(dataInput, std::ios::out | std::ios::binary | std::ios::trunc);
		dataFile.close();
		newDataCreated = true;
		dataFile.open(dataInput, std::ios::in | std::ios::binary);
	} else {
		dataFile.open(dataInput, std::ios::in | std::ios::binary);
		if (!dataFile.is_open()) {
			dataFile.open(dataInput, std::ios::out | std::ios::binary);
			dataFile.close();
			newDataCreated = true;
			dataFile.open(dataInput, std::ios::in | std::ios::binary);
		}
		if (!dataFile.is_open()) {
			std::cout << "ERROR [void PacketStorage::pushPacket(PacketType)]: ";
			std::cout << "Unable to open file " << dataInput;
			std::cout << std::endl;
			return;
		}
		if (newDataCreated == true) {
			indexFile.open(indexInput, std::ios::out | std::ios::trunc);
			indexFile.close();
			newIndexCreated = true;
			indexFile.open(indexInput, std::ios::in);
		}
	}
	while (std::getline(indexFile, temporary)) {
		if (temporary.size() > 0) {
			index.push_back(temporary);
			indices.push_back(std::stol(temporary, &sz));
		}
	}
	size = input.size();
	dataFile.seekg(0, std::ios::end);
	fileSize = dataFile.tellg();
	startAddress = fileSize;
	endAddress = fileSize + size - 1;
	if (endAddress < 0) {
		endAddress = 0;
	} else if (startAddress < 0) {
		startAddress = 0;
	}
	indices.insert(indices.begin(), endAddress);
	indices.insert(indices.begin(), startAddress);

	indexFile.close();
	indexFile.open(indexInput, std::ios::out | std::ios::trunc);
	if (!indexFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::pushPacket(PacketType)]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;
		return;
	}
	for (int i = 0; i < indices.size(); i++) {
		indexFile << indices[i] << std::endl;
	}
	dataFile.close();
	dataFile.open(dataInput, std::ios::out | std::ios::app | std::ios::binary);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::pushPacket(PacketType)]: ";
		std::cout << "Unable to open file " << dataInput;
		std::cout << std::endl;

	}
	bufferMemoryAlloc = new char[size];
	for (int i = 0; i < size; i++) {
		bufferMemoryAlloc[i] = input[i];
	}
	dataFile.write(bufferMemoryAlloc, size);

	delete[] bufferMemoryAlloc;	
	dataFile.close();
	indexFile.close();
}

void PacketStorage::removeMeshDuplicates()
{
#ifdef WINDOWS_IMPLEMENTATION
	const char *indexInput = "meshpacketindex.txt";
	const char *dataInput = "meshpacketdata.txt";
#else
	const char *indexInput = "/opt/chainlink/meshpacketindex.txt";
	const char *dataInput = "/opt/chainlink/meshpacketdata.txt";
#endif // WINDOWS_IMPLEMENTATION
	char *bufferMemoryAlloc;
	char *dataMemoryAlloc;
	std::fstream indexFile;
	std::fstream dataFile;
	std::vector<char> output;
	std::vector<long> indices;
	std::vector<std::string> index;
	std::string temporary;
	std::string::size_type sz;
	long startAddress;
	long endAddress;
	long size;
	long fileSize;
	int currentIndex[4];
	int duplicate[2];
	bool duplicatePresent;

	indexFile.open(indexInput, std::ios::in);
	if (!indexFile.is_open()) {
		std::cout << "ERROR [void PacketStorage::removeMeshDuplicates()]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;
		return; // Unable to open file.
	}
	while (std::getline(indexFile, temporary)) {
		if (temporary.size() > 0) {
			index.push_back(temporary);
			indices.push_back(std::stol(temporary, &sz));
		}
	}
	indexFile.close();
	size = index.size();
	dataFile.open(dataInput, std::ios::in | std::ios::binary);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [void PacketStorage::removeMeshDuplicates()]: ";
		std::cout << "Unable to open file " << dataInput;
		std::cout << std::endl;
		return; // Unable to open file.
	}
	dataFile.seekg(0, std::ios::end);
	fileSize = dataFile.tellg();
	dataFile.seekg(0, std::ios::beg);
	dataMemoryAlloc = new char[fileSize];
	dataFile.read(dataMemoryAlloc, fileSize);
	dataFile.close();
	if (index.size() < 3) { // Ignore if only one packet
		return;
	}
	duplicatePresent = false;
	do {
		if (duplicatePresent) {
			removeDuplicate(dataMemoryAlloc, indices, index, duplicate);
			duplicatePresent = false;
		}
		for (int i = 0; i < indices.size() - 2; i += 2) {
			for (int j = i + 2; j < indices.size(); j += 2) {				
				if (!duplicatePresent) {
					currentIndex[0] = indices[i];
					currentIndex[1] = indices[i + 1];
					currentIndex[2] = indices[j];
					currentIndex[3] = indices[j + 1];
					if (comparePacket(dataMemoryAlloc, currentIndex)) {
						duplicatePresent = true;
						duplicate[0] = currentIndex[2];
						duplicate[1] = currentIndex[3];
					}
				}
			}
		}
	} while (duplicatePresent == true);

	indexFile.open(indexInput, std::ios::out | std::ios::trunc);
	if (!indexFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::removeMeshDuplicates()]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;
		return;
	}
	for (int i = 0; i < index.size(); i++) {
		indexFile << index[i] << std::endl;
	}
	indexFile.close();
	dataFile.open(dataInput, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::removeMeshDuplicates()]: ";
		std::cout << "Unable to open file " << dataInput;
		std::cout << std::endl;
		return; // Handle error here!
	}
	if (index.size() > 0) {
		fileSize = std::stoi(index[1]) - std::stoi(index[index.size() - 2]) + 1;
	} else {
		fileSize = 0;
	}

	dataFile.write(dataMemoryAlloc, fileSize);
	dataFile.close();

	delete[] dataMemoryAlloc;
}

void PacketStorage::removeQueuedDuplicates()
{
#ifdef WINDOWS_IMPLEMENTATION
	const char *indexInput = "queuedpacketindex.txt";
	const char *dataInput = "queuedpacketdata.txt";
#else
	const char *indexInput = "/opt/chainlink/queuedpacketindex.txt";
	const char *dataInput = "/opt/chainlink/queuedpacketdata.txt";
#endif // WINDOWS_IMPLEMENTATION
	char *bufferMemoryAlloc;
	char *dataMemoryAlloc;
	std::fstream indexFile;
	std::fstream dataFile;
	std::vector<char> output;
	std::vector<long> indices;
	std::vector<std::string> index;
	std::string temporary;
	std::string::size_type sz;
	long startAddress;
	long endAddress;
	long size;
	long fileSize;
	int currentIndex[4];
	int duplicate[2];
	bool duplicatePresent;

	indexFile.open(indexInput, std::ios::in);
	if (!indexFile.is_open()) {
		std::cout << "ERROR [void PacketStorage::removeQueuedDuplicates()]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;
		return; // Unable to open file.
	}
	while (std::getline(indexFile, temporary)) {
		if (temporary.size() > 0) {
			index.push_back(temporary);
			indices.push_back(std::stol(temporary, &sz));
		}
	}
	indexFile.close();
	size = index.size();
	dataFile.open(dataInput, std::ios::in | std::ios::binary);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [void PacketStorage::removeQueuedDuplicates()]: ";
		std::cout << "Unable to open file " << dataInput;
		std::cout << std::endl;
		return; // Unable to open file.
	}
	dataFile.seekg(0, std::ios::end);
	fileSize = dataFile.tellg();
	dataFile.seekg(0, std::ios::beg);
	dataMemoryAlloc = new char[fileSize];
	dataFile.read(dataMemoryAlloc, fileSize);
	dataFile.close();
	if (index.size() < 3) { // Ignore if only one packet
		return;
	}
	duplicatePresent = false;
	do {
		if (duplicatePresent) {
			removeDuplicate(dataMemoryAlloc, indices, index, duplicate);
			duplicatePresent = false;
		}
		for (int i = 0; i < indices.size() - 2; i += 2) {
			for (int j = i + 2; j < indices.size(); j += 2) {
				if (!duplicatePresent) {
					currentIndex[0] = indices[i];
					currentIndex[1] = indices[i + 1];
					currentIndex[2] = indices[j];
					currentIndex[3] = indices[j + 1];
					if (comparePacket(dataMemoryAlloc, currentIndex)) {
						duplicatePresent = true;
						duplicate[0] = currentIndex[2];
						duplicate[1] = currentIndex[3];
					}
				}
			}
		}
	} while (duplicatePresent == true);

	indexFile.open(indexInput, std::ios::out | std::ios::trunc);
	if (!indexFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::removeQueuedDuplicates()]: ";
		std::cout << "Unable to open file " << indexInput;
		std::cout << std::endl;
		return;
	}
	for (int i = 0; i < index.size(); i++) {
		indexFile << index[i] << std::endl;
	}
	indexFile.close();
	dataFile.open(dataInput, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::removeQueuedDuplicates()]: ";
		std::cout << "Unable to open file " << dataInput;
		std::cout << std::endl;
		return; // Handle error here!
	}
	if (index.size() > 0) {
		fileSize = std::stoi(index[1]) - std::stoi(index[index.size() - 2]) + 1;
	} else {
		fileSize = 0;
	}

	dataFile.write(dataMemoryAlloc, fileSize);
	dataFile.close();

	delete[] dataMemoryAlloc;
}

void PacketStorage::resetStorage(const char *indexInput, const char *dataInput)
{
	std::fstream indexFile;
	std::fstream dataFile;

	indexFile.open(indexInput, std::ios::out | std::ios::trunc);
	if (!indexFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::resetPackageStorage(PacketType)]: ";
		std::cout << "Unable to create file " << indexInput;
		std::cout << std::endl;
	}
	indexFile.close();
	dataFile.open(dataInput, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!dataFile.is_open()) {
		std::cout << "ERROR [int PacketStorage::resetPackageStorage(PacketType)]: ";
		std::cout << "Unable to create file " << dataInput;
		std::cout << std::endl;
	}
	dataFile.close();
}

void PacketStorage::transferQueuedPackets()
{
	std::vector<char> packet;

	consolidateQueuedPackets();
	do {
		packet = popPacket(QUEUED);
		if (packet.size() > 0) {
			pushPacket(packet, MESH);
		}
	} while (packet.size() > 0);
}

bool comparePacket(char *buffer, int indices[4])
{
	bool output;
	int firstSize;
	int secondSize;
	int firstIndex;
	int secondIndex;

	firstSize = indices[1] - indices[0] + 1;
	secondSize = indices[3] - indices[2] + 1;
	firstIndex = indices[0];
	secondIndex = indices[2];
	if (firstSize == secondSize) {
		output = true;
		for (int i = 0; i < firstSize; i++) {
			if (buffer[firstIndex + i] != buffer[secondIndex + i]) {
				output = false;
			}
		}
	} else {
		output = false;
	}

	return output;
}

bool comparePacket(std::vector<char> first, std::vector<char> second)
{
	bool output;

	//output = false;
	if (first.size() == second.size()) {
		output = true;
		for (int i = 0; i < first.size(); i++) {
			if (first[i] != second[i]) {
				output = false;
			}
		}
	} else {
		output = false;
	}

	return output;
}

void removeDuplicate(char *buffer, std::vector<long>& indices, std::vector<std::string>& index, int duplicates[2])
{
	int startIndex, endIndex;
	int startAddress, endAddress;
	int fileSize, size;
	bool startSet;
	bool endSet;
	char *temporary;
	std::vector<long> indicesCopy;
	std::vector<std::string> indexCopy;

	startSet = false;
	endSet = false;
	startAddress = duplicates[0];
	endAddress = duplicates[1];
	size = endAddress - startAddress + 1;
	//fileSize = indices[1] - indices[indices.size() - 2] + 1;
	fileSize = indices[1] + 1;
	temporary = new char[fileSize];
	for (int i = 0; i < indices.size(); i++) {
		if (indices[i] == startAddress) {
			startIndex = i;
			startSet = true;
		}
		if (indices[i] == endAddress) {
			endIndex = i;
			endSet = true;
		}
	}
	// Check that endIndex = startIndex + 1
	if (startSet && endSet) {
		for (int i = 0; i < startAddress; i++) {
			temporary[i] = buffer[i];
		}
		for (int i = endAddress + 1, j = startAddress; i < fileSize; i++, j++) {
			temporary[j] = buffer[i];
		}
		for (int i = 0; i < fileSize - size; i++) {
			buffer[i] = temporary[i];
		}
		for (int i = 0; i < indices.size(); i++) {
			if (i < endIndex - 1) {
				indices[i] -= size;
				index[i] = std::to_string(indices[i]);
			}
		}
		for (int i = 0; i < startIndex; i++) {
			indexCopy.push_back(index[i]);
			indicesCopy.push_back(indices[i]);
		}
		for (int i = endIndex + 1; i < indices.size(); i++) {
			indexCopy.push_back(index[i]);
			indicesCopy.push_back(indices[i]);
		}
		index = indexCopy;
		indices = indicesCopy;
		//indices.erase(indices.begin() + startIndex, indices.begin() + startIndex + 2);
		//index.erase(index.begin() + startIndex, index.begin() + startIndex + 2);
		//indices.erase(indices.begin() + startIndex);
		//indices.erase(indices.begin() + startIndex);
		//index.erase(index.begin() + startIndex);
		//index.erase(index.begin() + startIndex);
		for (int i = 0; i < indices.size(); i++) {
			std::cout << indices[i] << " ";
		}
		std::cout << std::endl;
	}

	delete[] temporary;
}

bool verifyQueuedPacket(std::vector<char> input)
{
	std::vector<char> temporary;
	char sentByte;
	unsigned char checksum;
	unsigned char checkbyte;

	// Verify first byte is sent/unsent
	sentByte = input[0];
	if ((sentByte != PACKET_SENT) && (sentByte != PACKET_UNSENT)) {
		return false;
	}
	// Verify checksum
	checksum = 0;
	for (int i = 0; i < input.size(); i++) {
		checksum = checksum ^ static_cast<unsigned char>(input[i]);
	}
	checkbyte = static_cast<unsigned char>(input[input.size() - 1]);
	if (checkbyte != checksum) {
		return false;
	}

	return true;
}