#include "user.h"
#include <iostream>
#include <string>

User::User()
{
    std::cout << "User()" << std::endl;
    isLoggedIn = false;
}

bool User::getIsLoggedIn()
{
    return isLoggedIn;
}

void User::setIsLoggedIn(bool input)
{
    isLoggedIn = input;
}

std::string User::getUserName()
{
    return userName;
}

void User::setUserName(std::string input)
{
    userName = input;
}

std::string User::getPassWord()
{
    return passWord;
}

void User::setPassWord(std::string input)
{
    passWord = input;
}

std::string User::getFirstName()
{
    return firstName;
}

void User::setFirstName(std::string input)
{
    firstName = input;
}

std::string User::getLastName()
{
   return lastName;
}

void User::setLastName(std::string input)
{
    lastName = input;
}

std::string User::getAuthorization()
{
    return authorization;
}

void User::setAuthorization(std::string input)
{
    authorization = input;
}

void User::logOut()
{
    this->setIsLoggedIn(false);
    this->setUserName("");
    this->setPassWord("");
    this->setFirstName("");
    this->setLastName("");
    this->setAuthorization("");
}
