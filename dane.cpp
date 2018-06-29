#include <iostream>
#include <vector>
// Author: Dane Rodriguez
// Packet Parser Program (PPP)

//Function Prototypes
bool Verify( std::vector<char> &P, char &LSB, char &MSB);
int Categorize( std::vector<char> &P, char &FT);
void ParseAT( std::vector<char> &P, char SFID, char ATCMMD, char STS, char PV);
void ParseSMS( std::vector<char> &P, char PN[], char MSSGE[] );
//void ParseTXStatus( std::vector<char> &P, char SFID, char Address[], char TX_Retry, char DeliveryStatus, char DiscoveryStatus);
void ParseTXStatus(std::vector<char> &P, char Address[], char SFID, char TX_Retry, char DeliveryStatus, char DiscoveryStatus);
void Fail();

 const int DATA = 512;  
 const char START = 0x7e; 
 char Error_Code;
 
        //Frame Types
 const char RECIEVE_SMS = 0x9f;
 const char RECIEVE_AT = 0x88;
 const char RECIEVE_TX_STAT = 0x8B;

        //Examine
 const int Invalid_Response = 0;
 const int AT_Response = 1;
 const int SMS_Response = 2;
const int TX_Status = 3;


int accessory_main_3()
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
    
     std::vector<char> RecievedData;
  
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
        ParseTXStatus(RecievedData, Address, Store_FrameID, TX_Retry, DeliveryStatus, DiscoveryStatus);
        break;
    }
  }
  else
  {
      Fail();
  }
    return 0;
}


bool Verify( std::vector<char> &P, char &Length_LSB, char &Length_MSB)
{
    if(P[0] == START)
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

int Categorize( std::vector<char> &P, char &Store_FrameType)
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


void ParseAT( std::vector<char> &P, char SFID, char ATCMMD, char STS, char PV)
{
    SFID = P[4] ;
    ATCMMD = P[5];
    STS = P[6];
    PV = P[7];
    return;
}

void ParseSMS( std::vector<char> &P, char PN[], char MSSGE[] )
{
    int i;
    int j;
    int end = (P.back());
    for( i = 4, j = 0; i < 24; i++, j++)
    {  PN[j] = P[i];  }
    for( i = 24, j = 0; i < end; i++, j++)
    {  MSSGE[j] = P[i];  }
}

void ParseTXStatus(std::vector<char> &P, char Address[], char SFID, char TX_Retry, char DeliveryStatus, char DiscoveryStatus)
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
  std::cout<< "Packet Failed.  Code: " << Error_Code << std::endl;
  return;
}
