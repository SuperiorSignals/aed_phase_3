/*
*	Author: Stewart Nash
*	Date: 3/21/2018
*	File: datapacket.cpp
*	Description: Packet for encapsulating dispatched data, source
*/
#include "datapacket.h"
#include <iostream>
#include <string>
#include <vector>
#include "adcpin.h"
#include "vectorstring.h"

unsigned char DataPacket::packetIdentifier = 0;

DataPacket::DataPacket() : analogInputOne(ANALOG_ONE_CHIP, ANALOG_ONE_CHANNEL),
		analogInputTwo(ANALOG_TWO_CHIP, ANALOG_TWO_CHANNEL),
		vehicleVoltage(VEHICLE_VOLTAGE_CHIP, VEHICLE_VOLTAGE_CHANNEL)
{
	digitalInputOne.setLinuxKernelNumber(133);
	digitalInputTwo.setLinuxKernelNumber(134);
	digitalInputThree.setLinuxKernelNumber(135);
	digitalInputFour.setLinuxKernelNumber(136);
	digitalOutputOne.setLinuxKernelNumber(8);
	digitalOutputTwo.setLinuxKernelNumber(9);
}

GpioPin DataPacket::getDigitalInputOne() { return digitalInputOne; }
GpioPin DataPacket::getDigitalInputTwo() { return digitalInputTwo; }
GpioPin DataPacket::getDigitalInputThree() { return digitalInputThree; }
GpioPin DataPacket::getDigitalInputFour() { return digitalInputFour; }
GpioPin DataPacket::getDigitalOutputOne() { return digitalOutputOne; }
GpioPin DataPacket::getDigitalOutputTwo() { return digitalOutputTwo; }

AdcPin DataPacket::getAnalogInputOne() { return analogInputOne; }
AdcPin DataPacket::getAnalogInputTwo() { return analogInputTwo; }
AdcPin DataPacket::getVehicleVoltage() { return vehicleVoltage; }

GpsData DataPacket::getGpsData() { return gpsData; }
GpsModule DataPacket::getGpsModule() { return gpsModule; }
XBeeCell DataPacket::getXBeeCell() { return xBeeCell; }
XBeeMesh DataPacket::getXBeeMesh() { return xBeeMesh; }

/*
std::vector<char> DataPacket::getPacket()
{
	int length;
	std::vector<char> output;

	populatePacket();
	length = packet.size();
	for (int i = 0; i < length; i++) {
		output.push_back(static_cast<char>(packet[i]));
	}
	packetIdentifier++;

	return output;
}
*/

std::vector<char> DataPacket::getPacket(PacketType event)
{
	int length;
	std::vector<char> output;

	populatePacket(event);
	length = packet.size();
	for (int i = 0; i < length; i++) {
		output.push_back(static_cast<char>(packet[i]));
	}
	packetIdentifier++;

	return output;
}

std::vector<char> DataPacket::getPacket(PacketType event, Configuration config)
{
	int length;
	std::vector<char> output;

	configuration = config;
	populatePacket(event);
	length = packet.size();
	for (int i = 0; i < length; i++) {
		output.push_back(static_cast<char>(packet[i]));
	}
	packetIdentifier++;

	return output;
}

std::vector<char> DataPacket::getPacketTranslation(PacketType event)
{
	int length;
	std::vector<char> input;
	std::vector<char> output;
	std::vector<char> temporary;
	char nibbleLow;
	char nibbleHigh;

	populatePacket(event);
	length = packet.size();
	for (int i = 0; i < length; i++) {
		input.push_back(static_cast<char>(packet[i]));
	}
	for (int i = 0; i < length; i++) {
		temporary = spellOutCharacter(input[i]);
		nibbleLow = temporary[1];
		nibbleHigh = temporary[0];
		output.push_back(nibbleLow);
		output.push_back(nibbleHigh);
	}
	packetIdentifier++;

	return output;
}

std::vector<char> DataPacket::getPacketTranslation(PacketType event, Configuration config)
{
	int length;
	std::vector<char> input;
	std::vector<char> output;
	std::vector<char> temporary;
	char nibbleLow;
	char nibbleHigh;

	configuration = config;
	populatePacket(event);
	length = packet.size();
	for (int i = 0; i < length; i++) {
		input.push_back(static_cast<char>(packet[i]));
	}
	for (int i = 0; i < length; i++) {
		temporary = spellOutCharacter(input[i]);
		nibbleLow = temporary[1];
		nibbleHigh = temporary[0];
		output.push_back(nibbleLow);
		output.push_back(nibbleHigh);
	}
	packetIdentifier++;

	return output;
}

void DataPacket::setDigitalInputOne(GpioPin input) { digitalInputOne = input; }
void DataPacket::setDigitalInputTwo(GpioPin input) { digitalInputTwo = input; }
void DataPacket::setDigitalInputThree(GpioPin input) { digitalInputThree = input; }
void DataPacket::setDigitalInputFour(GpioPin input) { digitalInputFour = input; }
void DataPacket::setDigitalOutputOne(GpioPin input) { digitalOutputOne = input; }
void DataPacket::setDigitalOutputTwo(GpioPin input) { digitalOutputTwo = input; }

void DataPacket::setAnalogInputOne(AdcPin input) { analogInputOne = input; }
void DataPacket::setAnalogInputTwo(AdcPin input) { analogInputTwo = input; }
void DataPacket::setVehicleVoltage(AdcPin input) { vehicleVoltage = input; }

void DataPacket::setGpsData(GpsData input) { gpsData = input; }
void DataPacket::setGpsModule(GpsModule input) { gpsModule = input; }
void DataPacket::setXBeeCell(XBeeCell input) { xBeeCell = input; }
void DataPacket::setXBeeMesh(XBeeMesh input) { xBeeMesh = input; }
void DataPacket::setConfiguration(Configuration input) { configuration = input; }

/*
void DataPacket::populatePacket()
{
	std::string s;
	int i;
	unsigned char uc;
	char c;
	float f;
	std::vector<unsigned char> temporary;

	GpsRmcData gpsRmcData;
	GpsGgaData gpsGgaData;
	Date date;
	Time time;
	Position latitude;
	Position longitude;
	Unit altitude;

	gpsRmcData = gpsData.getGpsRmc();
	gpsGgaData = gpsData.getGpsGga();

	packet.clear();
	s = xBeeCell.getImei();
	temporary = compressImei(s);
	packet.insert(packet.end(), temporary.begin(), temporary.end());
	date = gpsRmcData.getDate();
	packet.push_back(static_cast<unsigned char>(date.getYear()));
	packet.push_back(static_cast<unsigned char>(date.getMonth()));
	packet.push_back(static_cast<unsigned char>(date.getDay()));
	time = gpsRmcData.getTime();
	packet.push_back(static_cast<unsigned char>(time.getHour()));
	packet.push_back(static_cast<unsigned char>(time.getMinute()));
	packet.push_back(static_cast<unsigned char>(time.getSecond()));
	c = (time.getMilliseconds() >> 8) & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = time.getMilliseconds() & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));

	packet.push_back(packetIdentifier);  // Event byte high. Packet identifier?
	packet.push_back(0); // Event byte low. Set accordingly.
	// Latitude
	latitude = gpsGgaData.getLatitude();
	packet.push_back(static_cast<unsigned char>(latitude.getDegrees()));
	//std::cout << "Latitude Degrees: " << latitude.getDegrees() << std::endl;
	i = static_cast<int>(latitude.getMinutes());
	packet.push_back(static_cast<unsigned char>(i));
	//std::cout << "Latitude Minutes: " << latitude.getMinutes() << std::endl;
	f = latitude.getMinutes();
	f = f - static_cast<float>(i);
	f *= 10000;
	i = static_cast<int>(f);
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	packet.push_back(static_cast<unsigned char>(latitude.getDirection()));
	// Longitude
	longitude = gpsGgaData.getLongitude();
	i = longitude.getDegrees();
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	//std::cout << "Longitude Degrees: " << longitude.getDegrees() << std::endl;
	i = static_cast<int>(longitude.getMinutes());
	packet.push_back(static_cast<unsigned char>(i));
	f = longitude.getMinutes();
	f = f - static_cast<float>(i);
	f *= 10000;
	i = static_cast<int>(f);
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	//std::cout << "Longitude Minutes: " << longitude.getMinutes() << std::endl;
	packet.push_back(static_cast<unsigned char>(longitude.getDirection()));
	// Altitude
	altitude = gpsGgaData.getAltitude();
	i = static_cast<int>(altitude.getValue());
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	f = altitude.getValue();
	f = f - static_cast<float>(i);
	f *= 10;
	i = static_cast<int>(f);
	packet.push_back(static_cast<unsigned char>(i));
	// Speed
	i = static_cast<int>(gpsRmcData.getSpeed());
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	f = gpsRmcData.getSpeed();
	f = f - static_cast<float>(i);
	f *= 10;
	i = static_cast<int>(f);
	packet.push_back(static_cast<unsigned char>(i));
	// Angle
	i = static_cast<int>(gpsRmcData.getTrueCourse());
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	f = gpsRmcData.getTrueCourse();
	f = f - static_cast<float>(i);
	f *= 10;
	i = static_cast<int>(f);
	packet.push_back(static_cast<unsigned char>(i));

	packet.push_back(0); // Vehicle voltage high
	packet.push_back(0); // Vehicle voltage low
	packet.push_back(0); // Battery status

	uc = 0;
	if (digitalInputOne.read() == GPIO_PIN_OUTPUT_HIGH) {
		uc += 1;
	}
	if (digitalInputTwo.read() == GPIO_PIN_OUTPUT_HIGH) {
		uc += 2;
	}
	if (digitalInputThree.read() == GPIO_PIN_OUTPUT_HIGH) {
		uc += 4;
	}
	if (digitalInputFour.read() == GPIO_PIN_OUTPUT_HIGH) {
		uc += 8;
	}
	if (digitalOutputOne.getMode() == GPIO_PIN_OUTPUT_HIGH) {
		uc += 16;
	}
	if (digitalOutputTwo.getMode() == GPIO_PIN_OUTPUT_HIGH) {
		uc += 32;
	}
	packet.push_back(uc); // Digital IO

	packet.push_back(0); // Analog 1 high
	packet.push_back(0); // Analog 1 low
	packet.push_back(0); // Analog 2 high
	packet.push_back(0); // Analog 2 low
	packet.push_back(0); // Relay
	if (gpsData.getValidity()) { // Validity
		packet.push_back(1);
	} else {
		packet.push_back(0);
	}

	packet.push_back(0); // Data length
	packet.push_back(generatePacketChecksum(packet));
}
*/

void DataPacket::populatePacket(PacketType event)
{
	std::string s;
	int i;
	unsigned char uc;
	char c;
	float f;
	std::vector<unsigned char> temporary;

	GpsRmcData gpsRmcData;
	GpsGgaData gpsGgaData;
	Date date;
	Time time;
	Position latitude;
	Position longitude;
	Unit altitude;

	gpsRmcData = gpsData.getGpsRmc();
	gpsGgaData = gpsData.getGpsGga();

	packet.clear();
	s = xBeeCell.getImei();
	temporary = compressImei(s);
	packet.insert(packet.end(), temporary.begin(), temporary.end());
	date = gpsRmcData.getDate();
	packet.push_back(static_cast<unsigned char>(date.getYear()));
	packet.push_back(static_cast<unsigned char>(date.getMonth()));
	packet.push_back(static_cast<unsigned char>(date.getDay()));
	time = gpsRmcData.getTime();
	packet.push_back(static_cast<unsigned char>(time.getHour()));
	packet.push_back(static_cast<unsigned char>(time.getMinute()));
	packet.push_back(static_cast<unsigned char>(time.getSecond()));
	c = (time.getMilliseconds() >> 8) & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = time.getMilliseconds() & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));

	packet.push_back(packetIdentifier);  // Event byte high. Packet identifier?
	switch (event) { // Event byte low.
	case NORMAL:
		packet.push_back(0);
		break;
	case OUT_OF_AREA:
		packet.push_back(1);
		break;
	case INPUT_TRIGGERED:
		packet.push_back(2);
		break;
	case CONFIGURATION_CONFIRM:
		packet.push_back(3);
		break;
	case DEVICE_REPORT:
		packet.push_back(4);
		break;
	case MESH:
		packet.push_back(5);
		break;
	case EVENT:
		packet.push_back(6);
		break;
	default:
		packet.push_back(0);
		break;
	}
						 // Latitude
	latitude = gpsGgaData.getLatitude();
	packet.push_back(static_cast<unsigned char>(latitude.getDegrees()));
	//std::cout << "Latitude Degrees: " << latitude.getDegrees() << std::endl;
	i = static_cast<int>(latitude.getMinutes());
	packet.push_back(static_cast<unsigned char>(i));
	//std::cout << "Latitude Minutes: " << latitude.getMinutes() << std::endl;
	f = latitude.getMinutes();
	f = f - static_cast<float>(i);
	f *= 10000;
	i = static_cast<int>(f);
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	packet.push_back(static_cast<unsigned char>(latitude.getDirection()));
	// Longitude
	longitude = gpsGgaData.getLongitude();
	i = longitude.getDegrees();
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	//std::cout << "Longitude Degrees: " << longitude.getDegrees() << std::endl;
	i = static_cast<int>(longitude.getMinutes());
	packet.push_back(static_cast<unsigned char>(i));
	f = longitude.getMinutes();
	f = f - static_cast<float>(i);
	f *= 10000;
	i = static_cast<int>(f);
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	//std::cout << "Longitude Minutes: " << longitude.getMinutes() << std::endl;
	packet.push_back(static_cast<unsigned char>(longitude.getDirection()));
	// Altitude
	altitude = gpsGgaData.getAltitude();
	i = static_cast<int>(altitude.getValue());
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	f = altitude.getValue();
	f = f - static_cast<float>(i);
	f *= 10;
	i = static_cast<int>(f);
	packet.push_back(static_cast<unsigned char>(i));
	// Speed
	i = static_cast<int>(gpsRmcData.getSpeed());
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	f = gpsRmcData.getSpeed();
	f = f - static_cast<float>(i);
	f *= 10;
	i = static_cast<int>(f);
	packet.push_back(static_cast<unsigned char>(i));
	// Angle
	i = static_cast<int>(gpsRmcData.getTrueCourse());
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c));
	f = gpsRmcData.getTrueCourse();
	f = f - static_cast<float>(i);
	f *= 10;
	i = static_cast<int>(f);
	packet.push_back(static_cast<unsigned char>(i));
	i = vehicleVoltage.read();
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c)); // Vehicle voltage high
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c)); // Vehicle voltage low

	packet.push_back(0); // Battery status

	uc = 0;

	/*
	digitalInputOne.setLinuxKernelNumber(IO_EXPANDER, 0, 3);
	digitalInputTwo.setLinuxKernelNumber(IO_EXPANDER, 0, 6);
	digitalInputThree.setLinuxKernelNumber(IO_EXPANDER, 0, 4);
	digitalInputFour.setLinuxKernelNumber(IO_EXPANDER, 0, 5);
	*/

	if (digitalInputOne.read() == GPIO_PIN_OUTPUT_LOW) { // Pin reading is reversed
		uc += 1;
	}
	if (digitalInputTwo.read() == GPIO_PIN_OUTPUT_LOW) { // Pin reading is reversed
		uc += 2;
	}
	if (digitalInputThree.read() == GPIO_PIN_OUTPUT_LOW) { // Pin reading is reversed
		uc += 4;
	}
	if (digitalInputFour.read() == GPIO_PIN_OUTPUT_LOW) { // Pin reading is reversed
		uc += 8;
	}
	if (configuration.getIsDigital0High() == true) {
		uc += 16;
	}
	if (configuration.getIsDigital1High() == true) {
		uc += 32;
	}
	packet.push_back(uc); // Digital IO

	i = analogInputOne.read();
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c)); // Analog 1 high
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c)); // Analog 1 low
	i = analogInputTwo.read();
	c = (i >> 8) && 0xFF;
	packet.push_back(static_cast<unsigned char>(c)); // Analog 2 high
	c = i & 0xFF;
	packet.push_back(static_cast<unsigned char>(c)); // Analog 2 low
	packet.push_back(0); // Relay
	if (gpsData.getValidity()) { // Validity
		packet.push_back(1);
	} else {
		packet.push_back(0);
	}

	packet.push_back(0); // Data length
	packet.push_back(generatePacketChecksum(packet));
}

std::vector<unsigned char> compressImei(std::string input)
{
	std::vector<unsigned char> output;
	int length;
	char c;

	length = input.size();
	if (length == IMEI_SIZE) { // It is the right size, do nothing

	} else if (length > IMEI_SIZE) { // It is too big, take off characters
		input = input.substr(0, IMEI_SIZE);
	} else if (length < IMEI_SIZE) { // It is too small, add characters
		for (int i = length; i < IMEI_SIZE; i++) {
			input.push_back('0');
		}
	}
	input.push_back('0');
	for (int i = 0; i < (IMEI_SIZE / 2 + 1); i++) {
		c = input[2 * i] - ASCII_DIGIT_OFFSET; // Pack high part of the byte
		c = c << 4;
		c += input[2 * i + 1] - ASCII_DIGIT_OFFSET;
		output.push_back(static_cast<unsigned char>(c));
	}

	return output;
}

unsigned char generatePacketChecksum(std::vector<unsigned char> input)
{
	unsigned char output;

	output = 0;
	for (int i = 0; i < input.size(); i++) {
		output = output ^ input[i];
	}

	return output;
}



/*****************************************************************************************************************************/
//DIGIMESH Packet Retrieval
//All Rights Reserved
//D_Rod Softworks


        //Function Prototypes
bool Verify( vector<char> &P, char &LSB, char &MSB);
void ParsePacket( vector<char> &P, char ADD[], char &REOPT, char RD[]);
void Fail();

 const char START = '0x7e'; 
 const int DATA = 512;
 
        //Frame Types
 const char RX_Frame_TYPE = '0x90';
 const char RESERVED_MSB = '0xFF';
 const char RESERVED_LSB = '0xFE';

int RecievePacket()
{
        //Variables for the Main Program
     vector<char> PacketData;
     char Length_MSB; 
     char Length_LSB; 
     char Address[9];
     char RecieveOpions;
     char RecievedData[DATA];
     
  
  if(Verify(PacketData, Length_LSB, Length_MSB))  //Check START
  {
        ParsePacket(PacketData, Address, RecieveOpions, RecievedData);
  }

    return 0;
}


bool Verify( vector<char> &P, char &Length_LSB, char &Length_MSB)
{
    if(P[0] == 'START' & P[3] == 'RX_Frame_TYPE')
    {
        P[1] = Length_MSB;
        P[2] = Length_LSB;
        return true; 
    }
    else
    {
      return false;
    }    
}    


void ParsePacket( vector<char> &P, char ADD[], char &REOPT, char RD[])
{
    int i;
    int j;
    int end = P.back();
    
    for(i=4, j=0; i<12; i++, j++)
    { ADD[j] = P[i]; }
    
    REOPT = P[14];
    for(i = 15, j = 0; i<end; i++, j++)
    { P[i] = RD[j]; }
    return;
}
