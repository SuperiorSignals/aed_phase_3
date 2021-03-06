#include "testfunctiontwo.h"
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "adcpin.h"
#include "apipacket.h"
#include "configuration.h"
#include "datapacket.h"
#include "gpiopin.h"
#include "gpsdata.h"
#include "gpsmodule.h"
#include "packetstorage.h"
#include "serialport.h"
#include "testfunction.h"
#include "vectorstring.h"
#include "xbee.h"
#include "xbeecell.h"

extern "C" {
#include "api_mode.h"
#include "gpio_interface.h"
}

#define CONNECTION_CHECK_TIME 150
#define EVENT_PACKET_WAIT_TIME 60
#define MESH_LISTEN_TIME 5.0
#define STANDARD_PACKET_TIME 600
#define STORED_PACKET_TIME 750
#define TEXT_CHECK_TIME 250

#define SERVER_ADDRESS "135.26.235.158"
#define SERVER_PORT "143C"
#define GPSMODULE_PORT "/dev/ttymxc5"
#define GPSMODULE_RATE 9600
#define XBEECELL_PORT "/dev/ttymxc1"
#define XBEECELL_RATE 115200
#define XBEEMESH_PORT "/dev/ttymxc2"
#define XBEEMESH_RATE 115200

#define PACKET_SENT 0x1C
#define PACKET_UNSENT 0x35

extern Configuration configuration;
extern PacketStorage packetStorage;
extern time_t mainStartTime;
extern time_t lastReportTime;
extern time_t lastStoredPopTime;
//extern double EMPIRICAL_CLOCK_ADJUSTMENT;
//extern double clock_adjustment;

/*
double clock_test(int duration)
{
	int i;
	clock_t endTime, startTime;
	time_t stop, start;
	long difference;
	double output;

	std::cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
	i = 0;
	start = time(NULL);
	startTime = clock();
	while (++i < duration) {
		do {
			stop = time(NULL);
		} while (difftime(stop, start) < static_cast<double>(i));
		endTime = clock();
		difference = static_cast<long long>(endTime - startTime);
		std::cout << "iteration: " << i << std::endl;
		std::cout << "clock ticks: " << difference << std::endl;
		std::cout << "clock ticks per sec: " << static_cast<double>(difference) / static_cast<double>(i) << std::endl;
	}
	output = static_cast<double>(difference) / static_cast<double>(i);
	output = static_cast<double>(CLOCKS_PER_SEC) / output;

	return output;
}

void gpio_set()
{
	GpioPin gpsPower;
	GpioPin gpsSysOn;
	GpioPin gpsOn;
	GpioPin cellOn;
	GpioPin meshOn;
	GpioPin meshRst;

	gpsPower.setLinuxKernelNumber(89);
	gpsPower.write(GPIO_PIN_OUTPUT_HIGH);
	gpsSysOn.setLinuxKernelNumber(42);
	gpsSysOn.write(GPIO_PIN_OUTPUT_HIGH);
	gpsOn.setLinuxKernelNumber(39);
	gpsOn.write(GPIO_PIN_OUTPUT_HIGH);
	cellOn.setLinuxKernelNumber(3);
	cellOn.write(GPIO_PIN_OUTPUT_HIGH);
	meshOn.setLinuxKernelNumber(43);
	meshOn.write(GPIO_PIN_OUTPUT_HIGH);
	meshRst.setLinuxKernelNumber(45);
	meshRst.write(GPIO_PIN_OUTPUT_LOW);
}

unsigned char gpio_test()
{
	GpioPin gpioPin1, gpioPin2;
	time_t startTime, endTime;
	time_t middleTime;
	int portNumber1, pinNumber1;
	int portNumber2, pinNumber2;
	GpioPinOutput output;
	bool isChanged = false;

	unsigned char uc;

	portNumber1 = 1;
	pinNumber1 = 3;
	portNumber2 = 1;
	pinNumber2 = 2;
	gpioPin1.setLinuxKernelNumber(EXPANSION_GPIO, portNumber1, pinNumber1);
	gpioPin2.setLinuxKernelNumber(EXPANSION_GPIO, portNumber2, pinNumber2);
	output = GPIO_PIN_OUTPUT_LOW;
	gpioPin1.write(output);
	std::cout << "Writing low to port " << portNumber1 << " pin " << pinNumber1;
	std::cout << "." << std::endl;
	time(&startTime);
	do {
		time(&endTime);
		do {
			if (isChanged) {
				if (gpioPin2.read() == GPIO_PIN_OUTPUT_LOW) {
					std::cout << "Port " << portNumber2 << " pin " << pinNumber2;
					std::cout << " is low." << std::endl;
				} else if (gpioPin2.read() == GPIO_PIN_OUTPUT_HIGH) {
					std::cout << "Port " << portNumber2 << " pin " << pinNumber2;
					std::cout << " is high." << std::endl;
				} else {
					std::cout << "Indetermintate state." << std::endl;
				}
				isChanged = false;
			}
			time(&middleTime);
		} while (difftime(middleTime, endTime) < 6.0);
		if (output == GPIO_PIN_OUTPUT_HIGH) {
			gpioPin1.write(GPIO_PIN_OUTPUT_LOW);
			std::cout << "Writing low to port " << portNumber1 << " pin " << pinNumber1;
			std::cout << "." << std::endl;
			output = GPIO_PIN_OUTPUT_LOW;
		} else if (output == GPIO_PIN_OUTPUT_LOW) {
			gpioPin1.write(GPIO_PIN_OUTPUT_HIGH);
			std::cout << "Writing high to port " << portNumber1 << " pin " << pinNumber1;
			std::cout << "." << std::endl;
			output = GPIO_PIN_OUTPUT_HIGH;
		} else {
			std::cout << "Error: Unknown state." << std::endl;
		}
		isChanged = true;
	} while (difftime(endTime, startTime) < 30.0);
	output = GPIO_PIN_OUTPUT_LOW;
	gpioPin1.write(output);
	std::cout << "Writing low to port " << portNumber1 << " pin " << pinNumber1;
	std::cout << "." << std::endl;

	return uc;
}

void indicators(bool isGpsValid, bool isCellConnected)
{
	GpioPin gpsGreen;
	GpioPin gpsRed;
	GpioPin cellGreen;
	GpioPin cellRed;

	gpsGreen.setLinuxKernelNumber(40);
	gpsRed.setLinuxKernelNumber(47);
	cellGreen.setLinuxKernelNumber(90);
	cellRed.setLinuxKernelNumber(41);

	if (isGpsValid) {
		gpsGreen.write(GPIO_PIN_OUTPUT_HIGH);
		gpsRed.write(GPIO_PIN_OUTPUT_LOW);
	} else {
		gpsGreen.write(GPIO_PIN_OUTPUT_LOW);
		gpsRed.write(GPIO_PIN_OUTPUT_HIGH);
	}

	if (isCellConnected) {
		cellGreen.write(GPIO_PIN_OUTPUT_HIGH);
		cellRed.write(GPIO_PIN_OUTPUT_LOW);
	} else {
		cellGreen.write(GPIO_PIN_OUTPUT_LOW);
		cellRed.write(GPIO_PIN_OUTPUT_HIGH);
	}
}

void report_shared(GpsData gpsData)
{
	std::ofstream outputFile;

#ifdef WINDOWS_IMPLEMENTATION
	outputFile.open("C:\\opt\\shared\\report.txt", std::ios::out | std::ios::trunc);
#else
	outputFile.open("/opt/shared/report.txt", std::ios::out | std::ios::trunc);
#endif
	if (outputFile.is_open()) {
		outputFile << ((gpsData.getGpsRmc()).getDate()).getMonth() << "-";
		outputFile << ((gpsData.getGpsRmc()).getDate()).getDay() << "-";
		outputFile << ((gpsData.getGpsRmc()).getDate()).getYear() << std::endl;
		outputFile << ((gpsData.getGpsRmc()).getTime()).getHour() << ":";
		outputFile << ((gpsData.getGpsRmc()).getTime()).getMinute() << ":";
		outputFile << ((gpsData.getGpsRmc()).getTime()).getSecond() << std::endl;
		outputFile << ((gpsData.getGpsGga()).getLongitude()).getDegrees() << " ";
		outputFile << ((gpsData.getGpsGga()).getLongitude()).getMinutes() << " ";
		outputFile << ((gpsData.getGpsGga()).getLongitude()).getDirection() << std::endl;
		outputFile << ((gpsData.getGpsGga()).getLatitude()).getDegrees() << " ";
		outputFile << ((gpsData.getGpsGga()).getLatitude()).getMinutes() << " ";
		outputFile << ((gpsData.getGpsGga()).getLatitude()).getDirection() << std::endl;
	} else {
		std::cout << "ERROR [void report_shared(GpsData)]: ";
		std::cout << "Could not open /opt/shared/report.txt for writing.";
		std::cout << std::endl;
	}

	outputFile.close();
}

void test_menu(int selection)
{
	switch (selection) {
	case 0:
		break;
	case 1:
		test_function_1();
		break;
	case 2:
		test_function_2();
		break;
	case 3:
		test_function_3();
		break;
	case 4:
		test_function_4();
		break;
	case 5:
		test_function_5();
		break;
	case 6:
		test_function_6();
		break;
	case 7:
		test_function_7();
		break;
	case 8:
		test_function_8();
		break;
	case 9:
		test_function_9();
		break;
	case 10:
		test_function_10();
		break;
	case 11:
		test_function_11();
		break;
	case 12:
		test_function_12();
		break;
	case 13:
		test_function_13();
		break;
	case 14:
		test_function_14();
		break;
	case 15:
		test_function_15();
		break;
	case 16:
		test_function_16();
		break;
	case 17:
		test_function_17();
		break;
	case 18:
		test_function_18();
		break;
	case 19:
		test_function_19();
		break;
	case 20:
		test_function_20();
		break;
	case 21:
		test_function_21();
		break;
	case 22:
		test_function_22();
		break;
	case 23:
		test_function_23();
		break;
	case 24:
		test_function_24();
		break;
	case 25:
		test_function_25();
		break;
	case 26:
		test_function_26();
		break;
	case 27:
		test_function_27();
		break;
	case 28:
		test_function_28();
		break;
	case 29:
		test_function_29();
		break;
	case 30:
		test_function_30a();
		break;
	case 31:
		test_function_31();
		break;
	case 32:
		test_function_32();
		break;
	case 33:
		test_function_33();
		break;
	case 34:
		test_function_34();
		break;
	case 35:
		test_function_35();
		break;
	case 36:
		test_function_36();
		break;
	case 37:
		test_function_37();
		break;
	case 38:
		test_function_38();
		break;
	case 39:
		test_function_39();
		break;
	case 40:
		test_function_40();
		break;
	case 41:
		test_function_41();
		break;
	case 42:
		test_function_42();
		break;
	case 43:
		test_function_43();
		break;
	case 44:
		test_function_44();
		break;
	case 45:
		test_function_45();
		break;
	case 46:
		test_function_46();
		break;
	case 47:
		test_function_47();
		break;
	case 48:
		test_function_48();
		break;
	case 49:
		test_function_49();
		break;
	case 50:
		test_function_50();
		break;
	case 51:
		test_function_51();
		break;
	case 52:
		test_function_52();
		break;
	case 53:
		test_function_53();
		break;
	case 54:
		test_function_54();
		break;
	case 55:
		test_function_55();
		break;
	case 56:
		test_function_56();
		break;
	case 57:
		test_function_57();
		break;
	case 58:
		test_function_58();
		break;
	case 59:
		test_function_59();
		break;
	case 60:
		test_function_60();
		break;
	case 61:
		test_function_61();
		break;
	case 62:
		test_function_62();
		break;
	case 63:
		test_function_63();
		break;
	case 64:
		test_function_64();
		break;
	case 65:
		test_function_65();
		break;
	case 66:
		test_function_66();
		break;
	case 67:
		test_function_67();
		break;
	case 68:
		test_function_68();
		break;
	case 69:
		test_function_69();
		break;
	case 70:
		test_function_70();
		break;
	default:
		break;
	}
}

void test_function_1()
{
	time_t startTime, endTime;
	time_t currentTime;
	const double MAX_TIME = 15;

	while (1) {
		time(&startTime);
		do {
			time(&endTime);
		} while (difftime(endTime, startTime) < MAX_TIME);
		std::cout << "Hello World!";
	}
}

void test_function_2()
{
	GpioPin digitalInputOne;
	//GpioPin digitalInputTwo;
	//GpioPin digitalInputThree;
	//GpioPin digitalInputFour;
	GpioPinOutput result;

	digitalInputOne.setLinuxKernelNumber(IO_EXPANDER, 0, 37);
	//digitalInputTwo.setLinuxKernelNumber(IO_EXPANDER, 0, 6);
	//digitalInputThree.setLinuxKernelNumber(IO_EXPANDER, 0, 4);
	//digitalInputFour.setLinuxKernelNumber(IO_EXPANDER, 0, 5);

	result = digitalInputOne.read();
	//if (result == GPIO_PIN_OUTPUT_HIGH) {
	//	std::cout << "Digital 1 high" << std::endl;
	//} else if (result == GPIO_PIN_OUTPUT_LOW) {
	//	std::cout << "Digital 1 low" << std::endl;
	//} else {
	//	std::cout << "Digital 1 unknown" << std::endl;
	//}
	//result = digitalInputTwo.read();
	//if (result == GPIO_PIN_OUTPUT_HIGH) {
	//	std::cout << "Digital 2 high" << std::endl;
	//} else if (result == GPIO_PIN_OUTPUT_LOW) {
	//	std::cout << "Digital 2 low" << std::endl;
	//} else {
	//	std::cout << "Digital 2 unknown" << std::endl;
	//}
	//result = digitalInputThree.read();
	//if (result == GPIO_PIN_OUTPUT_HIGH) {
	//	std::cout << "Digital 3 high" << std::endl;
	//} else if (result == GPIO_PIN_OUTPUT_LOW) {
	//	std::cout << "Digital 3 low" << std::endl;
	//} else {
	//	std::cout << "Digital 3 unknown" << std::endl;
	//}
	//result = digitalInputFour.read();
	//if (result == GPIO_PIN_OUTPUT_HIGH) {
	//	std::cout << "Digital 4 high" << std::endl;
	//} else if (result == GPIO_PIN_OUTPUT_LOW) {
	//	std::cout << "Digital 4 low" << std::endl;
	//} else {
	//	std::cout << "Digital 4 unknown" << std::endl;
	//}
}

void test_function_3()
{
	GpsModule gpsModule("/dev/ttymxc5", 9600);
	GpsData gpsData;
	std::vector<std::string> output;
	int size;

	gpsData.parseGpsData(gpsModule.getData());
	GpsGgaData gpsGga = gpsData.getGpsGga();
	Time time = gpsGga.getTime();
	Position latitude = gpsGga.getLatitude();
	Position longitude = gpsGga.getLongitude();
	std::cout << "GPS GGA" << std::endl;
	std::cout << "Time: " << time.getHour() << ":" << time.getMinute() << ":" << time.getSecond();
	std::cout << std::endl;
	std::cout << "Latitude: " << latitude.getDegrees() << " and " << latitude.getMinutes() << " " << latitude.getDirection();
	std::cout << std::endl;
	std::cout << "Longitude: " << longitude.getDegrees() << " and " << longitude.getMinutes() << " " << longitude.getDirection();
	std::cout << std::endl;
	GpsRmcData gpsRmc = gpsData.getGpsRmc();
	Date date = gpsRmc.getDate();
	time = gpsRmc.getTime();
	latitude = gpsRmc.getLatitude();
	longitude = gpsRmc.getLongitude();
	std::cout << "GPS RMC" << std::endl;
	std::cout << "Time: " << time.getHour() << ":" << time.getMinute() << ":" << time.getSecond();
	std::cout << std::endl;
	std::cout << "Date: " << date.getMonth() << "/" << date.getDay() << "/" << date.getYear();
	std::cout << std::endl;
	std::cout << "Latitude: " << latitude.getDegrees() << " and " << latitude.getMinutes() << " " << latitude.getDirection();
	std::cout << std::endl;
	std::cout << "Longitude: " << longitude.getDegrees() << " and " << longitude.getMinutes() << " " << longitude.getDirection();
	std::cout << std::endl;
	//system("pause");
}

void test_function_4()
{

}

void test_function_5()
{

}

void test_function_6()
{
	GpioPin gpsPower;
	GpioPin gpsSysOn;
	GpioPin gpsOn;
	GpioPin cellOn;
	GpioPin meshOn;
	GpioPin meshRst;

	gpsPower.setLinuxKernelNumber(89);
	gpsPower.write(GPIO_PIN_OUTPUT_HIGH);
	gpsSysOn.setLinuxKernelNumber(42);
	gpsSysOn.write(GPIO_PIN_OUTPUT_HIGH);
	gpsOn.setLinuxKernelNumber(39);
	gpsOn.write(GPIO_PIN_OUTPUT_HIGH);
	cellOn.setLinuxKernelNumber(3);
	cellOn.write(GPIO_PIN_OUTPUT_HIGH);
	meshOn.setLinuxKernelNumber(43);
	meshOn.write(GPIO_PIN_OUTPUT_HIGH);
	meshRst.setLinuxKernelNumber(45);
	meshRst.write(GPIO_PIN_OUTPUT_LOW);
}

void test_function_7()
{
	//Configuration configuration;
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;

	isCellConnected = false;
	isGpsValid = false;
	gpio_set();
	eventTriggered = false;
	configuration.loadConfiguration();
	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		isGpsValid = gpsData.getValidity();
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			} else {
				isCellConnected = false;
				packetStorage.pushPacket(packet, NORMAL);
				xBeeMesh.sendData(packet);
			}
			time(&lastReportTime);
			report_shared(gpsData);
		}
		textMessage = xBeeCell.getText();
		if (difftime(currentTime, lastReportTime) > EVENT_PACKET_WAIT_TIME) {
			eventTriggered = checkEvents(configuration);
			if (eventTriggered) {
				dataPacket.setGpsData(gpsData);
				packet = dataPacket.getPacket(EVENT);
				if (xBeeCell.getConnection() == 0) {
					isCellConnected = true;
					xBeeCell.dispatchUdpAt(packet);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				} else {
					isCellConnected = false;
					packetStorage.pushPacket(packet, EVENT);
				}
			}
		}
		indicators(isGpsValid, isCellConnected);
		if (textMessage.size() > 0) {
			configuration.parseConfiguration(textMessage);
		}
		networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		printCharacterVector(networkMessage);
	}
}

void test_function_8()
{
	std::vector<char> output;
	SerialPort serialPort("/dev/ttymxc5", 9600);

	serialPort.open();
	while (1) {
		serialPort.timedRead(output, 4.0);
		printCharacterVector(output);
	}
}

void test_function_9()
{
	DataPacket dataPacket;
	GpsModule gpsModule;
	GpsData gpsData;
	XBeeCell xBeeCell;
	std::vector<char> packet;

	gpsData.parseGpsData(gpsModule.getData());
	dataPacket.setGpsData(gpsData);
	packet = dataPacket.getPacket(NORMAL);
	displayHexadecimal(packet);
	//xBeeCell.dispatchUdpPython(packet);
	xBeeCell.dispatchUdpAt(packet);
	//system("pause");
}

void test_function_10()
{
	DataPacket dataPacket;
	GpsModule gpsModule;
	GpsData gpsData;
	XBeeCell xBeeCell;
	//XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	time_t startTime, endTime;
	time_t currentTime;

	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(NORMAL);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet, SERVER_ADDRESS, SERVER_PORT);
				}
			} else {
				packetStorage.pushPacket(packet, NORMAL);
			}
			time(&lastReportTime);
		}
		//networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
	}
}

void test_function_11()
{
	SerialPort serialPort("/dev/ttymxc1", 115200);
	char buffer[SERIAL_PORT_BUFFER_SIZE] = "";
	int stringLength;
	std::vector<char> inputBuffer;

	stringLength = 0;
	serialPort.open();
	//std::cout << "test point 1" << std::endl;
	serialPort.writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
									//std::cout<< "test point 2" << std::endl;
	stringLength = serialPort.timedRead(inputBuffer, 5.0);
	//std::cout << "test point 3" << std::endl;
	if (stringLength > 0) {
		convertVectorToArray(inputBuffer, buffer);
		displayBuffer(buffer, stringLength);
		displayHexadecimal(buffer, stringLength);
	} else {
		std::cout << "Empty buffer.";
	}
	serialPort.writeString("import xbee\r\n");
	serialPort.writeString("myXBee = xbee.XBee()\r\n");
	serialPort.writeString("print('@IMEI:' + myXBee.atcmd('IM') + '$')\r\n");
	serialPort.writeString("print('@CONN:' + str(myXBee.atcmd('AI')) + '$')\r\n");
	serialPort.writeString("print('@PHON:' + str(myXBee.atcmd('P#')) + '$')\r\n");
	serialPort.writeString("print('@ACCS:' + str(myXBee.atcmd('AN')) + '$')\r\n");
	serialPort.writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	stringLength = serialPort.timedRead(inputBuffer, 5.0);
	if (stringLength > 0) {
		convertVectorToArray(inputBuffer, buffer);
		displayBuffer(buffer, stringLength);
		displayHexadecimal(buffer, stringLength);
	} else {
		std::cout << "Empty buffer." << std::endl;
	}
}

void test_function_12()
{
	std::cout << "Enter clock constant: ";
	std::cin >> clock_adjustment;
}

void test_function_13()
{
	//Configuration configuration;
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;

	isCellConnected = false;
	isGpsValid = false;
	gpio_set();
	eventTriggered = false;
	configuration.loadConfiguration();
	//xBeeMesh.pauseMesh();
	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		isGpsValid = gpsData.getValidity();
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL, configuration);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			} else {
				isCellConnected = false;
				packetStorage.pushPacket(packet, NORMAL);
				xBeeMesh.sendData(packet);
				
				//if (xBeeMesh.getIsPaused() == true) {
				//xBeeMesh.unpauseMesh();
				//xBeeMesh.sendData(packet);
				//xBeeMesh.pauseMesh();
				//} else {
				//xBeeMesh.sendData(packet);
				//}
				

			}
			time(&lastReportTime);
			report_shared(gpsData);
		}
		textMessage = xBeeCell.getText();
		if (difftime(currentTime, lastReportTime) > EVENT_PACKET_WAIT_TIME) {
			eventTriggered = checkEvents(configuration);
			if (eventTriggered) {
				dataPacket.setGpsData(gpsData);
				packet = dataPacket.getPacket(EVENT, configuration);
				if (xBeeCell.getConnection() == 0) {
					isCellConnected = true;
					xBeeCell.dispatchUdpAt(packet);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				} else {
					isCellConnected = false;
					packetStorage.pushPacket(packet, EVENT);
				}
			}
		}
		indicators(isGpsValid, isCellConnected);
		if (textMessage.size() > 0) {
			std::cout << "Text Message: " << textMessage;
			std::cout << std::endl;
			configuration.parseConfiguration(textMessage);
		}
		xBeeMesh.unpauseMesh();
		networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		xBeeMesh.pauseMesh();
	}
}

void test_function_14()
{
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	std::vector<char> phoneNumber;
	std::vector<char> textMessage;

	phoneNumber = stringToVector("8327253625");
	textMessage = stringToVector("Hello World!");
	xBeeCell.dispatchSmsPython(textMessage, phoneNumber);
}

void test_function_15()
{

}

void test_function_16()
{
	GpioPin digitalOutputZero;
	GpioPin digitalOutputOne;
	GpioPin relay;
	int device;
	int output;

	digitalOutputZero.setLinuxKernelNumber(8);
	digitalOutputOne.setLinuxKernelNumber(9);
	relay.setLinuxKernelNumber(46);
	device = 0;
	output = 0;
	do {
		std::cout << "Device: ";
		std::cin >> device;
		std::cout << "Output: ";
		std::cin >> output;

		switch (device) {
		case 1:
			if (output == 0) {
				digitalOutputZero.write(GPIO_PIN_OUTPUT_LOW);
			} else if (output == 1) {
				digitalOutputZero.write(GPIO_PIN_OUTPUT_HIGH);
			}
			break;
		case 2:
			if (output == 0) {
				digitalOutputOne.write(GPIO_PIN_OUTPUT_LOW);
			} else if (output == 1) {
				digitalOutputOne.write(GPIO_PIN_OUTPUT_HIGH);
			}
			break;
		case 3:
			if (output == 0) {
				relay.write(GPIO_PIN_OUTPUT_LOW);
			} else if (output == 1) {
				relay.write(GPIO_PIN_OUTPUT_HIGH);
			}
			break;
		default:
			break;
		}

	} while (device != 0);
}

void test_function_17()
{
	XBeeMesh xBeeMesh;
	std::string string_1 = "Hello World! From device 1.";
	std::string string_2 = "Hello World! From device 2.";
	std::string temporary;

	std::cout << "XBeeMesh.getDestinationHigh()" << std::endl;
	temporary = xBeeMesh.getDestinationHigh();
	std::cout << "XBeeMesh.getDestinationLow()" << std::endl;
	xBeeMesh.getDestinationLow();
	std::cout << "XBeeMesh.getNetworkID()" << std::endl;
	xBeeMesh.getNetworkId();
	std::cout << "XBeeMesh.getNodeIdentifier()" << std::endl;
	xBeeMesh.getNodeIdentifier();
	std::cout << "XBeeMesh.getOperatingChannel()" << std::endl;
	xBeeMesh.getOperatingChannel();
}

void test_function_18()
{
	XBeeMesh xBeeMesh;
	std::string string_1 = "Hello World! From device 1.";
	std::string string_2 = "Hello World! From device 2.";
	char pause;
	std::vector<char> temporary;
	int choice;

	do {
		choice = 0;
		std::cout << "0. Do nothing" << std::endl;
		std::cout << "1. Pause mesh" << std::endl;
		std::cout << "2. Unpause mesh" << std::endl;
		std::cout << "3. Receive data" << std::endl;
		std::cout << "4. Send string 1" << std::endl;
		std::cout << "5. Send string 2" << std::endl;
		std::cout << "Choice: ";
		std::cin >> choice;

		switch (choice) {
		case 0:
			break;
		case 1:
			xBeeMesh.pauseMesh();
			break;
		case 2:
			xBeeMesh.unpauseMesh();
			break;
		case 3:
			xBeeMesh.receiveData(5);
			break;
		case 4:
			xBeeMesh.sendData(string_1);
			break;
		case 5:
			xBeeMesh.sendData(string_2);
			break;
		default:
			break;
		}
	} while (choice < 6);

	//xBeeMesh.sendData(string_2);
	//std::cout << "Press any key and enter to pause mesh.";
	//std::cin >> pause;
	//xBeeMesh.pauseMesh();
	//std::cout << "Press any key and enter to unpause mesh.";
	//std::cin >> pause;
	//xBeeMesh.unpauseMesh();
	//temporary = xBeeMesh.receiveData(5);
}

void test_function_19()
{
	SerialPort serialPort("/dev/ttymxc1", 115200);
	XBeeCell xBeeCell;
	char at_command[2] = { 'D','E' };
	char raw_data[256];
	char frame_length[2];
	char packet[512];
	char api_frame[API_AT_TOTAL_LENGTH];
	std::vector<char> inputBuffer;
	std::vector<unsigned char> bufferDuplicate;


	frame_length[0] = 0;
	frame_length[1] = 1 + 1 + 2 + 2;
	raw_data[0] = 0x14;
	raw_data[1] = 0x3C;
	xBeeCell.apiModeOperation();
	generateApiAtPacket(packet, 1, at_command, frame_length, raw_data);
	serialPort.open();
	serialPort.write(convertArrayToVector(packet, static_cast<int>(3 + frame_length[1] + 1)));
	serialPort.timedRead(inputBuffer, 1.0);
	bufferDuplicate = convertVectorToUnsigned(inputBuffer);
	displayHexadecimal(bufferDuplicate);
	xBeeCell.apiModeExit();
}

// Author: Dane Rodriguez
void test_function_20()
{
	//XBee xbee;
	const char AT_SET = 0x08;
	SerialPort serialPort("/dev/ttymxc1", 115200);
	XBeeCell xBeeCell;
	char at_command[2] = { 'D','E' };
	char api_frame[API_AT_TOTAL_LENGTH];
	std::vector<char> inputBuffer;
	std::vector<unsigned char> bufferDuplicate;
	char rawData[512];
	int frameLength = 0;

	//xbee.setOperationMode(API_MODE);
	//generateApiAtCommand(at_command, 0, 0, api_frame);
	xBeeCell.apiModeOperation();
	//std::cout << "Api Mode Entry Complete" << std::endl;
	generateApiAtCommandNoParam(at_command, 1, api_frame);
	serialPort.open();
	//displayHexadecimal(convertArrayToVector(api_frame, API_AT_NO_PARAM_TOTAL_LENGTH));
	serialPort.write(convertArrayToVector(api_frame, API_AT_NO_PARAM_TOTAL_LENGTH));
	serialPort.timedRead(inputBuffer, 5.0);
	bufferDuplicate = convertVectorToUnsigned(inputBuffer);
	displayHexadecimal(bufferDuplicate);

	//at_command[2] = { 'D', 'L' };
	at_command[0] = 'D';
	at_command[1] = 'L';
	//char api_frame = AT_SET; // Error
	//char frame_length = "0X";
	//serialPort.write(rawData, frameLength);


	xBeeCell.apiModeExit();
	//std::cout << "Api Mode Exit Complete" << std::endl;

	system("pause");
}

void test_function_21()
{
	SerialPort serialPort("/dev/ttymxc1", 115200);
	XBeeCell xBeeCell;
	char at_command[2] = { 'D','E' };
	char raw_data[256];
	char frame_length[2];
	char packet[512];
	char api_frame[API_AT_TOTAL_LENGTH];
	std::vector<char> inputBuffer;
	std::vector<unsigned char> bufferDuplicate;
	int choice;
	std::string entry;

	choice = 0;
	while (choice < 2) {
		std::cout << "0. Menu" << std::endl;
		std::cout << "1. Enter Command" << std::endl;
		std::cout << "2. Exit" << std::endl;
		std::cin >> choice;
		if (choice == 1) {
			std::cout << "AT Command: ";
			std::cin >> entry;
			if (entry.size() > 1) {
				at_command[0] = entry[0];
				at_command[1] = entry[1];

				if (entry[0] > 0x40 && entry[0] < 0x5B && entry[1] > 0x40 && entry[1] < 0x5B) {
					frame_length[0] = 0;
					frame_length[1] = 1 + 1 + 2;
					raw_data[0] = 0x00;
					raw_data[1] = 0x00;
					xBeeCell.apiModeOperation();
					generateApiAtPacket(packet, 1, at_command, frame_length, raw_data);
					serialPort.open();
					serialPort.write(convertArrayToVector(packet, static_cast<int>(3 + frame_length[1] + 1)));
					serialPort.timedRead(inputBuffer, 1.0);
					bufferDuplicate = convertVectorToUnsigned(inputBuffer);
					displayHexadecimal(bufferDuplicate);
					xBeeCell.apiModeExit();
				}
			}
		}
	}
}

void test_function_22()
{
	//XBee xbee;
	SerialPort serialPort("/dev/ttymxc1", 115200);
	XBeeCell xBeeCell;
	char at_command[2] = { 'I','M' };
	char api_frame[API_AT_TOTAL_LENGTH];
	std::vector<char> inputBuffer;
	std::vector<unsigned char> bufferDuplicate;

	//xbee.setOperationMode(API_MODE);
	//generateApiAtCommand(at_command, 0, 0, api_frame);
	xBeeCell.apiModeOperation();
	//std::cout << "Api Mode Entry Complete" << std::endl;
	generateApiAtCommandNoParam(at_command, 1, api_frame);
	serialPort.open();
	//displayHexadecimal(convertArrayToVector(api_frame, API_AT_NO_PARAM_TOTAL_LENGTH));
	serialPort.write(convertArrayToVector(api_frame, API_AT_NO_PARAM_TOTAL_LENGTH));
	serialPort.timedRead(inputBuffer, 5.0);
	bufferDuplicate = convertVectorToUnsigned(inputBuffer);
	displayHexadecimal(bufferDuplicate);
	xBeeCell.apiModeExit();
	//std::cout << "Api Mode Exit Complete" << std::endl;

	//system("pause");
}

void test_function_23()
{
	XBee xbee;

	std::cout << "Exiting . . ." << std::endl;
}

void test_function_24()
{
	int i;
	clock_t endTime, startTime;
	time_t stop, start;
	long long difference;

	std::cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << std::endl;
	i = 0;
	start = time(NULL);
	startTime = clock();
	while (++i < 60) {
		do {
			stop = time(NULL);
		} while (difftime(stop, start) < static_cast<double>(i));
		endTime = clock();
		difference = static_cast<long long>(endTime - startTime);
		std::cout << "iteration: " << i << std::endl;
		std::cout << "clock ticks: " << difference << std::endl;
		std::cout << "clock ticks per sec: " << static_cast<double>(difference) / static_cast<double>(i) << std::endl;
	}
}

void test_function_25()
{
	GpsModule gpsModule;
	GpsData gpsData;
	std::vector<std::string> output;
	int size;


	//output = newlineVectorToString(gpsModule.getData());
	//size = output.size();
	//for (int i = 0; i < size; i++) {
	//std::cout << output[i] << std::endl;
	//}

	gpsData.parseGpsData(gpsModule.getData());
	GpsGgaData gpsGga = gpsData.getGpsGga();
	Time time = gpsGga.getTime();
	Position latitude = gpsGga.getLatitude();
	Position longitude = gpsGga.getLongitude();
	std::cout << "GPS GGA" << std::endl;
	std::cout << "Time: " << time.getHour() << ":" << time.getMinute() << ":" << time.getSecond();
	std::cout << std::endl;
	std::cout << "Latitude: " << latitude.getDegrees() << " and " << latitude.getMinutes() << " " << latitude.getDirection();
	std::cout << std::endl;
	std::cout << "Longitude: " << longitude.getDegrees() << " and " << longitude.getMinutes() << " " << longitude.getDirection();
	std::cout << std::endl;
	GpsRmcData gpsRmc = gpsData.getGpsRmc();
	Date date = gpsRmc.getDate();
	time = gpsRmc.getTime();
	latitude = gpsRmc.getLatitude();
	longitude = gpsRmc.getLongitude();
	std::cout << "GPS RMC" << std::endl;
	std::cout << "Time: " << time.getHour() << ":" << time.getMinute() << ":" << time.getSecond();
	std::cout << std::endl;
	std::cout << "Date: " << date.getMonth() << "/" << date.getDay() << "/" << date.getYear();
	std::cout << std::endl;
	std::cout << "Latitude: " << latitude.getDegrees() << " and " << latitude.getMinutes() << " " << latitude.getDirection();
	std::cout << std::endl;
	std::cout << "Longitude: " << longitude.getDegrees() << " and " << longitude.getMinutes() << " " << longitude.getDirection();
	std::cout << std::endl;
	system("pause");
}

void test_function_26()
{
	DataPacket dataPacket;
	GpsModule gpsModule;
	GpsData gpsData;
	XBeeCell xBeeCell;
	std::vector<char> packet;

	gpsData.parseGpsData(gpsModule.getData());
	dataPacket.setGpsData(gpsData);
	packet = dataPacket.getPacket(NORMAL);
	displayHexadecimal(packet);
	//xBeeCell.dispatchUdpPython(packet);
	xBeeCell.dispatchUdpAt(packet);
	//system("pause");
}

void test_function_27()
{
	SerialPort serialPort("/dev/ttymxc1", 115200);
	char buffer[SERIAL_PORT_BUFFER_SIZE] = "";
	int stringLength;
	std::vector<char> inputBuffer;

	stringLength = 0;
	serialPort.open();
	//std::cout << "test point 1" << std::endl;
	serialPort.writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
									//std::cout<< "test point 2" << std::endl;
	stringLength = serialPort.timedRead(inputBuffer, 5.0);
	//std::cout << "test point 3" << std::endl;
	if (stringLength > 0) {
		convertVectorToArray(inputBuffer, buffer);
		displayBuffer(buffer, stringLength);
		displayHexadecimal(buffer, stringLength);
	}
	else {
		std::cout << "Empty buffer.";
	}
	serialPort.writeString("import usocket\r\n");
	serialPort.writeString("import network\r\n");
	serialPort.writeString("import time\r\n");
	serialPort.writeString("TCP_IP = 'www.micropython.org'\r\n");
	serialPort.writeString("TCP_PORT = 80\r\n");
	serialPort.writeString("address = (TCP_IP, TCP_PORT)\r\n");
	serialPort.writeString("request = 'GET /ks/test.html HTTP/1.1\\r\\nHost: www.micropython.org\\r\\n\\r\\n'\r\n");
	serialPort.writeString("cell = network.Cellular()\r\n");
	serialPort.writeString("while not cell.isconnected():\r\n");
	serialPort.writeString("\ttime.sleep_ms(1500)\r\n");
	serialPort.writeString("mySocket = usocket.socket(usocket.AF_INET, usocket.SOCK_STREAM)\r\n");
	serialPort.writeString("mySocket.connect(address)\r\n");
	serialPort.writeString("print('connected to ' + address[0] + '\\r\\n')\r\n");

	//serialPort.writeString("data = b'");
	//for (i = 0; i < DATA_PACKET_SIZE; i++) {
	//serialPutChar('\\');
	//integerToString(temporary, (int) input[i]);
	//serialPort.writeString(temporary);
	//}
	//serialPort.writeString("'\r\n");
	//serialPort.writeString("for i in data:\r\n");
	//serialPort.writeString("\tprint(i)\r\n");

	serialPort.writeString("numberOfBytesSent = mySocket.send(request)\r\n");
	serialPort.writeString("print('Bytes sent %d\\r\\n' % numberOfBytesSent)\r\n");
	serialPort.writeString("reply = mySocket.recv(512)\r\n");
	serialPort.writeString("print(reply)\r\n");
	serialPort.writeString("mySocket.close()\r\n");
	serialPort.writeString("print('socket closed\\r\\n')\r\n");
	serialPort.writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	stringLength = serialPort.timedRead(inputBuffer, 5.0);
	if (stringLength > 0) {
		convertVectorToArray(inputBuffer, buffer);
		displayBuffer(buffer, stringLength);
		displayHexadecimal(buffer, stringLength);
	}
	else {
		std::cout << "Empty buffer." << std::endl;
	}
}

void test_function_28()
{
	GpioPin gpioPin;
	time_t startTime, endTime;
	int portNumber, pinNumber;

	portNumber = 1;
	pinNumber = 3;
	gpioPin.setLinuxKernelNumber(EXPANSION_GPIO, portNumber, pinNumber);
	gpioPin.write(GPIO_PIN_OUTPUT_HIGH);
	std::cout << "Writing high to pin " << pinNumber << ".";
	std::cout << std::endl;
	time(&startTime);
	do {
		time(&endTime);
	} while (difftime(endTime, startTime) < 6.0);
	gpioPin.write(GPIO_PIN_OUTPUT_LOW);
	std::cout << "Writing low to pin " << pinNumber << ".";
	std::cout << std::endl;
}

void test_function_29()
{
	GpioPin gpioPin1, gpioPin2;
	time_t startTime, endTime;
	time_t middleTime;
	int portNumber1, pinNumber1;
	int portNumber2, pinNumber2;
	GpioPinOutput output;
	bool isChanged = false;

	portNumber1 = 1;
	pinNumber1 = 3;
	portNumber2 = 1;
	pinNumber2 = 2;
	gpioPin1.setLinuxKernelNumber(EXPANSION_GPIO, portNumber1, pinNumber1);
	gpioPin2.setLinuxKernelNumber(EXPANSION_GPIO, portNumber2, pinNumber2);
	output = GPIO_PIN_OUTPUT_LOW;
	gpioPin1.write(output);
	std::cout << "Writing low to port " << portNumber1 << " pin " << pinNumber1;
	std::cout << "." << std::endl;
	time(&startTime);
	do {
		time(&endTime);
		do {
			if (isChanged) {
				if (gpioPin2.read() == GPIO_PIN_OUTPUT_LOW) {
					std::cout << "Port " << portNumber2 << " pin " << pinNumber2;
					std::cout << " is low." << std::endl;
				}
				else if (gpioPin2.read() == GPIO_PIN_OUTPUT_HIGH) {
					std::cout << "Port " << portNumber2 << " pin " << pinNumber2;
					std::cout << " is high." << std::endl;
				}
				else {
					std::cout << "Indetermintate state." << std::endl;
				}
				isChanged = false;
			}
			time(&middleTime);
		} while (difftime(middleTime, endTime) < 6.0);
		if (output == GPIO_PIN_OUTPUT_HIGH) {
			gpioPin1.write(GPIO_PIN_OUTPUT_LOW);
			std::cout << "Writing low to port " << portNumber1 << " pin " << pinNumber1;
			std::cout << "." << std::endl;
			output = GPIO_PIN_OUTPUT_LOW;
		}
		else if (output == GPIO_PIN_OUTPUT_LOW) {
			gpioPin1.write(GPIO_PIN_OUTPUT_HIGH);
			std::cout << "Writing high to port " << portNumber1 << " pin " << pinNumber1;
			std::cout << "." << std::endl;
			output = GPIO_PIN_OUTPUT_HIGH;
		}
		else {
			std::cout << "Error: Unknown state." << std::endl;
		}
		isChanged = true;
	} while (difftime(endTime, startTime) < 30.0);
	output = GPIO_PIN_OUTPUT_LOW;
	gpioPin1.write(output);
	std::cout << "Writing low to port " << portNumber1 << " pin " << pinNumber1;
	std::cout << "." << std::endl;
}

void test_function_30b()
{
	GpsModule gpsModule("/dev/ttymxc1", 115200);
	SerialPort serialPort("/dev/ttymxc2", 115200);
	std::vector<char> message;
	time_t startTime, endTime;
	GpioPin gpioPin1, gpioPin2, gpioPin3, gpioPin4;
	unsigned char bitPattern;

	bitPattern = 0;
	gpioPin1.setLinuxKernelNumber(IO_EXPANDER, 0, 3);
	gpioPin2.setLinuxKernelNumber(IO_EXPANDER, 0, 6);
	gpioPin3.setLinuxKernelNumber(IO_EXPANDER, 0, 4);
	gpioPin4.setLinuxKernelNumber(IO_EXPANDER, 0, 5);
	serialPort.open();
	while (1) {
		time(&startTime);
		if (bitPattern & 1) {
			gpioPin1.write(GPIO_PIN_OUTPUT_HIGH);
		}
		else {
			gpioPin1.write(GPIO_PIN_OUTPUT_LOW);
		}
		if (bitPattern & 2) {
			gpioPin2.write(GPIO_PIN_OUTPUT_HIGH);
		}
		else {
			gpioPin2.write(GPIO_PIN_OUTPUT_LOW);
		}
		if (bitPattern & 4) {
			gpioPin3.write(GPIO_PIN_OUTPUT_HIGH);
		}
		else {
			gpioPin3.write(GPIO_PIN_OUTPUT_LOW);
		}
		if (bitPattern & 8) {
			gpioPin4.write(GPIO_PIN_OUTPUT_HIGH);
		}
		else {
			gpioPin4.write(GPIO_PIN_OUTPUT_LOW);
		}
		do {
			message = gpsModule.getData(2.0);
			serialPort.write(message);
			printCharacterVector(message);
			time(&endTime);
		} while (difftime(endTime, startTime < 360.0));
		bitPattern++;
	}
}

void test_function_30a()
{
	DataPacket dataPacket;
	GpsModule gpsModule;
	GpsData gpsData;
	XBeeCell xBeeCell;
	std::vector<char> packet;
	time_t startTime, endTime;
	//time_t middleTime;

	while (1) {
		time(&startTime);
		do {
			gpsData.parseGpsData(gpsModule.getData());
			time(&endTime);
		} while (difftime(endTime, startTime) < 300.0);
		dataPacket.setGpsData(gpsData);
		packet = dataPacket.getPacket(NORMAL);
		displayHexadecimal(packet);
		//xBeeCell.dispatchUdpPython(packet);
		xBeeCell.dispatchUdpAt(packet);
		std::cout << "Package dispatched." << std::endl;
	}
}

void test_function_31()
{
	std::vector<char> output;
	SerialPort serialPort("/dev/ttymxc1", 115200);

	serialPort.open();
	while (1) {
		serialPort.timedRead(output, 2.0);
		printCharacterVector(output);
	}
}

void test_function_32()
{
	std::vector<char> output;
	SerialPort serialPort("/dev/ttymxc2", 115200);

	serialPort.open();
	while (1) {
		serialPort.timedRead(output, 4.0);
		printCharacterVector(output);
	}
}

void test_function_33()
{
	GpioPin digitalInputOne;
	GpioPin digitalInputTwo;
	GpioPin digitalInputThree;
	GpioPin digitalInputFour;
	GpioPinOutput result;

	digitalInputOne.setLinuxKernelNumber(IO_EXPANDER, 0, 3);
	digitalInputTwo.setLinuxKernelNumber(IO_EXPANDER, 0, 6);
	digitalInputThree.setLinuxKernelNumber(IO_EXPANDER, 0, 4);
	digitalInputFour.setLinuxKernelNumber(IO_EXPANDER, 0, 5);

	result = digitalInputOne.read();
	if (result == GPIO_PIN_OUTPUT_HIGH) {
		std::cout << "Digital 1 high" << std::endl;
	}
	else if (result == GPIO_PIN_OUTPUT_LOW) {
		std::cout << "Digital 1 low" << std::endl;
	}
	else {
		std::cout << "Digital 1 unknown" << std::endl;
	}
	result = digitalInputTwo.read();
	if (result == GPIO_PIN_OUTPUT_HIGH) {
		std::cout << "Digital 2 high" << std::endl;
	}
	else if (result == GPIO_PIN_OUTPUT_LOW) {
		std::cout << "Digital 2 low" << std::endl;
	}
	else {
		std::cout << "Digital 2 unknown" << std::endl;
	}
	result = digitalInputThree.read();
	if (result == GPIO_PIN_OUTPUT_HIGH) {
		std::cout << "Digital 3 high" << std::endl;
	}
	else if (result == GPIO_PIN_OUTPUT_LOW) {
		std::cout << "Digital 3 low" << std::endl;
	}
	else {
		std::cout << "Digital 3 unknown" << std::endl;
	}
	result = digitalInputFour.read();
	if (result == GPIO_PIN_OUTPUT_HIGH) {
		std::cout << "Digital 4 high" << std::endl;
	}
	else if (result == GPIO_PIN_OUTPUT_LOW) {
		std::cout << "Digital 4 low" << std::endl;
	}
	else {
		std::cout << "Digital 4 unknown" << std::endl;
	}
}

void test_function_34()
{
	XBeeMesh xBeeMesh;
	std::string output = "Hello World!";

	std::cout << "Operating Channel" << std::endl;
	xBeeMesh.getOperatingChannel();
	std::cout << "Network ID" << std::endl;
	xBeeMesh.getNetworkId();
	std::cout << "Serial Number High" << std::endl;
	xBeeMesh.getSerialNumberHigh();
	std::cout << "Serial Number Low" << std::endl;
	xBeeMesh.getSerialNumberLow();
	std::cout << "Destination High" << std::endl;
	xBeeMesh.getDestinationHigh();
	std::cout << "Destination Low" << std::endl;
	xBeeMesh.getDestinationLow();
	xBeeMesh.sendData(output);
}

void test_function_35()
{
	XBeeMesh xBeeMesh;
	std::vector<char> output;

	while (1) {
		output = xBeeMesh.receiveData(5.0);
		if (output.size() > 0) {
			displayHexadecimal(output);
		}
	}
}

void test_function_36()
{
	PacketStorage packetStorage;
	std::string output_1 = "abcdefghijklm";
	std::string output_2 = "1234567890123";
	std::string output_3 = "nopqrstuvwxyz";
	std::string output_4 = "0987654321098";
	std::vector<char> output;

	packetStorage.pushPacket(stringToVector(output_1), NORMAL);
	packetStorage.pushPacket(stringToVector(output_2), NORMAL);
	packetStorage.pushPacket(stringToVector(output_3), NORMAL);
	packetStorage.pushPacket(stringToVector(output_4), NORMAL);

	output = packetStorage.popPacket(NORMAL);
	std::cout << vectorToString(output) << std::endl;
	output = packetStorage.popPacket(NORMAL);
	std::cout << vectorToString(output) << std::endl;
	output = packetStorage.popPacket(NORMAL);
	std::cout << vectorToString(output) << std::endl;
	output = packetStorage.popPacket(NORMAL);
	std::cout << vectorToString(output) << std::endl;
}

void test_function_37()
{
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	//XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	time_t startTime, endTime;
	time_t currentTime;

	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(NORMAL);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet, SERVER_ADDRESS, SERVER_PORT);
				}
			} else {
				packetStorage.pushPacket(packet, NORMAL);
			}
			time(&lastReportTime);
		}
		//networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
	}
}

void test_function_38()
{
	PacketStorage packetStorage;
	std::string output_1 = "abcdefghijklm";
	std::string output_2 = "1234567890123";
	std::string output_3 = "nopqrstuvwxyz";
	std::string output_4 = "0987654321098";
	std::string output_5 = "nopqrstuvwxyz";
	std::vector<char> output;

	packetStorage.pushPacket(stringToVector(output_1), NORMAL);
	packetStorage.pushPacket(stringToVector(output_2), NORMAL);
	packetStorage.pushPacket(stringToVector(output_3), NORMAL);
	packetStorage.pushPacket(stringToVector(output_4), NORMAL);
	packetStorage.pushPacket(stringToVector(output_5), NORMAL);

	//output = packetStorage.popPacket(NORMAL);
	//std::cout << vectorToString(output) << std::endl;
	//output = packetStorage.popPacket(NORMAL);
	//std::cout << vectorToString(output) << std::endl;
	//output = packetStorage.popPacket(NORMAL);
	//std::cout << vectorToString(output) << std::endl;
	//output = packetStorage.popPacket(NORMAL);
	//std::cout << vectorToString(output) << std::endl;
}

void test_function_39()
{
	AdcPin adcPin;
	int value;

	adcPin.setAdcChipNumber(0);
	adcPin.setAdcChannelNumber(2);
	value = adcPin.read();

	std::cout << "Read value is: " << value;
	std::cout << std::endl;
}

void test_function_40()
{
	//Configuration configuration;
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	//XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	bool eventTriggered;

	eventTriggered = false;
	configuration.loadConfiguration();
	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			} else {
				packetStorage.pushPacket(packet, NORMAL);
			}
			time(&lastReportTime);
		}
		textMessage = xBeeCell.getText();
		if (difftime(currentTime, lastReportTime) > EVENT_PACKET_WAIT_TIME) {
			eventTriggered = checkEvents(configuration);
			if (eventTriggered) {
				dataPacket.setGpsData(gpsData);
				packet = dataPacket.getPacket(EVENT);
				if (xBeeCell.getConnection() == 0) {
					xBeeCell.dispatchUdpAt(packet);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				} else {
					packetStorage.pushPacket(packet, EVENT);
				}
			}
		}
		if (textMessage.size() > 0) {
			configuration.parseConfiguration(textMessage);
		}
		//networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
	}
}
*/

void test_function_41()
{
	SerialPort serialPort("/dev/ttymxc1", 115200);
	char buffer[SERIAL_PORT_BUFFER_SIZE] = "";
	int stringLength;
	std::vector<char> inputBuffer;

	stringLength = 0;
	serialPort.open();
	//std::cout << "test point 1" << std::endl;
	serialPort.writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
									//std::cout<< "test point 2" << std::endl;
	stringLength = serialPort.timedRead(inputBuffer, 5.0);
	//std::cout << "test point 3" << std::endl;
	if (stringLength > 0) {
		convertVectorToArray(inputBuffer, buffer);
		displayBuffer(buffer, stringLength);
		displayHexadecimal(buffer, stringLength);
	}
	else {
		std::cout << "Empty buffer.";
	}
	serialPort.writeString("import xbee\r\n");
	serialPort.writeString("myXBee = xbee.XBee()\r\n");
	serialPort.writeString("print('@IMEI:' + myXBee.atcmd('IM') + '$')\r\n");
	serialPort.writeString("print('@CONN:' + str(myXBee.atcmd('AI')) + '$')\r\n");
	serialPort.writeString("print('@PHON:' + str(myXBee.atcmd('P#')) + '$')\r\n");
	serialPort.writeString("print('@ACCS:' + str(myXBee.atcmd('AN')) + '$')\r\n");
	serialPort.writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	stringLength = serialPort.timedRead(inputBuffer, 5.0);
	if (stringLength > 0) {
		convertVectorToArray(inputBuffer, buffer);
		displayBuffer(buffer, stringLength);
		displayHexadecimal(buffer, stringLength);
	} else {
		std::cout << "Empty buffer." << std::endl;
	}
}


void test_function_42()
{
	//Configuration configuration;
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	//XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;

	isCellConnected = false;
	isGpsValid = false;
	gpio_set();
	eventTriggered = false;
	configuration.loadConfiguration();
	//xBeeMesh.pauseMesh();
	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		isGpsValid = gpsData.getValidity();
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL, configuration);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			} else {
				isCellConnected = false;
				packetStorage.pushPacket(packet, NORMAL);
				//xBeeMesh.sendData(packet);
				/*
				if (xBeeMesh.getIsPaused() == true) {
				xBeeMesh.unpauseMesh();
				xBeeMesh.sendData(packet);
				xBeeMesh.pauseMesh();
				} else {
				xBeeMesh.sendData(packet);
				}
				*/
				test_function_43(packet);
			}
			time(&lastReportTime);
			report_shared(gpsData);
		}
		textMessage = xBeeCell.getText();
		if (difftime(currentTime, lastReportTime) > EVENT_PACKET_WAIT_TIME) {
			eventTriggered = checkEvents(configuration);
			if (eventTriggered) {
				dataPacket.setGpsData(gpsData);
				packet = dataPacket.getPacket(EVENT, configuration);
				if (xBeeCell.getConnection() == 0) {
					isCellConnected = true;
					xBeeCell.dispatchUdpAt(packet);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				} else {
					isCellConnected = false;
					packetStorage.pushPacket(packet, EVENT);
				}
			}
		}
		indicators(isGpsValid, isCellConnected);
		if (textMessage.size() > 0) {
			std::cout << "Text Message: " << textMessage;
			std::cout << std::endl;
			configuration.parseConfiguration(textMessage);
		}
		//xBeeMesh.unpauseMesh();
		//networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		//xBeeMesh.pauseMesh();
	}
}



void test_function_43(std::vector<char> input)
{
	XBeeMesh xBeeMesh;
	SerialPort serialPort("/dev/ttymxc2", 9600);
	MeshTxPacket meshTxPacket;


	std::vector<char> tx_packet;
	std::vector<char> inputBuffer;


	meshTxPacket.setData(input);
	meshTxPacket.setFrameId(0x01);
	tx_packet = meshTxPacket.getPacket();
	std::cout << "Tx Packet will be sent: ";
	displayHexadecimal(tx_packet);

	xBeeMesh.apiModeEntry();
	serialPort.openRts();
	serialPort.write(tx_packet);
	inputBuffer.clear();
	serialPort.timedRead(inputBuffer, 2.0);
	displayHexadecimal(inputBuffer);
	xBeeMesh.apiModeExit();
}

void test_function_44()
{
	//Configuration configuration;
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;
	SerialPort serialPort("/dev/ttymxc2", 9600);
	MeshTxPacket meshTxPacket;
	std::vector<char> inputBuffer;

	isCellConnected = false;
	isGpsValid = false;
	gpio_set();
	eventTriggered = false;
	configuration.loadConfiguration();
	//xBeeMesh.pauseMesh();
	xBeeMesh.unpauseMesh();
	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		isGpsValid = gpsData.getValidity();
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL, configuration);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			} else {
				isCellConnected = false;
				packetStorage.pushPacket(packet, NORMAL);
				{
					std::cout << "Transmitting through mesh." << std::endl;
					xBeeMesh.apiModeEntry();
					serialPort.openRts();
					serialPort.write(packet);
					inputBuffer.clear();
					serialPort.timedRead(inputBuffer, 2.0);
					displayHexadecimal(inputBuffer);
					xBeeMesh.apiModeExit();
				}
			}
			time(&lastReportTime);
			report_shared(gpsData);
		}
		textMessage = xBeeCell.getText();
		if (difftime(currentTime, lastReportTime) > EVENT_PACKET_WAIT_TIME) {
			eventTriggered = checkEvents(configuration);
			if (eventTriggered) {
				dataPacket.setGpsData(gpsData);
				packet = dataPacket.getPacket(EVENT, configuration);
				if (xBeeCell.getConnection() == 0) {
					isCellConnected = true;
					xBeeCell.dispatchUdpAt(packet);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				} else {
					isCellConnected = false;
					packetStorage.pushPacket(packet, EVENT);
				}
			}
		}
		indicators(isGpsValid, isCellConnected);
		if (textMessage.size() > 0) {
			std::cout << "Text Message: " << textMessage;
			std::cout << std::endl;
			configuration.parseConfiguration(textMessage);
		}
		//xBeeMesh.unpauseMesh();
		networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		//xBeeMesh.pauseMesh();
	}
}

void test_function_45()
{
	//Configuration configuration;
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell(XBEECELL_PORT, XBEECELL_RATE);
	XBeeMesh xBeeMesh;
	MeshTxPacket meshTxPacket;
	SerialPort serialPort("/dev/ttymxc1", 115200);
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::vector<char> inputBuffer;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;

	isCellConnected = false;
	isGpsValid = false;
	gpio_set();
	eventTriggered = false;
	configuration.loadConfiguration();
	//xBeeMesh.pauseMesh();
	time(&lastReportTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		isGpsValid = gpsData.getValidity();
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL, configuration);
			//displayHexadecimal(packet);
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
				xBeeCell.dispatchUdpAt(packet);
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			} else {
				isCellConnected = false;
				packetStorage.pushPacket(packet, NORMAL);
				if (xBeeMesh.getIsPaused() == true) {
					//xBeeMesh.unpauseMesh();
					{
						meshTxPacket.setData(packet);
						meshTxPacket.setFrameId(0x01);
						packet = meshTxPacket.getPacket();
						std::cout << "Mesh tx packet will be sent: ";
						displayHexadecimal(packet);
						xBeeMesh.apiModeEntry();
						serialPort.openRts();
						serialPort.write(packet);
						inputBuffer.clear();
						serialPort.timedRead(inputBuffer, 2.0);
						displayHexadecimal(inputBuffer);
						xBeeMesh.apiModeExit();
					}
					//xBeeMesh.pauseMesh();
				} else {
					{
						meshTxPacket.setData(packet);
						meshTxPacket.setFrameId(0x01);
						packet = meshTxPacket.getPacket();
						std::cout << "Mesh tx packet will be sent: ";
						displayHexadecimal(packet);
						xBeeMesh.apiModeEntry();
						serialPort.openRts();
						serialPort.write(packet);
						inputBuffer.clear();
						serialPort.timedRead(inputBuffer, 2.0);
						displayHexadecimal(inputBuffer);
						xBeeMesh.apiModeExit();
					}
				}
			}
			time(&lastReportTime);
			report_shared(gpsData);
		}
		textMessage = xBeeCell.getText();
		if (difftime(currentTime, lastReportTime) > EVENT_PACKET_WAIT_TIME) {
			eventTriggered = checkEvents(configuration);
			if (eventTriggered) {
				dataPacket.setGpsData(gpsData);
				packet = dataPacket.getPacket(EVENT, configuration);
				if (xBeeCell.getConnection() == 0) {
					isCellConnected = true;
					xBeeCell.dispatchUdpAt(packet);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				} else {
					isCellConnected = false;
					packetStorage.pushPacket(packet, EVENT);
				}
			}
		}
		indicators(isGpsValid, isCellConnected);
		if (textMessage.size() > 0) {
			std::cout << "Text Message: " << textMessage;
			std::cout << std::endl;
			configuration.parseConfiguration(textMessage);
		}
		//xBeeMesh.unpauseMesh();
		networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		//xBeeMesh.pauseMesh();
	}
}

void test_function_46()
{
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell;
	XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::vector<char> tx_data;
	std::vector<char> rx_packet;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	time_t connectCheckTime;
	time_t textCheckTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;
	SerialPort serialPort("/dev/ttymxc2", 9600);
	MeshTxPacket meshTxPacket;
	std::vector<char> inputBuffer;

	isCellConnected = false;
	gpio_set();
	xBeeMesh.pauseMesh();
	time(&lastReportTime);
	time(&connectCheckTime);
	time(&textCheckTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		time(&currentTime);
		if (difftime(currentTime, connectCheckTime) > CONNECTION_CHECK_TIME) {
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
			} else {
				isCellConnected = false;
			}
			time(&connectCheckTime);
		}
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			// Send out normal packet
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL, configuration);
			if (isCellConnected) {
				xBeeCell.dispatchUdpAt(packet);
			} else {
				xBeeMesh.unpauseMesh();
				packet.insert(packet.begin(), PACKET_UNSENT);
				meshTxPacket.setData(packet);
				meshTxPacket.setFrameId(0x01);
				tx_data = meshTxPacket.getPacket();
				std::cout << "Mesh tx packet will be sent: ";
				displayHexadecimal(tx_data);
				xBeeMesh.apiModeEntry();
				serialPort.openRts();
				serialPort.write(tx_data);
				inputBuffer.clear();
				serialPort.timedRead(inputBuffer, 2.0);
				displayHexadecimal(inputBuffer);
				xBeeMesh.apiModeExit();
				xBeeMesh.pauseMesh();
				// Push packet on queued stack
				packetStorage.pushPacket(packet, QUEUED);
			}
			time(&lastReportTime);
		} else if (difftime(currentTime, lastStoredPopTime) > STORED_PACKET_TIME) {
			// Deal with stored packets
			if (xBeeCell.getConnection() == 0) {
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			}
			time(&lastStoredPopTime);
		}
		xBeeMesh.unpauseMesh();
		rx_packet = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		xBeeMesh.pauseMesh();
		if (rx_packet.size() > 0) {
			if (verifyQueuedPacket(rx_packet)) {
				packetStorage.pushPacket(rx_packet, QUEUED);
			}
		} else {
			// Transmit stored mesh packets
			// and transfer
			rx_packet = packetStorage.popPacket(MESH);
			if (rx_packet.size() > 0) {
				if (isCellConnected) {
					// Transmit message through cell and mark complete
					if (rx_packet[0] == PACKET_UNSENT) {
						rx_packet.erase(rx_packet.begin());
						xBeeCell.dispatchUdpAt(rx_packet);
						rx_packet.insert(rx_packet.begin(), PACKET_SENT);
						packetStorage.pushPacket(rx_packet, MESH);
					} else {
						packetStorage.pushPacket(rx_packet, QUEUED);
					}	
				} else {
					// Transmit message through mesh
					xBeeMesh.unpauseMesh();
					meshTxPacket.setData(rx_packet);
					meshTxPacket.setFrameId(0x01);
					tx_data = meshTxPacket.getPacket();
					std::cout << "Mesh tx packet will be sent: ";
					displayHexadecimal(tx_data);
					xBeeMesh.apiModeEntry();
					serialPort.openRts();
					serialPort.write(tx_data);
					inputBuffer.clear();
					serialPort.timedRead(inputBuffer, 2.0);
					displayHexadecimal(inputBuffer);
					xBeeMesh.apiModeExit();
					xBeeMesh.pauseMesh();
					packetStorage.pushPacket(rx_packet, QUEUED);
				}
			} else {
				packetStorage.transferQueuedPackets();
			}
		}
		time(&currentTime);
		if (difftime(currentTime, textCheckTime) > TEXT_CHECK_TIME) {
			textMessage = xBeeCell.getText();
			if (textMessage.size() > 0) {
				configuration.parseConfiguration(textMessage);
			}
		}
	}
}

void test_function_47()
{
	XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> tx_data;
	std::string textMessage;
	SerialPort serialPort("/dev/ttymxc2", 9600);
	MeshTxPacket meshTxPacket;
	std::vector<char> inputBuffer;
	std::string output;

	output = "test_function_47()";
	packet = stringToVector(output);
	meshTxPacket.setData(packet);
	meshTxPacket.setFrameId(0x01);
	packet = meshTxPacket.getPacket();
	std::cout << "Mesh tx packet will be sent: ";
	displayHexadecimal(packet);
	xBeeMesh.apiModeEntry();
	serialPort.openRts();
	serialPort.write(packet);
	inputBuffer.clear();
	serialPort.timedRead(inputBuffer, 2.0);
	displayHexadecimal(inputBuffer);
	xBeeMesh.apiModeExit();
}

void test_function_48() 
{
	DataPacket dataPacket;
	GpsModule gpsModule(GPSMODULE_PORT, GPSMODULE_RATE);
	GpsData gpsData;
	XBeeCell xBeeCell;
	XBeeMesh xBeeMesh;
	std::vector<char> packet;
	std::vector<char> networkMessage;
	std::vector<char> tx_data;
	std::vector<char> rx_packet;
	std::string textMessage;
	time_t startTime, endTime;
	time_t currentTime;
	time_t connectCheckTime;
	bool eventTriggered;
	bool isCellConnected;
	bool isGpsValid;
	SerialPort serialPort("/dev/ttymxc2", 9600);
	MeshTxPacket meshTxPacket;
	std::vector<char> inputBuffer;

	isCellConnected = false;
	gpio_set();
	xBeeMesh.pauseMesh();
	time(&lastReportTime);
	time(&connectCheckTime);
	while (1) {
		gpsData.parseGpsData(gpsModule.getData());
		time(&currentTime);
		if (difftime(currentTime, connectCheckTime) > CONNECTION_CHECK_TIME) {
			if (xBeeCell.getConnection() == 0) {
				isCellConnected = true;
				std::cout << "isCellConnected = true";
				std::cout << std::endl;
			} else {
				isCellConnected = false;
				std::cout << "isCellConnected = false";
				std::cout << std::endl;
			}
			time(&connectCheckTime);
		}
		time(&currentTime);
		if (difftime(currentTime, lastReportTime) > STANDARD_PACKET_TIME) {
			// Send out normal packet
			dataPacket.setGpsData(gpsData);
			packet = dataPacket.getPacket(NORMAL, configuration);
			if (isCellConnected) {
				xBeeCell.dispatchUdpAt(packet);
			} else {
				xBeeMesh.unpauseMesh();
				packet.insert(packet.begin(), PACKET_UNSENT);
				meshTxPacket.setData(packet);
				meshTxPacket.setFrameId(0x01);
				tx_data = meshTxPacket.getPacket();
				std::cout << "Mesh tx packet will be sent: ";
				displayHexadecimal(tx_data);
				xBeeMesh.apiModeEntry();
				serialPort.openRts();
				serialPort.write(tx_data);
				inputBuffer.clear();
				serialPort.timedRead(inputBuffer, 2.0);
				displayHexadecimal(inputBuffer);
				xBeeMesh.apiModeExit();
				xBeeMesh.pauseMesh();
				// Push packet on queued stack
				packetStorage.pushPacket(packet, QUEUED);
			}
			time(&lastReportTime);
		} else if (difftime(currentTime, lastStoredPopTime) > STORED_PACKET_TIME) {
			// Deal with stored packets
			if (xBeeCell.getConnection() == 0) {
				packet = packetStorage.popPacket(EVENT);
				if (packet.size() > 0) {
					xBeeCell.dispatchUdpAt(packet);
				} else {
					packet = packetStorage.popPacket(NORMAL);
					if (packet.size() > 0) {
						xBeeCell.dispatchUdpAt(packet);
					}
				}
			}
			time(&lastStoredPopTime);
		}
		xBeeMesh.unpauseMesh();
		rx_packet = xBeeMesh.receiveData(MESH_LISTEN_TIME);
		xBeeMesh.pauseMesh();
		if (rx_packet.size() > 0) {
			if (verifyQueuedPacket(rx_packet)) {
				packetStorage.pushPacket(rx_packet, QUEUED);
			}
		} else {
			// Transmit stored mesh packets
			// and transfer
			rx_packet = packetStorage.popPacket(MESH);
			if (rx_packet.size() > 0) {
				if (isCellConnected) {
					// Transmit message through cell and mark complete
					if (rx_packet[0] == PACKET_UNSENT) {
						rx_packet.erase(rx_packet.begin());
						xBeeCell.dispatchUdpAt(rx_packet);
						rx_packet.insert(rx_packet.begin(), PACKET_SENT);
						packetStorage.pushPacket(rx_packet, MESH);
					} else {
						packetStorage.pushPacket(rx_packet, QUEUED);
					}
				} else {
					// Transmit message through mesh
					xBeeMesh.unpauseMesh();
					meshTxPacket.setData(rx_packet);
					meshTxPacket.setFrameId(0x01);
					tx_data = meshTxPacket.getPacket();
					std::cout << "Mesh tx packet will be sent: ";
					displayHexadecimal(tx_data);
					xBeeMesh.apiModeEntry();
					serialPort.openRts();
					serialPort.write(tx_data);
					inputBuffer.clear();
					serialPort.timedRead(inputBuffer, 2.0);
					displayHexadecimal(inputBuffer);
					xBeeMesh.apiModeExit();
					xBeeMesh.pauseMesh();
					packetStorage.pushPacket(rx_packet, QUEUED);
				}
			} else {
				packetStorage.transferQueuedPackets();
			}
		}
	}
}

void test_function_49()
{
	XBeeCell xBeeCell;
	XBeeMesh xBeeMesh;

	// If cell is 9600 baud, set to 115200 baud

	// If mesh is 115200 baud, set to 9600 baud

}
void test_function_50() { }
void test_function_51() { }
void test_function_52() { }
void test_function_53() { }
void test_function_54() { }
void test_function_55() { }
void test_function_56() { }
void test_function_57() { }
void test_function_58() { }
void test_function_59() { }
void test_function_60() { }
void test_function_61() { }
void test_function_62() { }
void test_function_63() { }
void test_function_64() { }
void test_function_65() { }
void test_function_66() { }
void test_function_67() { }
void test_function_68() { }
void test_function_69() { }
void test_function_70() { }
