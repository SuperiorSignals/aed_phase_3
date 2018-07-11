#ifndef USERDATA_H
#define USERDATA_H

#include <vector>
#include <string>

class UserData
{
public:
    UserData();
    std::vector<std::string> userNames;
    std::vector<std::string> passWords;
    std::vector<std::string> firstNames;
    std::vector<std::string> lastNames;
    std::vector<std::string> authorization; // Says if user is an administrator or normal user
};

#endif // USERDATA_H
