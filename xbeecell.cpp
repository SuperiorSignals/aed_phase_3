/*
*	Author: Stewart Nash
*	Date: 3/28/2018
*	File: xbeecell.cpp
*	Description: XBee cellular modem, source
*
*/
#include "xbeecell.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "configuration.h"
#include "vectorstring.h"
//#include "xbee.h"

extern Configuration configuration;

XBeeCell::XBeeCell()
{
	openPort("/dev/ttymxc1", 115200);
	connection = 35;
}

XBeeCell::XBeeCell(const char *portNumber, int baudRate)
{
	openPort(portNumber, baudRate);
	connection = 35;
}

/*
std::string XBeeCell::getConnection()
{
	std::string output;
	std::vector<char> input;

	enterPythonMode();
	writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
	readReply(0.5);
	writeString("import xbee\r\n");
	writeString("myXBee = xbee.XBee()\r\n");
	writeString("print('@IMEI:' + myXBee.atcmd('AI') + '$')\r\n");
	writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	input = readReply(0.5);
	//writeString("\x02"); // Ctrl+B (Exit raw REPL mode)
	//readReply(0.5);
	for (int i = 0; i < input.size(); i++) {
		if (input[i] > 0x2F && input[i] < 0x3A) {
			output.push_back(input[i]);
		}
	}

return output;
}
*/

int XBeeCell::getConnection()
{
	std::string message;
	std::vector<char> input;

	enterPythonMode();
	writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
	readReply(0.5);
	writeString("import xbee\r\n");
	writeString("myXBee = xbee.XBee()\r\n");
	writeString("print('@CONN:' + str(myXBee.atcmd('AI')) + '$')\r\n");
	writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	input = readReply(0.5);
	//writeString("\x02"); // Ctrl+B (Exit raw REPL mode)
	//readReply(0.5);
	//for (int i = 0; i < input.size(); i++) {
	//	if (input[i] > 0x2F && input[i] < 0x3A) {
	//		message.push_back(input[i]);
	//	}
	//}
	message = vectorToString(input);
	parseMessage(message);

	return connection;
}

std::string XBeeCell::getImei()
{
	std::string output;
	std::vector<char> input;

	enterPythonMode();
	writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
	readReply(0.5);
	writeString("import xbee\r\n");
	writeString("myXBee = xbee.XBee()\r\n");
	writeString("print('@IMEI:' + myXBee.atcmd('IM') + '$')\r\n");
	writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	input = readReply(0.5);
	//writeString("\x02"); // Ctrl+B (Exit raw REPL mode)
	//readReply(0.5);
	for (int i = 0; i < input.size(); i++) {
		if (input[i] > 0x2F && input[i] < 0x3A) {
			output.push_back(input[i]);
		}
	}

	return output;
}

std::string XBeeCell::getText()
{
	std::string output;
	std::vector<char> input;

	enterPythonMode();
	writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
	readReply(0.5);
	writeString("import network\r\n");
	writeString("import time\r\n");
	writeString("connection = network.Cellular()\r\n");
	writeString("while not connection.isconnected():\r\n");
	writeString("\ttime.sleep_ms(1500)\r\n");
	writeString("text = connection.sms_receive()\r\n");
	writeString("if text:\r\n");
	writeString("\tprint('@' + text['message'] + '$')\r\n");
	//writeString("\tprint('@PHON:' + text['sender'] + '$')\r\n");
	writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	input = readReply(1.0);
	//writeString("\x02"); // Ctrl+B (Exit raw REPL mode)
	//readReply(0.5);
	output = vectorToString(input);
	
	return output;
}

void XBeeCell::dispatchSmsPython(std::vector<char> textMessage, std::vector<char> phoneNumber)
{
	std::string phone;
	std::string message;

	phone = vectorToString(phoneNumber);
	message = vectorToString(textMessage);
	enterPythonMode();
	writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
	readReply(0.5);
	writeString("import network\r\n");
	writeString("import usocket\r\n");
	writeString("import time\r\n");
	writeString("connection = network.Cellular()\r\n");
	writeString("ph = ");
	writeString(phone);
	writeString("\r\n");
	writeString("maxWaitTime = 15\r\n");
	writeString("while not connection.isconnected()");
	writeString(" and (maxWaitTime > 0):\r\n");
	writeString("\ttime.sleep_ms(1500)\r\n");
	writeString("\tmaxWaitTime -= 1\r\n");
	writeString("if connection.isconnected():\r\n");
	writeString("\tconcat_text = '");
	writeString(message);
	writeString("'\r\n");
	writeString("\ttry:\r\n");
	writeString("\t\tconnection.sms_send(ph, concat_text)\r\n");
	writeString("\texcept OSError:\r\n");
	writeString("\t\tprint(\"@EROR:003$.\")\r\n");
	writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	readReply(1.0);
}

void XBeeCell::dispatchUdpAt(std::vector<char> input)
{
	std::cout << "void XBeeCell::dispacthUdpAt(std::vector<char>)" << std::endl;
	enterTransparentMode();
	enterCommandMode();
	writeCommand("ATDL 135.26.235.158\r"); // Set destination address
	//writeCommand("ATDL 23.89.198.205\r"); // Set destination address
	writeCommand("ATDE 143C\r"); // Set destination port in hex (0x143C = 5180)
	writeCommand("ATIP 0\r"); // Set protocol to UDP
	writeCommand("ATTD D\r"); // Set delimiter to 0x0D
	//writeCommand("ATWR\r"); // Apply changes
	writeCommand("ATAC\r"); // Apply changes immediately
	writeCommand("ATWR\r");
	exitCommandMode();
	input.push_back('\r');
	//input.push_back('\r');
	writeVector(input);
	readReply(1.0);
}

void XBeeCell::dispatchUdpAt(std::vector<char> input, std::string address, std::string port)
{
	enterTransparentMode();
	enterCommandMode();
	writeCommand("ATDL ");  
	writeCommand(address);
	writeCommand("\r"); // Set destination address
	writeCommand("ATDE "); 
	writeCommand(port); 
	writeCommand("\r"); // Set destination port in hex (0x143C = 5180)
	writeCommand("ATIP 0\r"); // Set protocol to UDP
	writeCommand("ATTD D\r"); // Set delimiter to 0x0D
	writeCommand("ATWR\r"); // Apply changes
	exitCommandMode();
	input.push_back('\r');
	//input.push_back('\r');
	writeVector(input);
	readReply(1.0);
}

void XBeeCell::dispatchUdpPython(std::vector<char> input)
{
	int length;

	enterPythonMode();
	writeString("\x01"); // Ctrl+A (Enter raw REPL mode)
	readReply(0.5);
	writeString("import usocket\r\n");
	writeString("import network\r\n");
	writeString("import time\r\n");
	writeString("UDP_IP = '135.26.235.158'\r\n");
	writeString("UDP_PORT = 5180\r\n");
	writeString("address = (UDP_IP, UDP_PORT)\r\n");
	//writeString("request = 'GET /ks/test.html HTTP/1.1\\r\\nHost: www.micropython.org\\r\\n\\r\\n'\r\n");
	writeString("cell = network.Cellular()\r\n");
	writeString("while not cell.isconnected():\r\n");
	writeString("\ttime.sleep_ms(1500)\r\n");
	writeString("mySocket = usocket.socket(usocket.AF_INET, usocket.SOCK_DGRAM)\r\n");
	writeString("mySocket.connect(address)\r\n");
	writeString("print('connected to ' + address[0] + '\\r\\n')\r\n");
	writeString("data = b'");
	length = input.size();
	for (int i = 0; i < length; i++) {
		writeString("\\x");
		unsigned char k;
		k = static_cast<unsigned char>(input[i]);
		int j = static_cast<int>(k);
		std::stringstream output;
		output << std::setfill('0') << std::setw(2) << std::hex << j;
		writeString(output.str());
	}
	writeString("'\r\n");
	//writeString("for i in data:\r\n");
	//writeString("\tprint(i)\r\n");
	//writeString("numberOfBytesSent = mySocket.sendall(data)\r\n");
	//writeString("mySocket.sendto(data, address)\r\n");
	writeString("mySocket.send(data)\r\n");
	//writeString("print('Bytes sent %d\\r\\n' % numberOfBytesSent)\r\n");
	//writeString("reply = mySocket.recv(512)\r\n");
	//writeString("print(reply)\r\n");
	writeString("mySocket.close()\r\n");
	writeString("print('socket closed\\r\\n')\r\n");
	//writeString("print('operation complete\\r\\n')\r\n");
	writeString("\x04"); // 0x04 = Ctrl+D (Save and execute paste code)
	//specialCharacter = 0x04; //Ctrl+D (Save and execute paste code)
	//serialPutChar(specialCharacter);
	//specialCharacter = 0x02; //Ctrl+B (exit raw REPL mode)
	//serialPutChar(specialCharacter);
	readReply(10.0);
}

void XBeeCell::enterPythonMode()
{
	enterCommandMode();
	writeCommand("ATTD D\r"); // Set delimiter to 0x0D (carriage return)
	writeCommand("ATAP 4\r"); // Operate in REPL mode
	writeCommand("ATAC\r"); // Apply changes
	exitCommandMode();
	readReply();
	setOperationMode(MICROPYTHON_MODE);
}

void XBeeCell::apiModeOperation()
{
	enterApiMode();
}

void XBeeCell::apiModeExit()
{
	exitApiMode();
}

/*
void XBeeCell::translateMessage(std::string message)
{

}
*/

void XBeeCell::parseMessage(std::string message)
{
	std::string output;
	InstructionType messageType;
	std::string temporary;
	
	if (message.size() > 7) { // Is this a proper minimum length?
		if (message.at(0) == '@' && message.at(message.size() - 1) == '$') { // Check string formatting
			temporary = message.substr(1, INSTRUCTION_LENGTH);
			if (temporary.compare("CONN:") == 0) {
				connection = std::stoi(temporary.substr(5, temporary.size() - 7));
			} else if (temporary.compare("CONF:") == 0) {
				configuration.parseConfiguration(temporary.substr(5, temporary.size() - 7));
			} else if (temporary.compare("EROR:") == 0) {

			} else if (temporary.compare("GEOF:") == 0) {

			} else if (temporary.compare("IMEI:") == 0) {

			} else if (temporary.compare("PHON:") == 0) {

			} else if (temporary.compare("PORT:") == 0) {

			} else if (temporary.compare("PROG:") == 0) {

			} else if (temporary.compare("TELE:") == 0) {

			} else if (temporary.compare("TEXT:") == 0) {

			} else if (temporary.compare("TIME:") == 0) {

			}			
		}
	}
}
