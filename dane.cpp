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



//Packet Parser Program (PPP)
//All Rights Reserved
//D_Rod Softworks


        //Function Prototypes
bool Verify( vector<char> &P, char &LSB, char &MSB);
int Categorize( vector<char> &P, char &FT);
void ParseAT( vector<char> &P, char SFID, char ATCMMD, char STS, char PV);
void ParseSMS( vector<char> &P, char PN[], char MSSGE[] );
void ParseTXStatus( vector<char> &P, char SFID, char Address[], char TX_Retry, char DeliveryStatus, char DiscoveryStatus);
void Fail();

 const char DATA = 512;  
 const char START = '0x7e'; 
 char Error_Code;
 
        //Frame Types
 const char RECIEVE_SMS = '0x9f';
 const char RECIEVE_AT = '0x88';
 const char RECIEVE_TX_STAT = '0x8B';

        //Examine
 const int Invalid_Response = 0;
 const int AT_Response = 1;
 const int SMS_Response = 2;
const int TX_Status = 3;


int main()
{
        //Variables for the Main Program
    char Length_MSB; 
    char Length_LSB; 
    char Store_FrameType;                 //temp
    char Store_FrameID;
  //AT
    char AT_Command;
    char Status;
    char Parameter_Value;
    char CheckSum;
  //SMS 
    char PhoneNumber[DATA];
    char Message[DATA];
  //TX STATUS  
    char Address[2];
    char TX_Retry;
    char DeliveryStatus;
    char DiscoveryStatus;
    
     vector<char> RecievedData;
  
  if(Verify(RecievedData, Length_LSB, Length_MSB))  //Check START
  {
    
    switch(Categorize(RecievedData, Store_FrameType))
    {
        case 0:
        Error_Code = 2;
        Fail();
        break;
        
        case 1:  //AT_Response
        ParseAT(RecievedData, Store_FrameID, AT_Command, Status, Parameter_Value);
        //ValidateChecksum;
        //DisplayAT();
        break;
        
        case 2:  //SMS_Response
        ParseSMS(RecievedData, PhoneNumber, Message);
        //DisplaySMS();
        break;
        
        case 3:  //Transmit Status
        ParseTXStatus(RecievedData, Store_FrameID, Address, TX_Retry, DeliveryStatus, DiscoveryStatus);
        break;
    }
  }
  else
  {
      Fail();
  }
    return 0;
}


bool Verify( vector<char> &P, char &Length_LSB, char &Length_MSB)
{
    if(P[0] == 'START')
    {
        Length_MSB = P[1]; 
        Length_LSB  = P[2];
        return true; 
    }
    else
    {
      Error_Code = '1';
      return false;
    }    
}    

int Categorize( vector<char> &P, char &Store_FrameType)
{
   Store_FrameType = P[3];
    
    if(Store_FrameType = RECIEVE_AT)
    { return AT_Response; }
    if(Store_FrameType = RECIEVE_SMS)
    { return SMS_Response; }
    if(Store_FrameType = RECIEVE_TX_STAT)
    { return TX_Status; }
    return 0;
}


void ParseAT( vector<char> &P, char SFID, char ATCMMD, char STS, char PV)
{
    SFID = P[4] ;
    ATCMMD = P[5];
    STS = P[6];
    PV = P[7];
    return;
}

void ParseSMS( vector<char> &P, char PN[], char MSSGE[] )
{
    int i;
    int j;
    int end = (P.back());
    for( i = 4, j = 0; i < 24; i++, j++)
    {  PN[j] = P[i];  }
    for( i = 24, j = 0; i < end; i++, j++)
    {  MSSGE[j] = P[i];  }
}

void ParseTXStatus( vector<char> &P, char Address[], char SFID, char TX_Retry, char DeliveryStatus, char DiscoveryStatus)
{
    
    SFID = P[4];
    Address[0] = P[5];  
    Address[1] = P[6];
    TX_Retry = P[7];
    DeliveryStatus = P[8];
    DiscoveryStatus = P[9];
    return;
}

void Fail()
{
  cout<<"Packet Failed.  Code: "<<Error_Code<<endl;
  return;
}

