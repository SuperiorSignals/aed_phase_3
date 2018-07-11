#pragma once
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include <string>
#include <vector>

#include "geofence.h"

#define DEFAULT_HOME_SERVER "135.26.235.158"
#define DEFAULT_REPORT_SERVER "23.89.198.205"
#define DEFAULT_UDP_PORT "5180"
#define DEFAULT_UDP_PORT_HEX "143C"
#define KEY_1 "56"
#define KEY_2 "42"
#define KEY_3 "49"
#define KEY_4 "12"
#define KEY_5 "23"
#define KEY_6 "99"
#define KEY_7 "88"
#define KEY_8 "77"

class Configuration;

std::vector<std::string> defaultConfigurationArray(std::vector<std::string> input);
bool checkEvents(Configuration &myConfiguration);
bool stringToBoolean(std::string input);
std::string booleanToString(bool input);
int stringToInteger(std::string input);
int stringToInteger(std::string input, int defaultNumber);
double stringToDouble(std::string input);

enum InstructionType { UNASSIGNED, IMEI, CONN, CONF, TEXT, EROR, PHON, TELE, SERV, PORT, PROG, PRG3, ACCS, GEOF, TIME, TMR1, TMR2 };
enum DeviceSelector { DI0, DI1, DI2, DI3, DO0, DO1, AI0, AI1, RLY, RST, GEO, SRV, PRT, SELECTOR_UNKOWN };
enum DeviceState { DEVICE_ON, DEVICE_OFF, STATE_UNKOWN };
enum DeviceCutoff { DEVICE_HIGH, DEVICE_LOW, DEVICE_TRANS, CUTOFF_UNKOWN };

class Configuration {
public:
	Configuration();
	void loadConfiguration();
	void parseConfiguration(std::string input);
	void resetConfiguration();
	void saveConfiguration();

	bool getIsBatteryStatusTrigger();
	bool getIsBatteryTriggerOnHigh();
	bool getIsDigital0Trigger();
	bool getIsDigital0TriggerOnHigh();
	bool getIsDigital1Trigger();
	bool getIsDigital1TriggerOnHigh();
	bool getIsDigital2Trigger();
	bool getIsDigital2TriggerOnHigh();
	bool getIsDigital3Trigger();
	bool getIsDigital3TriggerOnHigh();
	bool getIsDigital0High();
	bool getIsDigital1High();
	bool getIsRelayHigh();
	bool getIsVehicleVoltageTrigger();
	bool getIsAnalog0Trigger();
	bool getIsAnalog0TriggerMaximum();
	bool getIsAnalog1Trigger();
	bool getIsAnalog1TriggerMaximum();
	int getVehicleVoltageTriggerLevel();
	int getAnalog0TriggerLevel();
	int getAnalog1TriggerLevel();

	bool getIsDigital0TriggerTrans();
	bool getIsDigital1TriggerTrans();
	bool getIsDigital2TriggerTrans();
	bool getIsDigital3TriggerTrans();

	bool getWasBatteryTripped();
	bool getWasDigital0Tripped();
	bool getWasDigital1Tripped();
	bool getWasDigital2Tripped();
	bool getWasDigital3Tripped();
	bool getWasAnalog0Tripped();
	bool getWasAnalog1Tripped();
	bool getWasVehicleVoltageTripped();

	bool getWasDigital0High();
	bool getWasDigital1High();
	bool getWasDigital2High();
	bool getWasDigital3High();

	Geofence getGeofence();
	std::string getServerPort();
	std::string getServerAddress();
	int getReportTiming();

	void setIsBatteryStatusTrigger(bool input);
	void setIsBatteryTriggerOnHigh(bool input);
	void setIsDigital0Trigger(bool input);
	void setIsDigital0TriggerOnHigh(bool input);
	void setIsDigital1Trigger(bool input);
	void setIsDigital1TriggerOnHigh(bool input);
	void setIsDigital2Trigger(bool input);
	void setIsDigital2TriggerOnHigh(bool input);
	void setIsDigital3Trigger(bool input);
	void setIsDigital3TriggerOnHigh(bool input);
	void setIsDigital0High(bool input);
	void setIsDigital1High(bool input);
	void setIsRelayHigh(bool input);
	void setIsVehicleVoltageTrigger(bool input);
	void setIsAnalog0Trigger(bool input);
	void setIsAnalog0TriggerMaximum(bool input);
	void setIsAnalog1Trigger(bool input);
	void setIsAnalog1TriggerMaximum(bool input);
	void setVehicleVoltageTriggerLevel(int input);
	void setAnalog0TriggerLevel(int input);
	void setAnalog1TriggerLevel(int input);

	void setIsDigital0TriggerTrans(bool input);
	void setIsDigital1TriggerTrans(bool input);
	void setIsDigital2TriggerTrans(bool input);
	void setIsDigital3TriggerTrans(bool input);

	void setWasBatteryTripped(bool input);
	void setWasDigital0Tripped(bool input);
	void setWasDigital1Tripped(bool input);
	void setWasDigital2Tripped(bool input);
	void setWasDigital3Tripped(bool input);
	void setWasAnalog0Tripped(bool input);
	void setWasAnalog1Tripped(bool input);
	void setWasVehicleVoltageTripped(bool input);

	void setWasDigital0High(bool input);
	void setWasDigital1High(bool input);
	void setWasDigital2High(bool input);
	void setWasDigital3High(bool input);

	void setGeofence(Geofence input);
	void setServerPort(std::string input);
	void setServerAddress(std::string input);
	void setReportTiming(int input);

private:
	void configureOutputs();
	void decodeConfiguration(std::vector<std::string> message);
	void setConfiguration(DeviceSelector selector, DeviceState state, DeviceCutoff cutoff, int value = 0);
	
	Geofence geofence;
	int reportTiming;
	std::string serverPort;
	std::string serverAddress;
	
	bool isBatteryStatusTrigger;
	bool isBatteryTriggerOnHigh;
	bool isDigital0Trigger;
	bool isDigital0TriggerOnHigh;
	bool isDigital1Trigger;
	bool isDigital1TriggerOnHigh;
	bool isDigital2Trigger;
	bool isDigital2TriggerOnHigh;
	bool isDigital3Trigger;
	bool isDigital3TriggerOnHigh;
	bool isDigital0High;
	bool isDigital1High;
	bool isRelayHigh;
	bool isVehicleVoltageTrigger;
	bool isAnalog0Trigger;
	bool isAnalog0TriggerMaximum;
	bool isAnalog1Trigger;
	bool isAnalog1TriggerMaximum;
	int vehicleVoltageTriggerLevel;
	int analog0TriggerLevel;
	int analog1TriggerLevel;

	bool isDigital0TriggerTrans;
	bool isDigital1TriggerTrans;
	bool isDigital2TriggerTrans;
	bool isDigital3TriggerTrans;

	bool wasBatteryTripped;
	bool wasDigital0Tripped;
	bool wasDigital1Tripped;
	bool wasDigital2Tripped;
	bool wasDigital3Tripped;
	bool wasAnalog0Tripped;
	bool wasAnalog1Tripped;
	bool wasVehicleVoltageTripped;

	bool wasDigital0High;
	bool wasDigital1High;
	bool wasDigital2High;
	bool wasDigital3High;
};

#endif // !CONFIGURATION_H_
