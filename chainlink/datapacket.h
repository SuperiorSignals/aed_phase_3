/*
*	Author: Stewart Nash
*	Date: 3/21/2018
*	File: datapacket.h
*	Description: Packet for encapsulating dispatched data, header
*/
#pragma once
#ifndef DATAPACKET_H_
#define DATAPACKET_H_
#include <vector>
#include "adcpin.h"
#include "configuration.h"
#include "gpiopin.h"
#include "gpsdata.h"
#include "gpsmodule.h"
#include "xbeecell.h"
#include "xbeemesh.h"

#define IMEI_SIZE 15
#define ASCII_DIGIT_OFFSET 0x30

std::vector<unsigned char> compressImei(std::string input);
unsigned char generatePacketChecksum(std::vector<unsigned char> input);

enum PacketType { NORMAL, OUT_OF_AREA, INPUT_TRIGGERED, CONFIGURATION_CONFIRM, DEVICE_REPORT, MESH, EVENT, QUEUED };

class DataPacket {
private:
	std::vector<unsigned char> packet; // There is no need for this member

	GpioPin digitalInputOne;
	GpioPin digitalInputTwo;
	GpioPin digitalInputThree;
	GpioPin digitalInputFour;
	GpioPin digitalOutputOne;
	GpioPin digitalOutputTwo;
	
	AdcPin analogInputOne;
	AdcPin analogInputTwo;
	AdcPin vehicleVoltage;

	GpsModule gpsModule;
	GpsData gpsData;
	static unsigned char packetIdentifier;
	XBeeCell xBeeCell;
	XBeeMesh xBeeMesh;
	Configuration configuration;

	//void populatePacket();
	void populatePacket(PacketType event);

public:
	DataPacket();

	GpioPin getDigitalInputOne();
	GpioPin getDigitalInputTwo();
	GpioPin getDigitalInputThree();
	GpioPin getDigitalInputFour();
	GpioPin getDigitalOutputOne();
	GpioPin getDigitalOutputTwo();

	AdcPin getAnalogInputOne();
	AdcPin getAnalogInputTwo();
	AdcPin getVehicleVoltage();

	GpsData getGpsData();
	GpsModule getGpsModule();
	XBeeCell getXBeeCell();
	XBeeMesh getXBeeMesh();
	//std::vector<char> getPacket();
	std::vector<char> getPacket(PacketType event);
	std::vector<char> getPacket(PacketType event, Configuration config);
	std::vector<char> getPacketTranslation(PacketType event);
	std::vector<char> getPacketTranslation(PacketType event, Configuration config);

	void setDigitalInputOne(GpioPin input);
	void setDigitalInputTwo(GpioPin input);
	void setDigitalInputThree(GpioPin input);
	void setDigitalInputFour(GpioPin input);
	void setDigitalOutputOne(GpioPin input);
	void setDigitalOutputTwo(GpioPin input);

	void setAnalogInputOne(AdcPin input);
	void setAnalogInputTwo(AdcPin input);
	void setVehicleVoltage(AdcPin input);

	void setGpsData(GpsData input);
	void setGpsModule(GpsModule input);
	void setXBeeCell(XBeeCell input);
	void setXBeeMesh(XBeeMesh input);
	void setConfiguration(Configuration input);
};

#endif // !DATAPACKET_H_

