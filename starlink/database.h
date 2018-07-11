#ifndef DATABASE_H
#define DATABASE_H
#include "userdata.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "user.h"

#define DATABASE_FIELDS 5

class Database
{
public:
    Database();    
    void readUserData();
    void writeUserData();
    void copyUserData(UserData *myUserData);
    void logData(std::vector<std::string> input);
    std::vector<std::string> readReportData();
    bool verifyCredentials(std::string myUserName, std::string myPassword);
    void populateUserInfo(std::string myUserName, User *myUser);
    //void printDatabase();
private:
    void readUserData(UserData *myUserData);
    void writeUserData(UserData *myUserData);
    UserData userData;
    std::ifstream inputFile;
    std::ofstream outputFile;
};

#endif // DATABASE_H
