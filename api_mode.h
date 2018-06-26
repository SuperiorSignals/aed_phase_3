 /******************** (C) COPYRIGHT 2017 SSI /Stewart Nash ********************
* File Name          : api_mode.h
* Author             : SSI - Stewart Nash
* Date				 : 9/8/2017
* Description        : Headers for API communications with cell
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _API_MODE_H
	#define _API_MODE_H
	/* Includes ------------------------------------------------------------------*/
	//#include "stdio.h"
	//#include "string.h"
	/* Private define ------------------------------------------------------------*/
	#define STANDARD_API_ARRAY_SIZE 128
	#define API_AT_FRAME_LENGTH 5
	#define API_START_DELIMITER 0x7E
	#define API_AT_COMMAND_TYPE 0x08
	#define API_IP_COMMAND_TYPE 0x20
	#define API_AT_TOTAL_LENGTH 10
	#define API_AT_NO_PARAM_TOTAL_LENGTH 8
	#define MAX_DATA 512

	
	/* Exported types ------------------------------------------------------------*/
typedef unsigned char BOOL;
	/* Exported constants --------------------------------------------------------*/
	/* Exported macro ------------------------------------------------------------*/
	/* Exported functions --------------------------------------------------------*/
	void generateApiAtPacket(char packet[MAX_DATA], char api_frame, char at_command[2], char frame_length[2], char *raw_data);
	//void generateApiArray(char delimiter, int length, char *data, char checksum);	
	void generateApiArray(char delimiter, int length, char *data);
	//void generateApiFrame(char delimiter, int length, char *data, char checksum);
	void generateApiFrame(char delimiter, int length, char *data);
	//void packApiFrame(char *input, struct ApiFrame *output);
	//void packApiArray(struct ApiFrame *input, char *output);
	//void dispatchApiFrameArray(char *input);
	//void dispatchApiFrame(struct ApiFrame *input);
	//void dispatchRawDataAsApi(char delimiter, int length, char *data, char checksum);
	//void dispatchRawDataAsApi(char delimiter, int length, char *data);
	//void dispatchUdpApi(char *packet, int length);
	void generateApiAtCommand(char *command, char id, int parameter, char *output);
	void generateApiAtCommandNoParam(char *command, char id, char *output);
	char generateApiChecksum(char *input, int length);
	//BOOL validateApiFrame(struct ApiFrame *input);
	//void enterApiMode(void);
	//void exitApiMode(void);
	/* Test functions ------------------------------------------------------------*/
	//void test_function_10(void);
	
	struct ApiFrame {
		char startDelimiter;
		int length;
		char *frameData;
		char checkSum;
	};
	
#endif  /* _API_MODE_H */

/*******************(C)COPYRIGHT 2017 SSI/Stewart Nash *****END OF FILE****/
