#include "database.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "debuginfo.h"
#include "user.h"
#include "userdata.h"

Database::Database()
{
    std::cout << "Database()" << std::endl;
}

void Database::readUserData()
{
    std::string temporary;

#ifdef WINDOWS_IMPLEMENTATION
    inputFile.open("C:\\opt\\starlink\\database.txt", std::ios::in);
#else
    inputFile.open("/opt/starlink/database.txt", std::ios::in);
#endif
    if (inputFile.is_open()) {
        while (!inputFile.eof()) {
            getline(inputFile, temporary);
            userData.userNames.push_back(temporary);
            //temporary >> inputFile;
            getline(inputFile, temporary);
            userData.passWords.push_back(temporary);
            //temporary >> inputFile;
            getline(inputFile, temporary);
            userData.firstNames.push_back(temporary);
            //temporary >> inputFile;
            getline(inputFile, temporary);
            userData.lastNames.push_back(temporary);
            //temporary >> inputFile;
            getline(inputFile, temporary);
            userData.authorization.push_back(temporary);
        }
        inputFile.close();
    } else {
        // Recover from error
    }
}

void Database::writeUserData()
{

}

void Database::copyUserData(UserData *myUserData)
{
    int i, length;

    length = userData.userNames.size();
    for (i = 0; i < length; i++) {
        myUserData->userNames.push_back(userData.userNames[i]);
        myUserData->passWords.push_back(userData.passWords[i]);
        myUserData->firstNames.push_back(userData.firstNames[i]);
        myUserData->lastNames.push_back(userData.lastNames[i]);
        myUserData->authorization.push_back(userData.authorization[i]);
    }
}

void Database::logData(std::vector<std::string> input)
{
    int i, length;
    time_t currentTime;
    char *dateTime;

    currentTime = time(NULL);
    dateTime = ctime(&currentTime);
    length = input.size();
#ifdef WINDOWS_IMPLEMENTATION
    outputFile.open("C:\\opt\\starlink\\log.txt", std::ios::out | std::ios::app);
#else
    outputFile.open("/opt/starlink/log.txt", std::ios::out | std::ios::app);
#endif
    if (outputFile.is_open()) {
        outputFile << dateTime << std::endl;
        for (i = 0; i < length; i++) {
            outputFile << input[i] << std::endl;
        }
        outputFile.close();
    }
}

std::vector<std::string> Database::readReportData()
{
    std::string temporary;
    std::vector<std::string> output;

#ifdef WINDOWS_IMPLEMENTATION
    inputFile.open("C:\\opt\\shared\\report.txt", std::ios::in);
#else
    inputFile.open("/opt/shared/report.txt", std::ios::in);
#endif
    if (inputFile.is_open()) {
        while (!inputFile.eof()) {
            getline(inputFile, temporary);
            output.push_back(temporary);
        }
        inputFile.close();
    } else {
        // Recover from error
    }

    return output;
}

bool Database::verifyCredentials(std::string myUserName, std::string myPassword)
{
    int i, length;
    int userNumber;
    bool userExists;

    userNumber = 0;
    userExists = false;
    length = userData.userNames.size();
    for (i = 0; i < length; i++) {
        if ((userData.userNames[i]).compare(myUserName) == 0) {
            if ((userData.passWords[i]).compare(myPassword) == 0) {
                userExists = true;
                userNumber = i;
            }
        }
    }

    return userExists;
}

void Database::populateUserInfo(std::string myUserName, User *myUser)
{
    int i, length;
    int userNumber;
    bool userExists;

    userNumber = 0;
    userExists = false;
    length = userData.userNames.size();
    for (i = 0; i < length; i++) {
        if ((userData.userNames[i]).compare(myUserName) == 0) {
            userExists = true;
            userNumber = i;
        }
    }
    if (userExists) {
        myUser->setUserName(userData.userNames[userNumber]);
        myUser->setPassWord(userData.passWords[userNumber]);
        myUser->setFirstName(userData.firstNames[userNumber]);
        myUser->setLastName(userData.lastNames[userNumber]);
        myUser->setAuthorization(userData.authorization[userNumber]);
    }
}

/*
void Database::printDatabase()
{
    int i, length;

    length = userData.userNames.size();
    for (i = 0; i < length; i++) {
        std::cout << userData.userNames[i] << std::endl;
        std::cout << userData.passWords[i] << std::endl;
        std::cout << userData.firstNames[i] << std::endl;
        std::cout << userData.lastNames[i] << std::endl;
        std::cout << userData.authorization[i] << std::endl;
    }
}
*/

void Database::readUserData(UserData *myUserData)
{

}

void Database::writeUserData(UserData *myUserData)
{

}

