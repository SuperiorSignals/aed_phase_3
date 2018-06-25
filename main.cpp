#include <exception>
#include <iomanip>
#include <iostream>
#include <vector>
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

Configuration configuration;
PacketStorage packetStorage;
time_t mainStartTime;
time_t lastReportTime;

int main(int argc, char *argv[])
{
	time(&mainStartTime);
	int selection;
	/*
	test_function_33();
	*/
	while (1) {
		selection = 0;
		std::cout << "Enter test function: ";
		std::cin >> selection;
		if (selection > 0 && selection < 41) {
			test_menu(selection);
		}
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
