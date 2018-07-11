#ifndef CHECKLIST_H
#define CHECKLIST_H

#include <vector>
#include <string>

#define CHECKLIST_LENGTH 64
#define CHECKLIST_STRING_LENGTH 512

class CheckList
{
public:
    CheckList();
    void addListItem(std::string item, bool approval);
    void addListItem(std::string);
    void setItemChecked(unsigned int listIndex, bool value); // Index starts at 0
    std::string getListItem(unsigned int listIndex);
    bool getItemChecked(unsigned int listIndex);
    bool getItemApproval(unsigned int listIndex); // If item should be false, is false; otherwise, true
    int getSize();
private:
    std::vector<std::string> listItems;
    std::vector<bool> isItemChecked;
    std::vector<bool> isItemBad;
};

#endif // CHECKLIST_H
