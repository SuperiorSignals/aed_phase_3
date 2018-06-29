/******************************  Assembly For TX and AT For XBEE

     const char START = '0x7E';
     const char AT_FRAME_TYPE = '0x08';
     const char TX_FRAME_TYPE = '0x10';
     
    const char RESERVED_MSB = '0xFF';
    const char RESERVED_LSB = '0xFE';

     
void AssemblePrePacket( vector<char> myPacket, char FID, char MSB, char LSB);
void AssembleAT( vector<char> myPacket, vector<char> ATCMMD, vector<char> Parameter);
void AssembleTX_Request( vector<char> myPacket, vector<char> ADD, char BRRAD);


int main()
{

    //AT Variables
    char Frame_ID;
    vector<char> AT_Command;
    vector<char> Parameter;
    //TX Request Variables
    vector<char> Destination_Address;
    char Broadcast_Radius;
    char Length_MSB;
    char Length_LSB;
  
  vector<char> myPacket;


AssemblePrePacket(myPacket, Frame_ID, Length_MSB, Length_LSB);
//if(something)
AssembleAT(myPacket, AT_Command, Parameter);
//else
AssembleTX_Request(myPacket, Destination_Address, Broadcast_Radius);

//send packet via serial stuff
    
    return 0; 
}


void AssemblePrePacket(vector<char> myPacket, char FID, char MSB, char LSB)
{
  myPacket.push_back(START);
  myPacket.push_back(MSB);
  myPacket.push_back(LSB);
  //if(AT)
  //myPacket.push_back(AT_FRAME_TYPE);
  //else
  //myPacket.push_back(TX_FRAME_TYPE);
  
  myPacket.push_back(FID);
  return;
}
    
void AssembleAT(vector<char> myPacket, vector<char> ATCMMD, vector<char> Parameter)
{
  myPacket.insert(myPacket.end(), ATCMMD.begin(), ATCMMD.end());
  myPacket.insert(myPacket.end(), Parameter.begin(), Parameter.end());
  //Do Checksum
  return;
}

void AssembleTX_Request(vector<char> myPacket, vector<char> ADD, char BRRAD)
{
    myPacket.insert(myPacket.end(), ADD.begin(), ADD.end());
    myPacket.push_back(RESERVED_MSB);
    myPacket.push_back(RESERVED_LSB);
    myPacket.push_back(BRRAD);
    //do checksum
    return;
}

