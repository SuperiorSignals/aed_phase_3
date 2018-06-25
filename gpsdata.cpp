/*
*	Author: Stewart Nash
*	Date: 3/21/2018
*	File: gpsdata.cpp
*	Description: Contains GPS data, source
*/
#include "gpsdata.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include "vectorstring.h"

Time::Time()
{
	hour = 0;
	minute = 0;
	second = 0;
	milliseconds = 0;
}

int Time::getHour() { return hour; }
int Time::getMinute() { return minute; }
int Time::getSecond() { return second; }
int Time::getMilliseconds() { return milliseconds; }
void Time::setHour(int input) { hour = input; }
void Time::setMinute(int input) { minute = input; }
void Time::setSecond(int input) { second = input; }
void Time::setMilliseconds(int input) { milliseconds = input; }

Date::Date()
{
	year = 0;
	month = 0;
	day = 0;
}

int Date::getYear() { return year; }
int Date::getMonth() { return month; }
int Date::getDay() { return day; }
void Date::setYear(int input) { year = input; }
void Date::setMonth(int input) { month = input; }
void Date::setDay(int input) { day = input; }

Position::Position()
{
	degrees = 0;
	minutes = 0;
	direction = '\0';
}

int Position::getDegrees() { return degrees; }
float Position::getMinutes() { return minutes; }
char Position::getDirection() { return direction; }
void Position::setDegrees(int input) { degrees = input; }
void Position::setMinutes(float input) { minutes = input; }
void Position::setDirection(char input) { direction = input; }

Unit::Unit()
{
	value = 0;
	measure = '\0';
}

float Unit::getValue() { return value; }
char Unit::getMeasure() { return measure; }
void Unit::setValue(float input) { value = input; }
void Unit::setMeasure(char input) { measure = input; }

GsvUnit::GsvUnit()
{
	satellite = 0;
	elevation = 0;
	azimuth = 0;
	signalNoiseRatio = 0;
}

int GsvUnit::getSatellite() { return satellite; }
int GsvUnit::getElevation() { return elevation; }
int GsvUnit::getAzimuth() { return azimuth; }
int GsvUnit::getSignalNoiseRatio() { return signalNoiseRatio; }
void GsvUnit::setSatellite(int input) { satellite = input; }
void GsvUnit::setElevation(int input) { elevation = input; }
void GsvUnit::setAzimuth(int input) { azimuth = input; }
void GsvUnit::setSignalNoiseRatio(int input) { signalNoiseRatio = input; }

GpsGgaData::GpsGgaData()
{
	fixQuality = 0;
	satelliteNumber = 0;
	horizontalDilution = 0;
	lastUpdate = 0;
	station = 0;
	checksum = 0;
	validity = false;
}

Time GpsGgaData::getTime() { return time; }
Position GpsGgaData::getLatitude() { return latitude; }
Position GpsGgaData::getLongitude() { return longitude; }
int GpsGgaData::getFixQuality() { return fixQuality; }
int GpsGgaData::getSatelliteNumber() { return satelliteNumber; }
float GpsGgaData::getHorizontalDilution() { return horizontalDilution; }
Unit GpsGgaData::getAltitude() { return altitude; }
Unit GpsGgaData::getGeoidHeight() { return geoidHeight; }
float GpsGgaData::getLastUpdate() { return lastUpdate; }
int GpsGgaData::getStation() { return station; }
int GpsGgaData::getChecksum() { return checksum; }
bool GpsGgaData::getValidity() { return validity; }
void GpsGgaData::setTime(Time input) { time = input; }
void GpsGgaData::setLatitude(Position input) { latitude = input; }
void GpsGgaData::setLongitude(Position input) { longitude = input; }
void GpsGgaData::setFixQuality(int input) { fixQuality = input; }
void GpsGgaData::setSatelliteNumber(int input) { satelliteNumber = input; }
void GpsGgaData::setHorizontalDilution(float input) { horizontalDilution = input; }
void GpsGgaData::setAltitude(Unit input) { altitude = input; }
void GpsGgaData::setGeoidHeight(Unit input) { geoidHeight = input; }
void GpsGgaData::setLastUpdate(float input) { lastUpdate = input; }
void GpsGgaData::setStation(int input) { station = input; }
void GpsGgaData::setChecksum(int input) { checksum = input; }
void GpsGgaData::setValidity(bool input) { validity = input; }

GpsGnsData::GpsGnsData()
{
	satelliteNumber = 0;
	horizontalDilution = 0;
	altitude = 0;
	geoidHeight = 0;
	lastUpdate = 0;
	station = 0;
	checksum = 0;
	validity = false;
}

Time GpsGnsData::getTime() { return time; }
Position GpsGnsData::getLatitude() { return latitude; }
Position GpsGnsData::getLongitude() { return longitude; }
//char[MAX_GPS_STRING_SIZE] GpsGnsData::getMode() { return mode; }
//Variable length field, is MAX_SATELLITES_IN_VIEW sufficient?
std::vector<char> GpsGnsData::getMode() { return mode; }
int GpsGnsData::getSatelliteNumber() { return satelliteNumber; }
float GpsGnsData::getHorizontalDilution() { return horizontalDilution; }
float GpsGnsData::getAltitude() { return altitude; }
float GpsGnsData::getGeoidHeight() { return geoidHeight; }
float GpsGnsData::getLastUpdate() { return lastUpdate; }
int GpsGnsData::getStation() { return station; }
int GpsGnsData::getChecksum() { return checksum; }
void GpsGnsData::setTime(Time input) { time = input; }
void GpsGnsData::setLatitude(Position input) { latitude = input; }
void GpsGnsData::setLongitude(Position input) { longitude = input; }
//void GpsGnsData::setMode(char input[MAX_GPS_STRING_SIZE]) { mode = input; }
//Variable length field, is MAX_SATELLITES_IN_VIEW sufficient?
void GpsGnsData::setMode(std::vector<char> input) { mode = input; }
void GpsGnsData::setSatelliteNumber(int input) { satelliteNumber = input; }
void GpsGnsData::setHorizontalDilution(float input) { horizontalDilution = input; }
void GpsGnsData::setAltitude(float input) { altitude = input; }
void GpsGnsData::setGeoidHeight(float input) { geoidHeight = input; }
void GpsGnsData::setLastUpdate(float input) { lastUpdate = input; }
void GpsGnsData::setStation(int input) { station = input; }
void GpsGnsData::setChecksum(int input) { checksum = input; }

GpsGsaData::GpsGsaData()
{
	operationMode = '\0';
	fixMode = 0;
	pDilution = 0;
	horizontalDilution = 0;
	verticalDilution = 0;
	checksum = 0;
	validity = false;
}

char GpsGsaData::getOperationMode() { return operationMode; }
int GpsGsaData::getFixMode() { return fixMode; }
//int[MAX_SATELLITES_USED] GpsGsaData::getSatellites() { return satellites; }
std::vector<int> GpsGsaData::getSatellites() { return satellites; }
float GpsGsaData::getPDilution() { return pDilution; }
float GpsGsaData::getHorizontalDilution() { return horizontalDilution; }
float GpsGsaData::getVerticalDilution() { return verticalDilution; }
int GpsGsaData::getChecksum() { return checksum; }
void GpsGsaData::setOperationMode(char input) { operationMode = input; }
void GpsGsaData::setFixMode(int input) { fixMode = input; }
//void GpsGsaData::setSatellites(int input[MAX_SATELLITES_USED]) { satellites = input; }
void GpsGsaData::setSatellites(std::vector<int> input) { satellites = input; }
void GpsGsaData::setPDilution(float input) { pDilution = input; }
void GpsGsaData::setHorizontalDilution(float input) { horizontalDilution = input; }
void GpsGsaData::setVerticalDilution(float input) { verticalDilution = input; }
void GpsGsaData::setChecksum(int input) { checksum = input; }

GpsGsvData::GpsGsvData()
{
	numberOfMessages = 0;
	messageNumber = 0;
	numberOfSatellites = 0;
	checksum = 0;
	validity = false;
}

int GpsGsvData::getNumberOfMessages() { return numberOfMessages; }
int GpsGsvData::getMessageNumber() { return messageNumber; }
int GpsGsvData::getNumberOfSatellites() { return numberOfSatellites; }
//GsvUnit[MAX_SATELLITES_IN_VIEW] GpsGsvData::getSatellites() { return satellites; }
std::vector<GsvUnit> GpsGsvData::getSatellites() { return satellites; }
int GpsGsvData::getChecksum() { return checksum; }
void GpsGsvData::setNumberOfMessages(int input) { numberOfMessages = input; }
void GpsGsvData::setMessageNumber(int input) { messageNumber = input; }
void GpsGsvData::setNumberOfSatellites(int input) { numberOfSatellites = input; }
//void GpsGsvData::setSatellites(GsvUnit input[MAX_SATELLITES_IN_VIEW]) { satellites = input; }
void GpsGsvData::setSatellites(std::vector<GsvUnit> input) { satellites = input; }
void GpsGsvData::setChecksum(int input) { checksum = input; }

GpsRmcData::GpsRmcData()
{
	valid = '\0';
	speed = 0;
	trueCourse = 0;
	checksum = 0;
	validity = false;
}

Time GpsRmcData::getTime() { return time; }
char GpsRmcData::getValid() { return valid; }
Position GpsRmcData::getLatitude() { return latitude; }
Position GpsRmcData::getLongitude() { return longitude; }
float GpsRmcData::getSpeed() { return speed; }
float GpsRmcData::getTrueCourse() { return trueCourse; }
Date GpsRmcData::getDate() { return date; }
Unit GpsRmcData::getVariation() { return variation; }
int GpsRmcData::getChecksum() { return checksum; }
bool GpsRmcData::getValidity() { return validity; }
void GpsRmcData::setTime(Time input) { time = input; }
void GpsRmcData::setValid(char input) { valid = input; }
void GpsRmcData::setLatitude(Position input) { latitude = input; }
void GpsRmcData::setLongitude(Position input) { longitude = input; }
void GpsRmcData::setSpeed(float input) { speed = input; }
void GpsRmcData::setTrueCourse(float input) { trueCourse = input; }
void GpsRmcData::setDate(Date input) { date = input; }
void GpsRmcData::setVariation(Unit input) { variation = input; }
void GpsRmcData::setChecksum(int input) { checksum = input; }
void GpsRmcData::setValidity(bool input) { validity = input; }

GpsVtgData::GpsVtgData()
{
	checksum = 0;
	validity = false;
}

Unit GpsVtgData::getTrueTrack() { return trueTrack; }
Unit GpsVtgData::getMagneticTrack() { return magneticTrack; }
Unit GpsVtgData::getKnotSpeed() { return knotSpeed; }
Unit GpsVtgData::getKmhSpeed() { return kmhSpeed; }
int GpsVtgData::getChecksum() { return checksum; }
void GpsVtgData::setTrueTrack(Unit input) { trueTrack = input; }
void GpsVtgData::setMagneticTrack(Unit input) { magneticTrack = input; }
void GpsVtgData::setKnotSpeed(Unit input) { knotSpeed = input; }
void GpsVtgData::setKmhSpeed(Unit input) { kmhSpeed = input; }
void GpsVtgData::setChecksum(int input) { checksum = input; }

GpsData::GpsData()
{
	//validity = false;
	//gpsGgaDataValid = false;
	//gpsGnsDataValid = false;
	//gpsGsaDataValid = false;
	//gpsRmcDataValid = false;
	//gpsVtgDataValid = false;
}

GpsGgaData GpsData::getGpsGga() { return gpsGgaData; }
GpsGnsData GpsData::getGpsGns() { return gpsGnsData; }
GpsGsaData GpsData::getGpsGsa() { return gpsGsaData; }
GpsGsvData GpsData::getGpsGsv() { return gpsGsvData; }
GpsRmcData GpsData::getGpsRmc() { return gpsRmcData; }
GpsVtgData GpsData::getGpsVtg() { return gpsVtgData; }
//bool GpsData::getValidity() { return validity; }
bool GpsData::getValidity() {
	if (gpsGgaData.getValidity() && gpsRmcData.getValidity()) {
		return true;
	} else {
		return false;
	}

	return false; 
}
void GpsData::setGpsGga(GpsGgaData input) { gpsGgaData = input; }
void GpsData::setGpsGns(GpsGnsData input) { gpsGnsData = input; }
void GpsData::setGpsGsa(GpsGsaData input) { gpsGsaData = input; }
void GpsData::setGpsGsv(GpsGsvData input) { gpsGsvData = input; }
void GpsData::setGpsRmc(GpsRmcData input) { gpsRmcData = input; }
void GpsData::setGpsVtg(GpsVtgData input) { gpsVtgData = input; }
//void GpsData::setValidity(bool input) { validity = input; }

void GpsData::packGpsGga(std::string input)
{
	std::vector<std::string> tokens;
	int size, parameterCount;
	std::string temporary;
	Time time;
	Position latitude;
	Position longitude;
	Unit altitude;
	Unit geoidHeight;
	int ti;
	float tf;

	parameterCount = 0;
	if (validateChecksum(input)) {
		tokens = commaStringToVector(input);
		size = tokens.size();
		for (int i = 0; i < size; i++) {
			switch (i) {
			case 0:
				break;
			case 1:
				time = gpsGgaData.getTime();
				if (tokens[i].size() > 9) {
					try {
						temporary = tokens[i].substr(0, 2);
						ti = std::stoi(temporary);
						time.setHour(ti);
						temporary = tokens[i].substr(2, 2);
						ti = std::stoi(temporary);
						time.setMinute(ti);
						temporary = tokens[i].substr(4, 2);
						ti = std::stoi(temporary);
						time.setSecond(ti);
						temporary = tokens[i].substr(7, 3);
						ti = std::stoi(temporary);
						time.setMilliseconds(ti);
						++parameterCount;
					}
					catch (std::exception &e) {

					}
				}
				gpsGgaData.setTime(time);
				break;
			case 2:
				latitude = gpsGgaData.getLatitude();
				if (tokens[i].size() > 8) {
					try {
						temporary = tokens[i].substr(0, 2);
						ti = std::stoi(temporary);
						latitude.setDegrees(ti);
						temporary = tokens[i].substr(2, 7);
						tf = std::stof(temporary);
						latitude.setMinutes(tf);
						++parameterCount;
					}
					catch (std::exception& e) {

					}
				}
				gpsGgaData.setLatitude(latitude);
				break;
			case 3:
				latitude = gpsGgaData.getLatitude();
				if (tokens[i].size() > 0) {
					latitude.setDirection((tokens[i])[0]);
					++parameterCount;
				}
				gpsGgaData.setLatitude(latitude);
				break;
			case 4:
				longitude = gpsGgaData.getLongitude();
				if (tokens[i].size() > 9) {
					try {
						temporary = tokens[i].substr(0, 3);
						ti = std::stoi(temporary);
						longitude.setDegrees(ti);
						temporary = tokens[i].substr(3, 7);
						tf = std::stof(temporary);
						longitude.setMinutes(tf);
						++parameterCount;
					}
					catch (std::exception& e) {

					}
				}
				gpsGgaData.setLongitude(longitude);
				break;
			case 5:
				longitude = gpsGgaData.getLongitude();
				if (tokens[i].size() > 0) {
					longitude.setDirection((tokens[i])[0]);
					++parameterCount;
				}
				gpsGgaData.setLongitude(longitude);
				break;
			case 6:
				if (tokens[i].size() > 0) {
					try {
						ti = std::stoi(tokens[i]);
						gpsGgaData.setFixQuality(ti);
					}
					catch (std::exception& e) {

					}
				}
				break;
			case 7:
				if (tokens[i].size() > 0) {
					try {
						ti = std::stoi(tokens[i]);
						gpsGgaData.setSatelliteNumber(ti);
					}
					catch (std::exception& e) {

					}
				}
				break;
			case 8:
				if (tokens[i].size() > 2) {
					try {
						tf = std::stof(tokens[i]);
						gpsGgaData.setHorizontalDilution(tf);
					}
					catch (std::exception& e) {

					}
				}
				break;
			case 9:
				altitude = gpsGgaData.getAltitude();
				if (tokens[i].size() > 2) {
					try {
						tf = std::stof(tokens[i]);
						altitude.setValue(tf);
					}
					catch (std::exception& e) {

					}
				}
				gpsGgaData.setAltitude(altitude);
				break;
			case 10:
				altitude = gpsGgaData.getAltitude();
				if (tokens[i].size() > 0) {
					altitude.setMeasure((tokens[i])[0]);
				}
				gpsGgaData.setAltitude(altitude);
				break;
			case 11:
				geoidHeight = gpsGgaData.getGeoidHeight();
				if (tokens[i].size() > 2) {
					try {
						tf = std::stof(tokens[i]);
						geoidHeight.setValue(tf);
					}
					catch (std::exception& e) {

					}
				}
				gpsGgaData.setGeoidHeight(geoidHeight);
				break;
			case 12:
				geoidHeight = gpsGgaData.getGeoidHeight();
				if (tokens[i].size() > 0) {
					geoidHeight.setMeasure((tokens[i])[0]);
				}
				gpsGgaData.setGeoidHeight(geoidHeight);
				break;
			case 13:
				if (tokens[i].size() > 2) {
					try {
						tf = std::stof(tokens[i]);
						gpsGgaData.setLastUpdate(tf);
					}
					catch (std::exception& e) {

					}
				}
				break;
			case 14:
				if (tokens[i].size() > 3) {
					try {
						ti = std::stoi(tokens[i]);
						gpsGgaData.setStation(ti);
					}
					catch (std::exception& e) {

					}
				}
				break;
			default:
				break;
			}
		}
	}
	if (parameterCount == 7) {
		gpsGgaData.setValidity(true);
	} else {
		gpsGgaData.setValidity(false);
	}
}

void GpsData::packGpsGns(std::string input)
{
	//if (validateChecksum(input)) {
	//	std::cout << input << std::endl;
	//}
}

void GpsData::packGpsGsa(std::string input)
{
	//if (validateChecksum(input)) {
	//	std::cout << input << std::endl;
	//}
}

void GpsData::packGpsGsv(std::string input)
{
	//if (validateChecksum(input)) {
	//	std::cout << input << std::endl;
	//}
}

void GpsData::packGpsRmc(std::string input)
{
	std::vector<std::string> tokens;
	int size, parameterCount;
	std::string temporary;
	Time time;
	Date date;
	Position latitude;
	Position longitude;
	Unit variation;
	int ti;
	float tf;

	parameterCount = 0;
	if (validateChecksum(input)) {
		tokens = commaStringToVector(input);
		size = tokens.size();
		for (int i = 0; i < size; i++) {
			switch (i) {
			case 0:
				break;
			case 1:
				time = gpsRmcData.getTime();
				if (tokens[i].size() > 9) {
					try {
						temporary = tokens[i].substr(0, 2);
						ti = std::stoi(temporary);
						time.setHour(ti);
						temporary = tokens[i].substr(2, 2);
						ti = std::stoi(temporary);
						time.setMinute(ti);
						temporary = tokens[i].substr(4, 2);
						ti = std::stoi(temporary);
						time.setSecond(ti);
						temporary = tokens[i].substr(7, 3);
						ti = std::stoi(temporary);
						time.setMilliseconds(ti);
						++parameterCount;
					}
					catch (std::exception &e) {

					}
				}
				gpsRmcData.setTime(time);
				break;
			case 2:
				if (tokens[i].size() > 0) {
					gpsRmcData.setValid((tokens[i])[0]);
					++parameterCount;
				}
				break;
			case 3:
				latitude = gpsRmcData.getLatitude();
				if (tokens[i].size() > 8) {
					try {
						temporary = tokens[i].substr(0, 2);
						ti = std::stoi(temporary);
						latitude.setDegrees(ti);
						temporary = tokens[i].substr(2, 7);
						tf = std::stof(temporary);
						latitude.setMinutes(tf);
						++parameterCount;
					}
					catch (std::exception& e) {

					}
				}
				gpsRmcData.setLatitude(latitude);
				break;
			case 4:
				latitude = gpsRmcData.getLatitude();
				if (tokens[i].size() > 0) {
					latitude.setDirection((tokens[i])[0]);
					++parameterCount;
				}
				gpsRmcData.setLatitude(latitude);
				break;
			case 5:
				longitude = gpsRmcData.getLongitude();
				if (tokens[i].size() > 9) {
					try {
						temporary = tokens[i].substr(0, 3);
						ti = std::stoi(temporary);
						longitude.setDegrees(ti);
						temporary = tokens[i].substr(3, 7);
						tf = std::stof(temporary);
						longitude.setMinutes(tf);
						++parameterCount;
					}
					catch (std::exception& e) {

					}
				}
				gpsRmcData.setLongitude(longitude);
				break;
			case 6:
				longitude = gpsRmcData.getLongitude();
				if (tokens[i].size() > 0) {
					longitude.setDirection((tokens[i])[0]);
					++parameterCount;
				}
				gpsRmcData.setLongitude(longitude);
				break;
			case 7:
				if (tokens[i].size() > 3) {
					try {
						tf = std::stof(tokens[i]);
						gpsRmcData.setSpeed(tf);
						++parameterCount;
					}
					catch (std::exception& e) {

					}
				}
				break;
			case 8:
				if (tokens[i].size() > 3) {
					try {
						tf = std::stof(tokens[i]);
						gpsRmcData.setTrueCourse(tf);
						++parameterCount;
					}
					catch (std::exception& e) {

					}
				}
				break;
			case 9:
				date = gpsRmcData.getDate();
				if (tokens[i].size() > 5) {
					try {
						temporary = tokens[i].substr(0, 2);
						ti = std::stoi(temporary);
						date.setDay(ti);
						temporary = tokens[i].substr(2, 2);
						ti = std::stoi(temporary);
						date.setMonth(ti);
						temporary = tokens[i].substr(4, 2);
						ti = std::stoi(temporary);
						date.setYear(ti);
						++parameterCount;
					}
					catch (std::exception &e) {

					}
				}
				gpsRmcData.setDate(date);
				break;
			case 10:
				variation = gpsRmcData.getVariation();
				if (tokens[i].size() > 2) {
					try {
						tf = std::stof(tokens[i]);
						variation.setValue(tf);
					}
					catch (std::exception& e) {

					}
				}
				gpsRmcData.setVariation(variation);
				break;
			case 11:
				variation = gpsRmcData.getVariation();
				if (tokens[i].size() > 0) {
					variation.setMeasure((tokens[i])[0]);
				}
				gpsRmcData.setVariation(variation);
				break;
			default:
				break;
			}
		}
	}
	if (parameterCount == 9) {
		gpsRmcData.setValidity(true);
	} else {
		gpsRmcData.setValidity(false);
	}

}

void GpsData::packGpsVtg(std::string input)
{
	//if (validateChecksum(input)) {
	//	std::cout << input << std::endl;
	//}
}

void GpsData::parseGpsData(std::vector<char> input)
{
	std::vector<std::string> information;
	std::string token;
	int size;
	GpsCategory category;

	information = newlineVectorToString(input);
	category = UNDEFINED_GPS;
	size = information.size();
	for (int i = 0; i < size; i++) {
		token = information[i].substr(0, 6);
		if (!token.compare("$GPGGA")) {
			category = GPGGA;
		} else if (!token.compare("$GNGGA")) {
			category = GPGGA;
		} else if (!token.compare("$GPGSA")) {
			category = GPGSA;
		} else if (!token.compare("$GNGSA")) {
			category = GPGSA;
		} else if (!token.compare("$GPGSV")) {
			category = GPGSV;
		} else if (!token.compare("$GNGSV")) {
			category = GPGSV;
		} else if (!token.compare("$GPRMC")) {
			category = GPRMC;
		} else if (!token.compare("$GNRMC")) {
			category = GPRMC;
		} else if (!token.compare("$GPVTG")) {
			category = GPVTG;
		} else if (!token.compare("$GNGNS")) {
			category = GNGNS;
		} else {
			category = UNDEFINED_GPS;
		}

		switch (category) {
		case GPGGA:
			packGpsGga(information[i]);
			break;
		case GPGSA:
			packGpsGsa(information[i]);
			break;
		case GPGSV:
			packGpsGsv(information[i]);
			break;
		case GPRMC:
			packGpsRmc(information[i]);
			break;
		case GPVTG:
			packGpsVtg(information[i]);
			break;
		case GNGNS:
			packGpsGns(information[i]);
			break;
		default:
			break;
		}
	}
}

bool GpsData::validateChecksum(std::string input)
{
	std::size_t asteriskPosition;
	std::size_t stringLength;
	int astPos, strLen;
	unsigned int checksum;
	char calculation;
	std::string preChecksum;
	std::string postChecksum;
	std::stringstream streaming;

	calculation = 0;
	asteriskPosition = input.find_first_of('*');
	stringLength = input.size();
	astPos = static_cast<int>(asteriskPosition);
	strLen = static_cast<int>(stringLength);
	if (asteriskPosition == std::string::npos || astPos == 0) { // Check that asterisk is present
		return false;
	} else if (strLen != astPos + ASTERISK_OFFSET){ // Check that string is not too long?
		return false;
	} else {
		preChecksum = input.substr(1, asteriskPosition - 1);
		postChecksum = input.substr(asteriskPosition + 1, stringLength - asteriskPosition - 2);
		streaming << std::hex << postChecksum;
		streaming >> checksum;
		for (int i = 0; i < astPos; i++) {
			calculation = calculation ^ preChecksum[i];
		}
		if (static_cast<unsigned int>(calculation) == checksum) {
			return true;
		} else {
			return false;
		}
	}

	return false; // Never reached
}