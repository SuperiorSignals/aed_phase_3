#include "configuration.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "adcpin.h"
#include "gpiopin.h"
#include "vectorstring.h"
#include "visual_studio.h"

#define DIGITAL_OUT_ZERO_KERNEL_NUMBER 8
#define DIGITAL_OUT_ONE_KERNEL_NUMBER 9
#define RELAY_KERNEL_NUMBER 46

Configuration::Configuration()
{
	serverPort = DEFAULT_UDP_PORT_HEX;
	serverAddress = DEFAULT_HOME_SERVER;

	isBatteryStatusTrigger = false;
	isBatteryTriggerOnHigh = false;
	isDigital0Trigger = false;
	isDigital0TriggerOnHigh = false;
	isDigital1Trigger = false;
	isDigital1TriggerOnHigh = false;
	isDigital2Trigger = false;
	isDigital2TriggerOnHigh = false;
	isDigital3Trigger = false;
	isDigital3TriggerOnHigh = false;
	isDigital0High = false;
	isDigital1High = false;
	isRelayHigh = false;
	isVehicleVoltageTrigger = false;
	isAnalog0Trigger = false;
	isAnalog0TriggerMaximum = false;
	isAnalog1Trigger = false;
	isAnalog1TriggerMaximum = false;
	vehicleVoltageTriggerLevel = 0x3FFF;
	analog0TriggerLevel = 0x3FFF;
	analog1TriggerLevel = 0x3FFF;

	isDigital0TriggerTrans = false;
	isDigital1TriggerTrans = false;
	isDigital2TriggerTrans = false;
	isDigital3TriggerTrans = false;

	wasBatteryTripped = false;
	wasDigital0Tripped = false;
	wasDigital1Tripped = false;
	wasDigital2Tripped = false;
	wasDigital3Tripped = false;
	wasAnalog0Tripped = false;
	wasAnalog1Tripped = false;
	wasVehicleVoltageTripped = false;

	wasDigital0High = false;
	wasDigital1High = false;
	wasDigital2High = false;
	wasDigital3High = false;
}

void Configuration::loadConfiguration()
{
	std::fstream inputFile;
	std::string temporary;
	std::vector<std::string> stringArray;

#ifdef WINDOWS_IMPLEMENTATION
	inputFile.open("configuration.txt", std::ios::in);
#else
	inputFile.open("/opt/chainlink/configuration.txt", std::ios::in);
#endif // WINDOWS_IMPLEMENTATION
	if (!inputFile.is_open()) {
		std::cout << "ERROR [void Configuration::loadConfiguration()]: ";
		std::cout << "Unable to open file configuration.txt";
		std::cout << std::endl;
		std::cout << "A new file will be created.";
		std::cout << std::endl;
		saveConfiguration();
		resetConfiguration();
		return;
	}
	while (std::getline(inputFile, temporary)) {
		stringArray.push_back(temporary);
	}
	stringArray = defaultConfigurationArray(stringArray);
	// Previously started at 9, set to start at 0
	setIsBatteryStatusTrigger(stringToBoolean(stringArray[0]));
	setIsBatteryTriggerOnHigh(stringToBoolean(stringArray[1]));
	setIsDigital0Trigger(stringToBoolean(stringArray[2]));
	setIsDigital0TriggerOnHigh(stringToBoolean(stringArray[3]));
	setIsDigital1Trigger(stringToBoolean(stringArray[4]));
	setIsDigital1TriggerOnHigh(stringToBoolean(stringArray[5]));
	setIsDigital2Trigger(stringToBoolean(stringArray[6]));
	setIsDigital2TriggerOnHigh(stringToBoolean(stringArray[7]));
	setIsDigital3Trigger(stringToBoolean(stringArray[8]));
	setIsDigital3TriggerOnHigh(stringToBoolean(stringArray[9]));
	setIsDigital0High(stringToBoolean(stringArray[10]));
	setIsDigital1High(stringToBoolean(stringArray[11]));
	setIsRelayHigh(stringToBoolean(stringArray[12]));
	setIsVehicleVoltageTrigger(stringToBoolean(stringArray[13]));
	setIsAnalog0Trigger(stringToBoolean(stringArray[14]));
	setIsAnalog0TriggerMaximum(stringToBoolean(stringArray[15]));
	setIsAnalog1Trigger(stringToBoolean(stringArray[16]));
	setIsAnalog1TriggerMaximum(stringToBoolean(stringArray[17]));

	setVehicleVoltageTriggerLevel(std::stoi(stringArray[18]));
	setAnalog0TriggerLevel(std::stoi(stringArray[19]));
	setAnalog1TriggerLevel(std::stoi(stringArray[20]));

	setIsDigital0TriggerTrans(stringToBoolean(stringArray[21]));
	setIsDigital1TriggerTrans(stringToBoolean(stringArray[22]));
	setIsDigital2TriggerTrans(stringToBoolean(stringArray[23]));
	setIsDigital3TriggerTrans(stringToBoolean(stringArray[24]));

	Position temp;
	int i, j;
	float k;
	char l;
	geofence.setIsActive(stringToBoolean(stringArray[25]));
	temp.setDegrees(stringToBoolean(stringArray[26]));
	i = stringToInteger(stringArray[27]);
	j = stringToInteger(stringArray[28]);
	k = j;
	k = k / 1000.0;
	k += i;
	temp.setMinutes(k);
	l = stringArray[29][0];
	temp.setDirection(l);
	geofence.setReferenceLatitude(temp);
	temp.setDegrees(stringToBoolean(stringArray[30]));
	i = stringToInteger(stringArray[31]);
	j = stringToInteger(stringArray[32]);
	k = j;
	k = k / 1000.0;
	k += i;
	temp.setMinutes(k);
	l = stringArray[33][0];
	temp.setDirection(l);
	geofence.setReferenceLatitude(temp);
	geofence.setMaximumDistance(stringToDouble(stringArray[34]));

	setServerAddress(stringArray[35]);
	setServerPort(stringArray[36]);

	configureOutputs();
}

void Configuration::parseConfiguration(std::string input)
{
	std::vector<std::string> message;
	std::vector<std::string> preamble;
	std::vector<std::string> postamble;
	InstructionType key;

	if (input.compare("OK\x04\x04>") == 0) {
		std::cout << "Aborting text parsing. Incorrect message.";
		std::cout << std::endl;
		return;
	}
	std::cout << "In parse configuration. Input: " << input;
	message = splitString(input, ',');
	if (message.size() > 0) {
		preamble = splitString(message[0], ':');
		if (preamble.size() > 1) {
			message[0] = preamble[1];
			preamble[0] += ':';
			message.insert(message.begin(), preamble[0]);
		}
		postamble = splitString(message[message.size() - 1], '$');
		if (postamble.size() > 1) {
			message[message.size() - 1] = postamble[0];
		}
	}
	if (message[0].size() > 2) {
		if ((message[0])[0] == 'O' && (message[0])[1] == 'K') {
			std::string temporary;
			for (int i = 0; i < message[0].size(); i++) {
				if (i > 1) {
					temporary.push_back((message[0])[i]);
				}
			}
			message[0] = temporary;
		}
	}
	std::cout << "Messages:" << std::endl;
	for (int i = 0; i < message.size(); i++) {
		std::cout << message[i] << std::endl;
	}
	if (message.size() > 2) {
		/*
		int lastString;
		int lastChar;
		lastString = message.size() - 1;
		if ((message[lastString]).size() == 1) {
			if ((message[lastString])[0] == '>') {
				message.pop_back();
			}
		}
		lastString = message.size() - 1;
		lastChar = (message[lastString]).size() - 1;
		if ((message[lastString])[lastChar] == '$') {
			(message[lastString]).pop_back();
		}
		std::cout << "Processed message: " << std::endl;
		for (int i = 0; i < message.size(); i++) {
			std::cout << message[i] << std::endl;
		}
		*/

		if ((message[0]).compare("@CONN:") == 0) {
			key = CONN;
		}
		if ((message[0]).compare("@IMEI:") == 0) {
			key = IMEI;
		}
		if ((message[0]).compare("@CONF:") == 0) {
			key = CONF;
		}
		if ((message[0]).compare("@TEXT:") == 0) {
			key = TEXT;
		}
		if ((message[0]).compare("@EROR:") == 0) {
			key = EROR;
		}
		if ((message[0]).compare("@PHON:") == 0) {
			key = PHON;
		}
		if ((message[0]).compare("@SERV:") == 0) {
			key = SERV;
		}
		if ((message[0]).compare("@PORT:") == 0) {
			key = PORT;
		}
		if ((message[0]).compare("@GEOF:") == 0) {
			key = GEOF;
		}
		if ((message[0]).compare("@TIME:") == 0) {
			key = TIME;
		}
		if ((message[0]).compare("@PROG:") == 0) {
			key = PROG;
		}
		if ((message[0]).compare("@TELE:") == 0) {
			key = TELE;
		}

		switch (key) {
		case CONN:
			break;
		case IMEI:
			break;
		case CONF:
			decodeConfiguration(message);
			saveConfiguration();
			break;
		case TEXT:
			break;
		case EROR:
			break;
		case PHON:
			break;
		case SERV:
			setServerAddress(message[1]);
			break;
		case PORT:
			setServerPort(message[1]);
			break;
		case GEOF:
			break;
		case TIME:
			setReportTiming(stringToInteger(message[1], 600));
			break;
		case PROG:
			break;
		case TELE:
			break;
		default:
			break;
		}
	}
}

void Configuration::resetConfiguration()
{
	Geofence myGeofence;

	setGeofence(myGeofence);
	setServerAddress(DEFAULT_HOME_SERVER);
	setServerPort(DEFAULT_UDP_PORT_HEX);

	setIsBatteryStatusTrigger(false);
	setIsBatteryTriggerOnHigh(false);
	setIsDigital0Trigger(false);
	setIsDigital0TriggerOnHigh(false);
	setIsDigital1Trigger(false);
	setIsDigital1TriggerOnHigh(false);
	setIsDigital2Trigger(false);
	setIsDigital2TriggerOnHigh(false);
	setIsDigital3Trigger(false);
	setIsDigital3TriggerOnHigh(false);
	setIsDigital0High(false);
	setIsDigital1High(false);
	setIsRelayHigh(false);
	setIsVehicleVoltageTrigger(false);
	setIsAnalog0Trigger(false);
	setIsAnalog0TriggerMaximum(false);
	setIsAnalog1Trigger(false);
	setIsAnalog1TriggerMaximum(false);
	setVehicleVoltageTriggerLevel(0x3FFF); // Set to maximum
	setAnalog0TriggerLevel(0x3FFF); // Set to maximum
	setAnalog1TriggerLevel(0x3FFF); // Set to maximum

	setIsDigital0TriggerTrans(false);
	setIsDigital1TriggerTrans(false);
	setIsDigital2TriggerTrans(false);
	setIsDigital3TriggerTrans(false);

	configureOutputs();
}

void Configuration::saveConfiguration()
{
	std::fstream outputFile;
	std::string temporary;
	std::vector<std::string> stringArray;

#ifdef WINDOWS_IMPLEMENTATION
	outputFile.open("configuration.txt", std::ios::out | std::ios::trunc);
#else
	outputFile.open("/opt/chainlink/configuration.txt", std::ios::out | std::ios::trunc);
#endif // WINDOWS_IMPLEMENTATION
	if (!outputFile.is_open()) {
		std::cout << "ERROR [void Configuration::saveConfiguration()]: ";
		std::cout << "Unable to open file configuration.txt";
		std::cout << std::endl;
		resetConfiguration();
		return;
	}
	outputFile << booleanToString(getIsBatteryStatusTrigger()) << std::endl; // Line 1
	outputFile << booleanToString(getIsBatteryTriggerOnHigh()) << std::endl; // Line 2
	outputFile << booleanToString(getIsDigital0Trigger()) << std::endl; // Line 3
	outputFile << booleanToString(getIsDigital0TriggerOnHigh()) << std::endl; // Line 4
	outputFile << booleanToString(getIsDigital1Trigger()) << std::endl; // Line 5
	outputFile << booleanToString(getIsDigital1TriggerOnHigh()) << std::endl; // Line 6
	outputFile << booleanToString(getIsDigital2Trigger()) << std::endl; // Line 7
	outputFile << booleanToString(getIsDigital2TriggerOnHigh()) << std::endl; // Line 8
	outputFile << booleanToString(getIsDigital3Trigger()) << std::endl; // Line 9
	outputFile << booleanToString(getIsDigital3TriggerOnHigh()) << std::endl; // Line 10
	outputFile << booleanToString(getIsDigital0High()) << std::endl; // Line 11
	outputFile << booleanToString(getIsDigital1High()) << std::endl; // Line 12
	outputFile << booleanToString(getIsRelayHigh()) << std::endl; // Line 13
	outputFile << booleanToString(getIsVehicleVoltageTrigger()) << std::endl; // Line 14
	outputFile << booleanToString(getIsAnalog0Trigger()) << std::endl; // Line 15
	outputFile << booleanToString(getIsAnalog0TriggerMaximum()) << std::endl; // Line 16
	outputFile << booleanToString(getIsAnalog1Trigger()) << std::endl; // Line 17
	outputFile << booleanToString(getIsAnalog1TriggerMaximum()) << std::endl; // Line 18
	outputFile << getVehicleVoltageTriggerLevel() << std::endl; // Line 19
	outputFile << getAnalog0TriggerLevel() << std::endl; // Line 20
	outputFile << getAnalog1TriggerLevel() << std::endl; // Line 21

	outputFile << booleanToString(getIsDigital0TriggerTrans()) << std::endl; // Line 22
	outputFile << booleanToString(getIsDigital1TriggerTrans()) << std::endl; // Line 23
	outputFile << booleanToString(getIsDigital2TriggerTrans()) << std::endl; // Line 24
	outputFile << booleanToString(getIsDigital3TriggerTrans()) << std::endl; // Line 25

	int i;
	float j;
	outputFile << booleanToString(geofence.getIsActive()) << std::endl; // Line 26
	outputFile << (geofence.getReferenceLatitude()).getDegrees() << std::endl; // Line 27
	j = (geofence.getReferenceLatitude()).getMinutes();
	i = j;
	j = j - i;
	outputFile << i << std::endl; // Line 28
	outputFile << j << std::endl; // Line 29
	outputFile << (geofence.getReferenceLatitude()).getDirection() << std::endl; // Line 30

	outputFile << (geofence.getReferenceLongitude()).getDegrees() << std::endl; // Line 31
	j = (geofence.getReferenceLongitude()).getMinutes();
	i = j;
	j = j - i;
	outputFile << i << std::endl; // Line 32
	outputFile << j << std::endl; // Line 33
	outputFile << (geofence.getReferenceLongitude()).getDirection() << std::endl; // Line 34
	outputFile << geofence.getMaximumDistance(); // Line 35
}

Geofence Configuration::getGeofence() { return geofence; }
std::string Configuration::getServerPort() { return serverPort; }
std::string Configuration::getServerAddress() { return serverAddress; }
int Configuration::getReportTiming() { return reportTiming; }

bool Configuration::getIsBatteryStatusTrigger() { return isBatteryStatusTrigger; }
bool Configuration::getIsBatteryTriggerOnHigh() { return isBatteryTriggerOnHigh; }
bool Configuration::getIsDigital0Trigger() { return isDigital0Trigger; }
bool Configuration::getIsDigital0TriggerOnHigh() { return isDigital0TriggerOnHigh; }
bool Configuration::getIsDigital1Trigger() { return isDigital1Trigger; }
bool Configuration::getIsDigital1TriggerOnHigh() { return isDigital1TriggerOnHigh; }
bool Configuration::getIsDigital2Trigger() { return isDigital2Trigger; }
bool Configuration::getIsDigital2TriggerOnHigh() { return isDigital2TriggerOnHigh; }
bool Configuration::getIsDigital3Trigger() { return isDigital3Trigger; }
bool Configuration::getIsDigital3TriggerOnHigh() { return isDigital3TriggerOnHigh; }
bool Configuration::getIsDigital0High() { return isDigital0High; }
bool Configuration::getIsDigital1High() { return isDigital1High; }
bool Configuration::getIsRelayHigh() { return isRelayHigh; }
bool Configuration::getIsVehicleVoltageTrigger() { return isVehicleVoltageTrigger; }
bool Configuration::getIsAnalog0Trigger() { return isAnalog0Trigger; }
bool Configuration::getIsAnalog0TriggerMaximum() { return isAnalog0TriggerMaximum; }
bool Configuration::getIsAnalog1Trigger() { return isAnalog1Trigger; }
bool Configuration::getIsAnalog1TriggerMaximum() { return isAnalog1TriggerMaximum; }
int Configuration::getVehicleVoltageTriggerLevel() { return vehicleVoltageTriggerLevel; }
int Configuration::getAnalog0TriggerLevel() { return analog0TriggerLevel; }
int Configuration::getAnalog1TriggerLevel() { return analog1TriggerLevel; }

bool Configuration::getIsDigital0TriggerTrans() { return isDigital0TriggerTrans; }
bool Configuration::getIsDigital1TriggerTrans() { return isDigital1TriggerTrans; }
bool Configuration::getIsDigital2TriggerTrans() { return isDigital2TriggerTrans; }
bool Configuration::getIsDigital3TriggerTrans() { return isDigital3TriggerTrans; }

bool Configuration::getWasBatteryTripped() { return wasBatteryTripped; }
bool Configuration::getWasDigital0Tripped() { return wasDigital0Tripped; }
bool Configuration::getWasDigital1Tripped() { return wasDigital1Tripped; }
bool Configuration::getWasDigital2Tripped() { return wasDigital2Tripped; }
bool Configuration::getWasDigital3Tripped() { return wasDigital3Tripped; }
bool Configuration::getWasAnalog0Tripped() { return wasAnalog0Tripped; }
bool Configuration::getWasAnalog1Tripped() { return wasAnalog1Tripped; }
bool Configuration::getWasVehicleVoltageTripped() { return wasVehicleVoltageTripped; }

bool Configuration::getWasDigital0High() { return wasDigital0High; }
bool Configuration::getWasDigital1High() { return wasDigital1High; }
bool Configuration::getWasDigital2High() { return wasDigital2High; }
bool Configuration::getWasDigital3High() { return wasDigital3High; }

void Configuration::setGeofence(Geofence input) { geofence = input; }
void Configuration::setServerPort(std::string input) { serverPort = input; }
void Configuration::setServerAddress(std::string input) { serverAddress = input; }
void Configuration::setReportTiming(int input) { reportTiming = input; }

void Configuration::setIsBatteryStatusTrigger(bool input) { isBatteryStatusTrigger = input; }
void Configuration::setIsBatteryTriggerOnHigh(bool input) { isBatteryTriggerOnHigh = input; }
void Configuration::setIsDigital0Trigger(bool input) { isDigital0Trigger = input; }
void Configuration::setIsDigital0TriggerOnHigh(bool input) { isDigital0TriggerOnHigh = input; }
void Configuration::setIsDigital1Trigger(bool input) { isDigital1Trigger = input; }
void Configuration::setIsDigital1TriggerOnHigh(bool input) { isDigital1TriggerOnHigh = input; }
void Configuration::setIsDigital2Trigger(bool input) { isDigital2Trigger = input; }
void Configuration::setIsDigital2TriggerOnHigh(bool input) { isDigital2TriggerOnHigh = input; }
void Configuration::setIsDigital3Trigger(bool input) { isDigital3Trigger = input; }
void Configuration::setIsDigital3TriggerOnHigh(bool input) { isDigital3TriggerOnHigh = input; }
void Configuration::setIsDigital0High(bool input) { isDigital0High = input; }
void Configuration::setIsDigital1High(bool input) { isDigital1High = input; }
void Configuration::setIsRelayHigh(bool input) { isRelayHigh = input; }
void Configuration::setIsVehicleVoltageTrigger(bool input) { isVehicleVoltageTrigger = input; }
void Configuration::setIsAnalog0Trigger(bool input) { isAnalog0Trigger = input; }
void Configuration::setIsAnalog0TriggerMaximum(bool input) { isAnalog0TriggerMaximum = input; }
void Configuration::setIsAnalog1Trigger(bool input) { isAnalog1Trigger = input; }
void Configuration::setIsAnalog1TriggerMaximum(bool input) { isAnalog1TriggerMaximum = input; }
void Configuration::setVehicleVoltageTriggerLevel(int input) { vehicleVoltageTriggerLevel = input; }
void Configuration::setAnalog0TriggerLevel(int input) { analog0TriggerLevel = input; }
void Configuration::setAnalog1TriggerLevel(int input) { analog1TriggerLevel = input; }

void Configuration::setIsDigital0TriggerTrans(bool input) { isDigital0TriggerTrans = input; }
void Configuration::setIsDigital1TriggerTrans(bool input) { isDigital1TriggerTrans = input; }
void Configuration::setIsDigital2TriggerTrans(bool input) { isDigital2TriggerTrans = input; }
void Configuration::setIsDigital3TriggerTrans(bool input) { isDigital3TriggerTrans = input; }

void Configuration::setWasBatteryTripped(bool input) { wasBatteryTripped = input; }
void Configuration::setWasDigital0Tripped(bool input) { wasDigital0Tripped = input; }
void Configuration::setWasDigital1Tripped(bool input) { wasDigital1Tripped = input; }
void Configuration::setWasDigital2Tripped(bool input) { wasDigital2Tripped = input; }
void Configuration::setWasDigital3Tripped(bool input) { wasDigital3Tripped = input; }
void Configuration::setWasAnalog0Tripped(bool input) { wasAnalog0Tripped = input; }
void Configuration::setWasAnalog1Tripped(bool input) { wasAnalog1Tripped = input; }
void Configuration::setWasVehicleVoltageTripped(bool input) { wasVehicleVoltageTripped = input; }

void Configuration::setWasDigital0High(bool input) { wasDigital0High = input; }
void Configuration::setWasDigital1High(bool input) { wasDigital1High = input; }
void Configuration::setWasDigital2High(bool input) { wasDigital2High = input; }
void Configuration::setWasDigital3High(bool input) { wasDigital3High = input; }

void Configuration::configureOutputs()
{
	if (getIsDigital0High() == true) {
		GpioPin digitalOutZero;

		digitalOutZero.setLinuxKernelNumber(DIGITAL_OUT_ZERO_KERNEL_NUMBER);
		digitalOutZero.write(GPIO_PIN_OUTPUT_HIGH);
	} else {
		GpioPin digitalOutZero;

		digitalOutZero.setLinuxKernelNumber(DIGITAL_OUT_ZERO_KERNEL_NUMBER);
		digitalOutZero.write(GPIO_PIN_OUTPUT_LOW);
	}
	if (getIsDigital1High() == true) {
		GpioPin digitalOutOne;

		digitalOutOne.setLinuxKernelNumber(DIGITAL_OUT_ONE_KERNEL_NUMBER);
		digitalOutOne.write(GPIO_PIN_OUTPUT_HIGH);
	} else {
		GpioPin digitalOutOne;

		digitalOutOne.setLinuxKernelNumber(DIGITAL_OUT_ONE_KERNEL_NUMBER);
		digitalOutOne.write(GPIO_PIN_OUTPUT_LOW);
	}
	if (getIsRelayHigh() == true) {
		GpioPin relay;

		relay.setLinuxKernelNumber(RELAY_KERNEL_NUMBER);
		relay.write(GPIO_PIN_OUTPUT_HIGH);
	} else {
		GpioPin relay;

		relay.setLinuxKernelNumber(RELAY_KERNEL_NUMBER);
		relay.write(GPIO_PIN_OUTPUT_LOW);
	}
}

void Configuration::decodeConfiguration(std::vector<std::string> message)
{
	DeviceSelector selector;
	DeviceState state;
	DeviceCutoff cutoff;
	int value;

	selector = SELECTOR_UNKOWN;
	state = STATE_UNKOWN;
	cutoff = CUTOFF_UNKOWN;
	value = 0;

	std::cout << "In decode configuration: ";
	if (message.size() > 2) {
		if ((message[1]).compare("DI0") == 0) {
			selector = DI0;
			std::cout << "selector DI0, ";
		}
		if ((message[1]).compare("DI1") == 0) {
			selector = DI1;
			std::cout << "selector DI1, ";
		}
		if ((message[1]).compare("DI2") == 0) {
			selector = DI2;
			std::cout << "selector DI2, ";
		}
		if ((message[1]).compare("DI3") == 0) {
			selector = DI3;
			std::cout << "selector DI3, ";
		}
		if ((message[1]).compare("DO0") == 0) {
			selector = DO0;
			std::cout << "selector DO0, ";
		}
		if ((message[1]).compare("DO1") == 0) {
			selector = DO1;
			std::cout << "selector DO1, ";
		}
		if ((message[1]).compare("AI0") == 0) {
			selector = AI0;
			std::cout << "selector AI0, ";
		}
		if ((message[1]).compare("AI1") == 0) {
			selector = AI1;
			std::cout << "selector AI1";
		}
		if ((message[1]).compare("RLY") == 0) {
			selector = RLY;
			std::cout << "selector RLY";
		}
		if ((message[1]).compare("RST") == 0) {
			selector = RST;
		}
		if ((message[1]).compare("GEO") == 0) {
			selector = GEO;
		}
		if ((message[1]).compare("SRV") == 0) {
			selector = SRV;
		}
		if ((message[1]).compare("PRT") == 0) {
			selector = PRT;
		}

		if ((message[2]).compare("ON") == 0) {
			state = DEVICE_ON;
			std::cout << "state on, ";
		}
		if ((message[2]).compare("OFF") == 0) {
			state = DEVICE_OFF;
			std::cout << "state off, ";
		}
	}

	if (message.size() > 3) {
		if ((message[3]).compare("HIGH") == 0) {
			cutoff = DEVICE_HIGH;
			std::cout << "cutoff high, ";
		}
		if ((message[3]).compare("LOW") == 0) {
			cutoff = DEVICE_LOW;
			std::cout << "cutoff low, ";
		}
		if ((message[3]).compare("TRANS") == 0) {
			cutoff = DEVICE_TRANS;
			std::cout << "cutoff trans, ";
		}
	}

	if (message.size() > 4) {
		value = std::stoi(message[4]);
	}
	std::cout << std::endl;
	setConfiguration(selector, state, cutoff, value);
}

void Configuration::setConfiguration(DeviceSelector selector, DeviceState state, DeviceCutoff cutoff, int value)
{
	std::cout << "In set configuration." << std::endl;
	switch (selector) {
	case DI0:
		if (state == DEVICE_ON) {
			setIsDigital0Trigger(true);
		} else if (state == DEVICE_OFF) {
			setIsDigital0Trigger(false);
		}
		if (cutoff == DEVICE_HIGH) {
			setIsDigital0TriggerOnHigh(true);
			setIsDigital0TriggerTrans(false);
		} else if (cutoff == DEVICE_LOW) {
			setIsDigital0TriggerOnHigh(false);
			setIsDigital0TriggerTrans(false);
		} else if (cutoff == DEVICE_TRANS) {
			setIsDigital0TriggerTrans(true);
			setIsDigital0TriggerOnHigh(false);
		}
		break;
	case DI1:
		if (state == DEVICE_ON) {
			setIsDigital1Trigger(true);
		} else if (state == DEVICE_OFF) {
			setIsDigital1Trigger(false);
		}
		if (cutoff == DEVICE_HIGH) {
			setIsDigital1TriggerOnHigh(true);
			setIsDigital1TriggerTrans(false);
		} else if (cutoff == DEVICE_LOW) {
			setIsDigital1TriggerOnHigh(false);
			setIsDigital1TriggerTrans(false);
		} else if (cutoff == DEVICE_TRANS) {
			setIsDigital1TriggerTrans(true);
			setIsDigital1TriggerOnHigh(false);
		}
		break;
	case DI2:
		if (state == DEVICE_ON) {
			setIsDigital2Trigger(true);
		} else if (state == DEVICE_OFF) {
			setIsDigital2Trigger(false);
		}
		if (cutoff == DEVICE_HIGH) {
			setIsDigital2TriggerOnHigh(true);
			setIsDigital2TriggerTrans(false);
		} else if (cutoff == DEVICE_LOW) {
			setIsDigital2TriggerOnHigh(false);
			setIsDigital2TriggerTrans(false);
		} else if (cutoff == DEVICE_TRANS) {
			setIsDigital2TriggerTrans(true);
			setIsDigital2TriggerOnHigh(false);
		}
		break;
	case DI3:
		if (state == DEVICE_ON) {
			setIsDigital3Trigger(true);
		} else if (state == DEVICE_OFF) {
			setIsDigital3Trigger(false);
		}
		if (cutoff == DEVICE_HIGH) {
			setIsDigital3TriggerOnHigh(true);
			setIsDigital3TriggerTrans(false);
		} else if (cutoff == DEVICE_LOW) {
			setIsDigital3TriggerOnHigh(false);
			setIsDigital3TriggerTrans(false);
		} else if (cutoff == DEVICE_TRANS) {
			setIsDigital3TriggerTrans(true);
			setIsDigital3TriggerOnHigh(false);
		}
		break;
	case DO0:
		if (state == DEVICE_ON) {
			GpioPin digitalOutZero;

			digitalOutZero.setLinuxKernelNumber(DIGITAL_OUT_ZERO_KERNEL_NUMBER);
			digitalOutZero.write(GPIO_PIN_OUTPUT_HIGH);
			setIsDigital0High(true);
			std::cout << "DO0 set high." << std::endl;
		} else if (state == DEVICE_OFF) {
			GpioPin digitalOutZero;

			digitalOutZero.setLinuxKernelNumber(DIGITAL_OUT_ZERO_KERNEL_NUMBER);
			digitalOutZero.write(GPIO_PIN_OUTPUT_LOW);
			setIsDigital0High(false);
			std::cout << "DO0 set low." << std::endl;
		}
		break;
	case DO1:
		if (state == DEVICE_ON) {
			GpioPin digitalOutOne;

			digitalOutOne.setLinuxKernelNumber(DIGITAL_OUT_ONE_KERNEL_NUMBER);
			digitalOutOne.write(GPIO_PIN_OUTPUT_HIGH);
			setIsDigital1High(true);
			std::cout << "DO1 set high." << std::endl;
		} else if (state == DEVICE_OFF) {
			GpioPin digitalOutOne;

			digitalOutOne.setLinuxKernelNumber(DIGITAL_OUT_ONE_KERNEL_NUMBER);
			digitalOutOne.write(GPIO_PIN_OUTPUT_LOW);
			setIsDigital1High(false);
			std::cout << "DO1 set low." << std::endl;
		}
		break;
	case AI0:
		if (state == DEVICE_ON) {
			setIsAnalog0Trigger(true);
		} else if (state == DEVICE_OFF) {
			setIsAnalog0Trigger(false);
		}
		if (cutoff == DEVICE_HIGH) {
			setIsAnalog0TriggerMaximum(true);
		} else if (cutoff == DEVICE_LOW) {
			setIsAnalog0TriggerMaximum(false);
		}
		setAnalog0TriggerLevel(value);
		break;
	case AI1:
		if (state == DEVICE_ON) {
			setIsAnalog1Trigger(true);
		} else if (state == DEVICE_OFF) {
			setIsAnalog1Trigger(false);
		}
		if (cutoff == DEVICE_HIGH) {
			setIsAnalog1TriggerMaximum(true);
		} else if (cutoff == DEVICE_LOW) {
			setIsAnalog1TriggerMaximum(false);
		}
		setAnalog1TriggerLevel(value);
		break;
	case RLY:
		if (state == DEVICE_ON) {
			GpioPin relay;

			relay.setLinuxKernelNumber(RELAY_KERNEL_NUMBER);
			relay.write(GPIO_PIN_OUTPUT_HIGH);
			setIsRelayHigh(true);
		} else if (state == DEVICE_OFF) {
			GpioPin relay;

			relay.setLinuxKernelNumber(RELAY_KERNEL_NUMBER);
			relay.write(GPIO_PIN_OUTPUT_LOW);
			setIsRelayHigh(false);
		}
		break;
	case RST:
		break;
	case GEO:
		break;
	case SRV:
		
		break;
	case PRT:
		break;
	default:
		break;
	}
}

/*
The given numeric values are line numbers.


MEMORY MAP
----------
1 - 25		Device configuration
26 - 35		Geofence information
36 - 37		Cellular configuration
38 - 45		First Run Code

Detailed MEMORY MAP
-------------------
(Device configuration)
DEVICE_MEMORY_OFFSET 0
1 - isBatteryStatusTrigger
2 - isBatteryTriggerOnHigh
3 - isDigital0Trigger
4 - isDigital0TriggerOnHigh
5 - isDigital1Trigger
6 - isDigital1TriggerOnHigh
7 - isDigital2Trigger
8 - isDigital2TriggerOnHigh
9 - isDigital3Trigger
10 - isDigital3TriggerOnHigh
11 - isDigital0High
12 - isDigital1High
13 - isRelayHigh
14 - isVehicleVoltageTrigger
15 - isAnalog0Trigger
16 - isAnalog0TriggerMaximum
17 - isAnalog1Trigger
18 - isAnalog1TriggerMaximum
19 - vehicleVoltageTriggerLevel
20 - analog0TriggerLevel
21 - analog1TriggerLevel
22 - isDigital0TriggerTrans
23 - isDigital1TriggerTrans
24 - isDigital2TriggerTrans
25 - isDigital3TriggerTrans
(Geofencing)
GEOFENCE_MEMORY_OFFSET 25
26 - isGeofenceEnabled
27 - latitudeDegrees
28 - latitudeMinutes (Whole)
29 - latitudeMinuteFractions
30 - latitudeDirection
31 - longitudeDegrees
32 - longitudeMinutes (Whole)
33 - longitudeMinuteFractions
34 - longitudeDirection
35 - maximumDistance
(Cellular)
CELL_MEMORY_OFFSET 35
36 - serverAddress
37 - serverPort
(First Run Code)
FIRST_RUN_CODE_OFFSET 37
38 - 45 Key
*/

std::vector<std::string> defaultConfigurationArray(std::vector<std::string> input)
{
	int arraySize;

	arraySize = input.size();
	switch (arraySize) {
	case 0:
		input.push_back("0");
	case 1:
		input.push_back("0");
	case 2:
		input.push_back("0");
	case 3:
		input.push_back("0");
	case 4:
		input.push_back("0");
	case 5:
		input.push_back("0");
	case 6:
		input.push_back("0");
	case 7:
		input.push_back("0");
	case 8:
		input.push_back("0");
	case 9:
		input.push_back("0");
	case 10:
		input.push_back("0");
	case 11:
		input.push_back("0");
	case 12:
		input.push_back("0");
	case 13:
		input.push_back("0");
	case 14:
		input.push_back("0");
	case 15:
		input.push_back("0");
	case 16:
		input.push_back("0");
	case 17:
		input.push_back("0");
	case 18:
		input.push_back("16383");
	case 19:
		input.push_back("16383");
	case 20:
		input.push_back("16383");
	case 21:
		input.push_back("0");
	case 22:
		input.push_back("0");
	case 23:
		input.push_back("0");
	case 24:
		input.push_back("0");
	case 25:
		input.push_back("0");
	case 26:
		input.push_back("0");
	case 27:
		input.push_back("0");
	case 28:
		input.push_back("0");
	case 29:
		input.push_back("N");
	case 30:
		input.push_back("0");
	case 31:
		input.push_back("0");
	case 32:
		input.push_back("0");
	case 33:
		input.push_back("W");
	case 34:
		input.push_back("0");
	case 35:
		input.push_back(DEFAULT_HOME_SERVER);
	case 36:
		input.push_back(DEFAULT_UDP_PORT_HEX);
	case 37:
		input.push_back("0");
	case 38:
		input.push_back("0");
	case 39:
		input.push_back("0");
	case 40:
		input.push_back("0");
	case 41:
		input.push_back("0");
	case 42:
		input.push_back("0");
	case 43:
		input.push_back("0");
	case 44:
		input.push_back("0");
	default:
		break;
	}

	return input;
}

bool checkEvents(Configuration &myConfiguration)
{
	bool isTriggered;
	int i, j;
	char k;

	GpioPin digitalInput0;
	GpioPin digitalInput1;
	GpioPin digitalInput2;
	GpioPin digitalInput3;
	AdcPin analogInput0(ANALOG_0_CHIP, ANALOG_0_CHANNEL);
	AdcPin analogInput1(ANALOG_1_CHIP, ANALOG_1_CHANNEL);

	digitalInput0.setLinuxKernelNumber(133);
	digitalInput1.setLinuxKernelNumber(134);
	digitalInput2.setLinuxKernelNumber(135);
	digitalInput3.setLinuxKernelNumber(136);

	isTriggered = false;

	if (myConfiguration.getIsAnalog0Trigger()) {
		j = analogInput0.read();
		i = myConfiguration.getAnalog0TriggerLevel();
		if (myConfiguration.getIsAnalog0TriggerMaximum() == true) {
			if (j > i) {
				if (myConfiguration.getWasAnalog0Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasAnalog0Tripped(true);
				}
			} else {
				myConfiguration.setWasAnalog0Tripped(false);
			}
		} else if (myConfiguration.getIsAnalog0TriggerMaximum() == false) {
			if (j < i) {
				if (myConfiguration.getWasAnalog0Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasAnalog0Tripped(true);
				}
			} else {
				myConfiguration.setWasAnalog0Tripped(false);
			}
		}
	}
	if (myConfiguration.getIsAnalog1Trigger()) {
		j = analogInput1.read();
		i = myConfiguration.getAnalog1TriggerLevel();
		if (myConfiguration.getIsAnalog1TriggerMaximum() == true) {
			if (j > i) {
				if (myConfiguration.getWasAnalog1Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasAnalog1Tripped(true);
				}
			} else {
				myConfiguration.setWasAnalog1Tripped(false);
			}
		} else if (myConfiguration.getIsAnalog1TriggerMaximum() == false) {
			if (j < i) {
				if (myConfiguration.getWasAnalog1Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasAnalog1Tripped(true);
				}
			} else {
				myConfiguration.setWasAnalog1Tripped(false);
			}
		}
	}
	if (myConfiguration.getIsDigital0Trigger()) { // Digital 0
		if (myConfiguration.getIsDigital0TriggerTrans()) {
			if (digitalInput0.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital0High()) {
					// The state has not changed, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital0High(true);
				}
			} else {
				if (myConfiguration.getWasDigital0High()) {
					isTriggered = true;
					myConfiguration.setWasDigital0High(false);
				} else {
					// The state has not changed, don't do anything
				}
			}
		} else if (myConfiguration.getIsDigital0TriggerOnHigh()) {
			if (digitalInput0.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital0Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital0Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital0Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital0Tripped(false);
				}
			}
		} else {
			if (digitalInput0.read()== GPIO_PIN_OUTPUT_LOW) {
				if (myConfiguration.getWasDigital0Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital0Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital0Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital0Tripped(false);
				}
			}
		}
	}
	if (myConfiguration.getIsDigital1Trigger()) { // Digital 1
		if (myConfiguration.getIsDigital1TriggerTrans()) {
			if (digitalInput1.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital1High()) {
					// The state has not changed, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital1High(true);
				}
			} else {
				if (myConfiguration.getWasDigital1High()) {
					isTriggered = true;
					myConfiguration.setWasDigital1High(false);
				} else {
					// The state has not changed, don't do anything
				}
			}
		} else if (myConfiguration.getIsDigital1TriggerOnHigh()) {
			if (digitalInput1.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital1Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital1Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital1Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital1Tripped(false);
				}
			}
		} else {
			if (digitalInput1.read()== GPIO_PIN_OUTPUT_LOW) {
				if (myConfiguration.getWasDigital1Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital1Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital1Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital1Tripped(false);
				}
			}
		}
	}
	if (myConfiguration.getIsDigital2Trigger()) { // Digital 0
		if (myConfiguration.getIsDigital2TriggerTrans()) {
			if (digitalInput2.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital2High()) {
					// The state has not changed, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital2High(true);
				}
			} else {
				if (myConfiguration.getWasDigital2High()) {
					isTriggered = true;
					myConfiguration.setWasDigital2High(false);
				} else {
					// The state has not changed, don't do anything
				}
			}
		} else if (myConfiguration.getIsDigital2TriggerOnHigh()) {
			if (digitalInput2.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital2Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital2Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital2Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital2Tripped(false);
				}
			}
		} else {
			if (digitalInput2.read()== GPIO_PIN_OUTPUT_LOW) {
				if (myConfiguration.getWasDigital2Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital2Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital2Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital2Tripped(false);
				}
			}
		}
	}
	if (myConfiguration.getIsDigital3Trigger()) { // Digital 0
		if (myConfiguration.getIsDigital3TriggerTrans()) {
			if (digitalInput3.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital3High()) {
					// The state has not changed, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital3High(true);
				}
			} else {
				if (myConfiguration.getWasDigital3High()) {
					isTriggered = true;
					myConfiguration.setWasDigital3High(false);
				} else {
					// The state has not changed, don't do anything
				}
			}
		} else if (myConfiguration.getIsDigital3TriggerOnHigh()) {
			if (digitalInput3.read() == GPIO_PIN_OUTPUT_HIGH) {
				if (myConfiguration.getWasDigital3Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital3Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital3Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital3Tripped(false);
				}
			}
		} else {
			if (digitalInput3.read()== GPIO_PIN_OUTPUT_LOW) {
				if (myConfiguration.getWasDigital3Tripped()) {
					// It is in the same state, don't do anything
				} else {
					isTriggered = true;
					myConfiguration.setWasDigital3Tripped(true);
				}
			} else {
				if (myConfiguration.getWasDigital3Tripped()) { // Clear tripped flag
					myConfiguration.setWasDigital3Tripped(false);
				}
			}
		}
	}

	return isTriggered;
}

bool stringToBoolean(std::string input)
{
	int output;

	try {
		output = std::stoi(input);
	} catch(std::invalid_argument& e) {
		std::cout << "ERROR [bool stringToBoolean(std::string input)]: ";
		std::cout << "std::stoi threw an instance of std::invalid_argument.";
		std::cout << "Unable to parse string: " << input;
		std::cout << std::endl;
		return false;
	}
	if (output == 1) {
		return true;
	} else if (output == 0) {
		return 0;
	}
	std::cout << "ERROR [bool stringToBoolean(std::string input)]: ";
	std::cout << "Unable to parse integer string: ";
	std::cout << input;
	std::cout << std::endl;
	return false;
}

std::string booleanToString(bool input)
{
	std::string output;

	if (input == true) {
		output = "1";
	} else {
		output = "0";
	}

	return output;
}

int stringToInteger(std::string input)
{
	int output;

	output = 0;
	try {
		output = std::stoi(input);
	} catch (const char *error) {
		std::cout << "Error [int stringToInteger(std::string)]: " << error;
		std::cout << std::endl;
		output = 0;
	}

	return output;
}

int stringToInteger(std::string input, int defaultNumber)
{
	int output;

	output = defaultNumber;
	try {
		output = std::stoi(input);
	}
	catch (const char *error) {
		std::cout << "Error [int stringToInteger(std::string)]: " << error;
		std::cout << std::endl;
		output = defaultNumber;
	}

	return output;
}

double stringToDouble(std::string input)
{
	double output;

	output = 0;
	try {
		output = std::stod(input);
	} catch (const char *error) {
		std::cout << "Error [double stringToDouble(std::string)]: " << error;
		std::cout << std::endl;
		output = 0;
	}

	return output;
}
