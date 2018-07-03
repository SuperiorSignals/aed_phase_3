#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include "configuration.h"
#include "datapacket.h"
#include "gpiopin.h"
#include "gpsdata.h"
#include "gpsmodule.h"
#include "packetstorage.h"
#include "serialport.h"
#include "testfunction.h"
#include "testfunctiontwo.h"
#include "vectorstring.h"
#include "xbee.h"
#include "xbeecell.h"

extern "C" {
#include "api_mode.h"
#include "gpio_interface.h"
}

#define STANDARD_PACKET_TIME 600
#define EVENT_PACKET_WAIT_TIME 60
#define MESH_LISTEN_TIME 5.0
#define SERVER_ADDRESS "135.26.235.158"
#define SERVER_PORT "143C"
#define GPSMODULE_PORT "/dev/ttymxc2"
#define GPSMODULE_RATE 9600
#define XBEECELL_PORT "/dev/ttymxc1"
#define XBEECELL_RATE 115200
#define XBEEMESH_PORT "/dev/ttymxc1"
#define XBEEMESH_RATE 115200

void standardRoutine();
void test_menu(int selection);

Configuration configuration;
PacketStorage packetStorage;
time_t mainStartTime;
time_t lastReportTime;

int main(int argc, char *argv[])
{
	time(&mainStartTime);
	int selection;
	bool validation;

	while (1) {
		selection = 0;
		std::cout << "Enter test function: ";
		if (std::cin >> selection) {
			validation = true;
		} else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			validation = false;
		}
		if (validation) {
			test_menu(selection);
		}
		//if (selection > 0 && selection < 70) {
		//	test_menu(selection);
		//}
	}

#ifdef WINDOWS_IMPLEMENTATION
	system("pause");
#endif

	return 0;
}

void standardRoutine()
{
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
		networkMessage = xBeeMesh.receiveData(MESH_LISTEN_TIME);
	}
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
		//test_function_43();
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

/*
void displayHexadecimal(char *buffer, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		std::cout << "(" << i << ") " << static_cast<int>(buffer[i]);
		if (buffer[i] > 0x1F && buffer[i] < 0x7F ) {
			std::cout << " [" << buffer[i] << "]";
		} else {

		}
	}
	std::cout << std::endl;
}
*/
