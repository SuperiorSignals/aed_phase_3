/*
*	Author: Stewart Nash
*	Date: 3/26/2018
*	File: vectorstring.h
*	Description: Utility functions for converting vectors, header
*/
#pragma once
#ifndef VECTORSTRING_H_
#define VECTORSTRING_H_
#include <string>
#include <vector>

std::vector<std::string> commaStringToVector(std::string input);
std::vector<std::string> newlineVectorToString(std::vector<char> input);
std::string vectorToString(std::vector<char> input);
std::vector<std::string> splitString(std::string input, char delimiter);
std::vector<char> stringToVector(std::string input);

#endif // !VECTORSTRING_H_
