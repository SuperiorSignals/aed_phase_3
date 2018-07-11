#ifndef USER_H
#define USER_H
#include <string>

class User
{
public:
    User();
    bool getIsLoggedIn();
    void setIsLoggedIn(bool input);
    std::string getUserName();
    void setUserName(std::string input);
    std::string getPassWord();
    void setPassWord(std::string input);
    std::string getFirstName();
    void setFirstName(std::string input);
    std::string getLastName();
    void setLastName(std::string input);
    std::string getAuthorization();
    void setAuthorization(std::string input);
    void logOut();

private:
    bool isLoggedIn;
    std::string userName;
    std::string passWord;
    std::string firstName;
    std::string lastName;
    std::string authorization;
};

#endif // USER_H
