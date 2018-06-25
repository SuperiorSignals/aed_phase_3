/******************** (C) COPYRIGHT 2017 SSI - Stewart Nash ********************
* File Name		: api_mode.c
* Author			: SSI - Stewart Nash
* Date				: 9/8/2017
* Description	: Implementation of api for cell comms
********************************************************************************
* Detailed Description:
*
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, SSI/STEWART NASH SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "api_mode.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STANDARD_API_SIZE 80

#define CELL_INSTRUCTION_STRING_SIZE 48
#define SMALL_STRING_SIZE 16
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char standardApiArray[STANDARD_API_SIZE];
char serverAddress[CELL_INSTRUCTION_STRING_SIZE] = "135.26.235.158";
char serverPort[CELL_INSTRUCTION_STRING_SIZE] = "5180";
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name	: generateApiArray
* Description		: Make an ApiArray with given data
* Input					: delimiter - start delimiter
*								: length - data length
*								: data - given data
* Output				: None
* Return				: None
*******************************************************************************/
void generateApiArray(char delimiter, int length, char *data)
{
	
}

/*******************************************************************************
* Function Name	: generateApiFrame
* Description		: Make an ApiFrame structure with given data
* Input					: delimiter - start delimiter
*								: length - data length
*								: data - given data
* Output				: None
* Return				: None
*******************************************************************************/
void generateApiFrame(char delimiter, int length, char *data)
{
	
}

/*******************************************************************************
* Function Name	: packApiFrame
* Description		: Put ApiArray into an ApiFrame structure
* Input					: input - the array
*								: output - the frame
* Output				: None
* Return				: None
*******************************************************************************/
/*
void packApiFrame(char *input, struct ApiFrame *output)
{
	
}
*/

/*******************************************************************************
* Function Name	: packApiArray
* Description		: Fill ApiArray with contents of an ApiFrame structure
* Input					: input - the frame
*								: output - the array
* Output				: None
* Return				: None
*******************************************************************************/
/*
void packApiArray(struct ApiFrame *input, char *output)
{
	
}
*/

/*******************************************************************************
* Function Name	: dispatchApiFrameArray
* Description		: Dispatch character array of API frame to cell modem
* Input					: input - the frame
* Output				: None
* Return				: None
*******************************************************************************/
/*
void dispatchApiFrameArray(char *input)
{
	int length;
	int i;
		
	length = input[1];
	length = length << 8;
	length += input[2];
	length += 4;
	for (i = 0; i < length; i++) {
		serialPutChar(input[i]);
	}
}
*/

/*******************************************************************************
* Function Name	: dispatchApiFrame
* Description		: Dispatch API frame to cell modem
* Input					: input - the frame
* Output				: None
* Return				: None
*******************************************************************************/
/*
void dispatchApiFrame(struct ApiFrame *input)
{
	
}
*/
/*******************************************************************************
* Function Name	: dispatchRawDataAsApi
* Description		: Send given data as an API packet
* Input					: delimiter - start delimiter
*								: length - data length
*								: data - given data
* Output				: None
* Return				: None
*******************************************************************************/
/*
void dispatchRawDataAsApi(char delimiter, int length, char *data)
{
	int i;
	char j;
	
	serialPutChar(delimiter);
	serialPutInt(length);
	for (i = 0; i < length; i++) {
		j += data[i];
		serialPutChar(data[i]);
	}
	j = 0xFF - j;
	serialPutChar(j);
}
*/

/*******************************************************************************
* Function Name	: dispatchUdpApi
* Description		: Send data packet as with API
* Input					: packet - data packet
*								: length - data packet length
* Output				: None
* Return				: None
*******************************************************************************/
/*
void dispatchUdpApi(char *packet, int length)
{
	char output[STANDARD_API_ARRAY_SIZE];
	char address[5][SMALL_STRING_SIZE];
	int lengthCopy;
	unsigned char lengthHigh;
	unsigned char lengthLow;
	unsigned char *temporary;
	unsigned char id;
	int i, j;
	
	if (length > STANDARD_API_ARRAY_SIZE) {
		return;
	}
	lengthCopy = length;
	length += 16 - 4; // Subtract length of start delimiter, length, and checksum
	temporary = (char *) &length;
	lengthHigh = *temporary;
	lengthLow = *(temporary + 1);
	id = packet[16]; // set frame ID to packet identification number
	output[0] = API_START_DELIMITER; // Start delimiter
	output[1] = lengthHigh; // Length MSB - include only length of frame data
	output[2] = lengthLow; // Length LSB - include only length of frame data
	output[3] = API_IP_COMMAND_TYPE; // Frame type / API identifier (frame data) - AT command 0x08
	output[4] = id; // Frame ID / API frame ID (frame data)
	separateSmallString(serverAddress, address, '.');
	stringToInteger(address[0], &j);
	temporary = (char *) &j;
	output[5] = *(temporary + 1);
	//output[5] = 135;
	stringToInteger(address[1], &j);
	temporary = (char *) &j;
	output[6] = *(temporary + 1);
	//output[6] = 26;
	stringToInteger(address[2], &j);
	temporary = (char *) &j;
	output[7] = *(temporary + 1);
	//output[7] = 235;
	stringToInteger(address[3], &j);
	temporary = (char *) &j;
	output[8] = *(temporary + 1);
	//output[8] = 158;	
	stringToInteger(serverPort, &j);
	temporary = (char *) &j;
	output[9] = *temporary;
	//output[9] = 0x14;
	output[10] = *(temporary + 1);
	//output[10] = 0x3C;
	output[11] = 0; // UDP protocol
	output[12] = 0; // Transmit option, ignored for UDP
	for (i = 0; i < lengthCopy; i++) {
		output[13 + i] = packet[i];
	}
	output[i + 13] = *temporary; // Parameter value (frame data)
	output[i + 14] = *(temporary + 1); // Parameter value (frame data)
	output[i + 15] = generateApiChecksum(output, length);
	
	for (i = 0; i < length + 4; i++) {
		serialPutChar(output[i]);
	}
}
*/

/*******************************************************************************
* Function Name	: validateApiFrame
* Description		: Validate checksum of ApiFrame
* Input					: input - the frame
* Output				: None
* Return				: None
*******************************************************************************/
/*
BOOL validateApiFrame(struct ApiFrame *input)
{
	
}
*/

/*******************************************************************************
* Function Name	: generateApiAtCommand
* Description		: Generates AT command frame data for Api frame
* Input					: command - two ASCII characters of command
*								: id - frame ID
*								: parameter - value of parameter
*								: output - array which contains frame
* Output				: None
* Return				: None
*******************************************************************************/
void generateApiAtCommand(char *command, char id, int parameter, char *output)
{
	char *temporary;
	
	temporary = (char *) &parameter;
	output[0] = API_START_DELIMITER; // Start delimiter
	output[1] = 0; // Length MSB - include only length of frame data
	output[2] = 6; // Length LSB - include only length of frame data
	output[3] = API_AT_COMMAND_TYPE; // Frame type / API identifier (frame data) - AT command 0x08
	output[4] = id; // Frame ID / API frame ID (frame data)
	output[5] = command[0]; // AT command (frame data) - first ASCII character
	output[6] = command[1]; // AT command (frame data) - second ASCII character	
	output[7] = *temporary; // Parameter value (frame data)
	output[8] = *(temporary + 1); // Parameter value (frame data)
	output[9] = generateApiChecksum(output, 6);
}

/*******************************************************************************
* Function Name	: generateApiAtCommandNoParam
* Description		: Generates AT command frame data for Api frame
* Input					: command - two ASCII characters of command
*								: id - frame ID
*								: output - array which contains frame
* Output				: None
* Return				: None
*******************************************************************************/
void generateApiAtCommandNoParam(char *command, char id, char *output)
{
	output[0] = API_START_DELIMITER; // Start delimiter
	output[1] = 0; // Length MSB - include only length of frame data
	output[2] = 4; // Length LSB - include only length of frame data
	output[3] = API_AT_COMMAND_TYPE; // Frame type / API identifier (frame data) - AT command 0x08
	output[4] = id; // Frame ID / API frame ID (frame data)
	output[5] = command[0]; // AT command (frame data) - first ASCII character
	output[6] = command[1]; // AT command (frame data) - second ASCII character
	output[7] = generateApiChecksum(output, 4);
}

/*******************************************************************************
* Function Name	: generateApiChecksum
* Description		: Generates a checksum Api frame
* Input					: input - input frame
*								: length - length of the frame data
* Output				: None
* Return				: Checksum
*******************************************************************************/
char generateApiChecksum(char *input, int length)
{
	char output;
	int i, j;
	
	j = 0;
	for (i = 0; i < length; i++) {
			j += input[i + 3];
	}
	j = j & 0x00FF;
	output = j;
	output = 0xFF - output;
	
	return output;
}

//-------------------Test Functions-------------------------------

// Function:		test_function_10
// Location:		api_mode.c
// Description:	Send AT command via API

/*
void test_function_10(void)
{
	int i;
	char apiAtCommand[API_AT_FRAME_LENGTH];
	char command[3] = {'H', 'V', '\0'};
	char parameter = 1;
	char sample[15] = {0x7E, 0x00, 0x0A, 0x01, 0x01, 0x01,
	0x50, 0x01, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0xB8};
	
	generateApiAtCommand(command, 0, parameter, apiAtCommand);
	enterCommandMode();
	//serialPutString
	//enterApiMode();
	//dispatchRawDataAsApi(0x7E, API_AT_FRAME_LENGTH, apiAtCommand);
	//for (i = 0; i < 15; i++) {
	//	serialPutChar(sample[i]);
	//}
	//exitApiMode();	
}
*/

/*******************(C)COPYRIGHT 2017 Stewart Nash/SSI *****END OF FILE****/
