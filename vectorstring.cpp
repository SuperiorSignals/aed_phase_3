/*
*	Author: Stewart Nash
*	Date: 3/26/2018
*	File: vectorstring.cpp
*	Description: Utility functions for converting vectors, source
*/
#include "vectorstring.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string vectorToString(std::vector<char> input)
{
	std::string output;
	int size;

	size = input.size();
	if (size > 0) {
		for (int i = 0; i < size; i++) {
			output.push_back(input[i]);
		}
		//output.push_back('\0');
	}

	return output;
}

std::vector<char> stringToVector(std::string input)
{
	std::vector<char> output;
	int size;

	size = input.size();
	if (size > 0) {
		for (int i = 0; i < size; i++) {
			output.push_back(input[i]);
		}
	}

	return output;
}

std::vector<std::string> newlineVectorToString(std::vector<char> input)
{
	std::string temporary;
	std::vector<std::string> output;
	int size;

	size = input.size();
	if (size > 0) {
		for (int i = 0; i < size; i++) {
			if (input[i] == '\n') {
				if (temporary.size() > 0) {
					output.push_back(temporary);
				}
				temporary.clear();
			} else {
				temporary.push_back(input[i]);
			}
		}
	}

	return output;
}

std::vector<std::string> commaStringToVector(std::string input)
{
	std::vector<std::string> output;
	std::stringstream streaming(input);
	std::string token;

	while (std::getline(streaming, token, ',')) {
		output.push_back(token);
	}

	return output;
}

/*
std::vector<std::string> splitString(std::string input, char delimiter)
{
	std::vector<std::string> output;
	std::vector<char> temporary;
	std::string token;

	for (int i = 0; i < input.size(); i++) {
		if (input[i] == delimiter) {
			if (i > 0) {
				output.push_back(vectorToString(temporary));
				temporary.clear();
			}
		} else {
			temporary.push_back(input[i]);
		}
	}

	return output;
}
*/

std::vector<std::string> splitString(std::string input, char delimiter)
{
	std::vector<std::string> output;
	std::stringstream streaming(input);
	std::string token;

	while (std::getline(streaming, token, delimiter)) {
		output.push_back(token);
	}

	return output;
}
