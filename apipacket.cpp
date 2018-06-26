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


//*************************************************************DANE

const int DATA = 512;

class TXRQ_Packet
{

    public:
     const char START = '~';
     const char FRAME_TYPE = ' ';
     const char TRANSMIT_OPT = '0';
     const char TX_PROTOCOL = '0';
  
                               //Assign Nonconstant Variables with outside Data
      void assignFrameID(const char x) { Frame_ID = x; }
      void assignAddress(vector<char> a) { Destination_Address = a; }
      void assignDPort(vector<char> d) { Destination_Port = d; }
      void assignSrcPort(vector<char> s) { SourcePort = s; }
      void assignPayload(vector<char> p) { PayLoad = p; }
      
                               //pass a vector and set as equal to existing member variable
      char getFrameID() { return Frame_ID; }
      void getDestination_Address(vector<char> a) { a = Destination_Address; }
      void getDestination_Port(vector<char> a) { a = Destination_Port; }
      void getSourcePort(vector<char> a) {a = SourcePort; }
      void getPayload(vector<char> a) { a = PayLoad; }
      
                                //lumps protocol and transmit in with sourceport 
      void Concat() { SourcePort.push_back(TX_PROTOCOL); SourcePort.push_back(TRANSMIT_OPT); }
    
                              //Takes all prepared data and assembles Packet
      void BuildPacket();
                              
    private:
      char Frame_ID;
      vector<char> Destination_Address;
      vector<char> Destination_Port;
      vector<char> SourcePort;
      vector<char> PayLoad;
      
      vector<char> TXPacket;    //Raw, no checksum
};



int main()
{
    cout<<"Starting TX Request"<<endl;
    vector<char> Address;
    vector<char> Destination;
    vector<char> Source;
    vector<char> Payload;
    
    TXRQ_Packet MyPacket;
    

    MyPacket.assignFrameID(1);
    MyPacket.assignAddress(Address);
    MyPacket.assignDPort(Destination);
    MyPacket.assignSrcPort(Source);
    MyPacket.assignPayload(Payload);
    
    MyPacket.BuildPacket();
    
    
    return 0; 
}
    
void TXRQ_Packet::BuildPacket()
{
    TXPacket.push_back(START);
    TXPacket.push_back(FRAME_TYPE);
    TXPacket.push_back(Frame_ID);
    TXPacket.insert(TXPacket.end(), Destination_Address.begin(), Destination_Address.end());
    TXPacket.insert(TXPacket.end(), Destination_Port.begin(), Destination_Port.end());
    Concat();                   //concats TX_PROTOCOL and TRANSMIT_OPT tp SourcePort
    TXPacket.insert(TXPacket.end(), SourcePort.begin(), SourcePort.end());
    TXPacket.insert(TXPacket.end(), PayLoad.begin(), PayLoad.end());
	
   return;
}   
    
