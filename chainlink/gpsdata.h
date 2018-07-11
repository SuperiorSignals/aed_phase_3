/*
*	Author: Stewart Nash
*	Date: 3/21/2018
*	File: gpsdata.h
*	Description: Contains GPS data, header
*/
#pragma once
#ifndef GPSDATA_H_
#define GPSDATA_H_
#include <string>
#include <vector>

#define MAX_GPS_STRING_SIZE 128
#define MAX_SATELLITES_IN_VIEW 4
#define MAX_SATELLITES_USED 11
#define ASTERISK_OFFSET 4 // Asterisk location from end of string "*FF\n" vs. length of string

enum GpsCategory {UNDEFINED_GPS, GPGGA, GPGSA, GPGSV, GPRMC, GPVTG, GNGNS, UNKNOWN_GPS = 25};

class Time
{
private:
	int hour;
	int minute;
	int second;
	int milliseconds;
public:
	Time();
	int getHour();
	int getMinute();
	int getSecond();
	int getMilliseconds();
	void setHour(int input);
	void setMinute(int input);
	void setSecond(int input);
	void setMilliseconds(int input);
};

class Date
{
private:
	int year;
	int month;
	int day;
public:
	Date();
	int getYear();
	int getMonth();
	int getDay();
	void setYear(int input);
	void setMonth(int input);
	void setDay(int input);
};

class Position
{
private:
	int degrees;
	float minutes;
	char direction;
public:
	Position();
	int getDegrees();
	float getMinutes();
	char getDirection();
	void setDegrees(int input);
	void setMinutes(float input);
	void setDirection(char input);
};

class Unit
{
private:
	float value;
	char measure;
public:
	Unit();
	float getValue();
	char getMeasure();
	void setValue(float input);
	void setMeasure(char input);
};

class GsvUnit
{
private:
	int satellite;
	int elevation;
	int azimuth;
	int signalNoiseRatio;
public:
	GsvUnit();
	int getSatellite();
	int getElevation();
	int getAzimuth();
	int getSignalNoiseRatio();
	void setSatellite(int input);
	void setElevation(int input);
	void setAzimuth(int input);
	void setSignalNoiseRatio(int input);
};

/*
0. $GPGGA (message ID)
1. hhmmss.sss (UTC time)
2. ddmm.mmmm (latitude)
3. N/S (N/S inidcator)
4. dddmm.mmmm (longitude)
5. E/W (E/W indicator)
6. v (position fix indicator)
7. v (satellites used)
8. v.v (HDOP)
9. v.v (altitude)
10. [units]
11. (geoidal separation)
12. [units]
(13. (age with DGPS))
14. (station)
*/

class GpsGgaData
{
private:
	Time time;
	Position latitude;
	Position longitude;
	int fixQuality;
	int satelliteNumber;
	float horizontalDilution;
	Unit altitude;
	Unit geoidHeight;
	float lastUpdate;
	int station;
	int checksum;
	bool validity;
public:
	GpsGgaData();
	Time getTime();
	Position getLatitude();
	Position getLongitude();
	int getFixQuality();
	int getSatelliteNumber();
	float getHorizontalDilution();
	Unit getAltitude();
	Unit getGeoidHeight();
	float getLastUpdate();
	int getStation();
	int getChecksum();
	bool getValidity();
	void setTime(Time input);
	void setLatitude(Position input);
	void setLongitude(Position input);
	void setFixQuality(int input);
	void setSatelliteNumber(int input);
	void setHorizontalDilution(float input);
	void setAltitude(Unit input);
	void setGeoidHeight(Unit input);
	void setLastUpdate(float input);
	void setStation(int input);
	void setChecksum(int input);
	void setValidity(bool input);
};

/*
GNSS fix data
Field. digits [commentary]
--------------------------
0. $--GNS [--: GP, GL, GN]
1. hhmmss.ss [UTC]
2.
3.
4.
5.
6.
7.
*/

class GpsGnsData
{
private:
	Time time;
	Position latitude;
	Position longitude;
	//char mode[MAX_GPS_STRING_SIZE]; //Variable length field, is MAX_SATELLITES_IN_VIEW sufficient?
	std::vector<char> mode;
	int satelliteNumber;
	float horizontalDilution;
	float altitude;
	float geoidHeight;
	float lastUpdate;
	int station;
	int checksum;
	bool validity;
public:
	GpsGnsData();
	Time getTime();
	Position getLatitude();
	Position getLongitude();
	//char[MAX_GPS_STRING_SIZE] getMode();
	//Variable length field, is MAX_SATELLITES_IN_VIEW sufficient?
	std::vector<char> getMode();
	int getSatelliteNumber();
	float getHorizontalDilution();
	float getAltitude();
	float getGeoidHeight();
	float getLastUpdate();
	int getStation();
	int getChecksum();
	void setTime(Time input);
	void setLatitude(Position input);
	void setLongitude(Position input);
	//void setMode(char input[MAX_GPS_STRING_SIZE]);
	//Variable length field, is MAX_SATELLITES_IN_VIEW sufficient?
	void setMode(std::vector<char> input);
	void setSatelliteNumber(int input);
	void setHorizontalDilution(float input);
	void setAltitude(float input);
	void setGeoidHeight(float input);
	void setLastUpdate(float input);
	void setStation(int input);
	void setChecksum(int input);
};

/*
DOP and active satellites
0. $--GSA [--: GP, GN?]
1. a [mode 1: M - manual, A - automatic]
2. x [mode 2: 1 - not available, 2 - 2D, 3 - 3D]
*/

class GpsGsaData
{
private:
	char operationMode;
	int fixMode;
	//int satellites[MAX_SATELLITES_USED];
	std::vector<int> satellites;
	float pDilution;
	float horizontalDilution;
	float verticalDilution;
	int checksum;
	bool validity;
public:
	GpsGsaData();
	char getOperationMode();
	int getFixMode();
	//int[MAX_SATELLITES_USED] getSatellites();
	std::vector<int> getSatellites();
	float getPDilution();
	float getHorizontalDilution();
	float getVerticalDilution();
	int getChecksum();
	void setOperationMode(char input);
	void setFixMode(int input);
	//void setSatellites(int input[MAX_SATELLITES_USED]);
	void setSatellites(std::vector<int> input);
	void setPDilution(float input);
	void setHorizontalDilution(float input);
	void setVerticalDilution(float input);
	void setChecksum(int input);
};

class GpsGsvData
{
private:
	int numberOfMessages;
	int messageNumber;
	int numberOfSatellites;
	//GsvUnit satellites[MAX_SATELLITES_IN_VIEW];
	std::vector<GsvUnit> satellites;
	int checksum;
	bool validity;
public:
	GpsGsvData();
	int getNumberOfMessages();
	int getMessageNumber();
	int getNumberOfSatellites();
	//GsvUnit[MAX_SATELLITES_IN_VIEW] getSatellites();
	std::vector<GsvUnit> getSatellites();
	int getChecksum();
	void setNumberOfMessages(int input);
	void setMessageNumber(int input);
	void setNumberOfSatellites(int input);
	//void setSatellites(GsvUnit input[MAX_SATELLITES_IN_VIEW]);
	void setSatellites(std::vector<GsvUnit> input);
	void setChecksum(int input);
};

class GpsRmcData
{
private:
	Time time;
	char valid;
	Position latitude;
	Position longitude;
	float speed;
	float trueCourse;
	Date date;
	Unit variation;
	int checksum;
	bool validity;
public:
	GpsRmcData();
	Time getTime();
	char getValid();
	Position getLatitude();
	Position getLongitude();
	float getSpeed();
	float getTrueCourse();
	Date getDate();
	Unit getVariation();
	int getChecksum();
	bool getValidity();
	void setTime(Time input);
	void setValid(char input);
	void setLatitude(Position input);
	void setLongitude(Position input);
	void setSpeed(float input);
	void setTrueCourse(float input);
	void setDate(Date input);
	void setVariation(Unit input);
	void setChecksum(int input);
	void setValidity(bool input);
};

class GpsVtgData
{
private:
	Unit trueTrack;
	Unit magneticTrack;
	Unit knotSpeed;
	Unit kmhSpeed;
	int checksum;
	bool validity;
public:
	GpsVtgData();
	Unit getTrueTrack();
	Unit getMagneticTrack();
	Unit getKnotSpeed();
	Unit getKmhSpeed();
	int getChecksum();
	void setTrueTrack(Unit input);
	void setMagneticTrack(Unit input);
	void setKnotSpeed(Unit input);
	void setKmhSpeed(Unit input);
	void setChecksum(int input);
};

class GpsData
{
private:
	GpsGgaData gpsGgaData;
	GpsGnsData gpsGnsData;
	GpsGsaData gpsGsaData;
	GpsGsvData gpsGsvData;
	GpsRmcData gpsRmcData;
	GpsVtgData gpsVtgData;
	//bool validity;
	//bool gpsGgaDataValid;
	//bool gpsGnsDataValid;
	//bool gpsGsaDataValid;
	//bool gpsRmcDataValid;
	//bool gpsVtgDataValid;
public:
	GpsData();
	GpsGgaData getGpsGga();
	GpsGnsData getGpsGns();
	GpsGsaData getGpsGsa();
	GpsGsvData getGpsGsv();
	GpsRmcData getGpsRmc();
	GpsVtgData getGpsVtg();
	bool getValidity();
	void setGpsGga(GpsGgaData input);
	void setGpsGns(GpsGnsData input);
	void setGpsGsa(GpsGsaData input);
	void setGpsGsv(GpsGsvData input);
	void setGpsRmc(GpsRmcData input);
	void setGpsVtg(GpsVtgData input);
	void packGpsGga(std::string input);
	void packGpsGns(std::string input);
	void packGpsGsa(std::string input);
	void packGpsGsv(std::string input);
	void packGpsRmc(std::string input);
	void packGpsVtg(std::string input);
	//void setValidity(bool input);
	void parseGpsData(std::vector<char> input);
	bool validateChecksum(std::string input);
};
#endif // !GPSDATA_H_

