#include "checklist.h"

#include <iostream>

CheckList::CheckList()
{
    std::cout << "CheckList()" << std::endl;
}

void CheckList::addListItem(std::string item, bool approval)
{
    this->listItems.push_back(item);
    this->isItemChecked.push_back(false);
    this->isItemBad.push_back(approval);
}

void CheckList::addListItem(std::string item)
{
    this->listItems.push_back(item);
    this->isItemChecked.push_back(false);
    this->isItemBad.push_back(false);
}

void CheckList::setItemChecked(unsigned int listIndex, bool value)
{
    if (isItemChecked.size() > listIndex) {
        isItemChecked[listIndex] = value;
    } else {
        // Error handling
    }
}

std::string CheckList::getListItem(unsigned int listIndex)
{
    if (listItems.size() > listIndex) {
        return listItems[listIndex];
    } else {
        return "";
        // Error handling
    }
}

bool CheckList::getItemChecked(unsigned int listIndex)
{
    if (isItemChecked.size() > listIndex) {
        return isItemChecked[listIndex];
    } else {
        return false;
        // Error handling
    }
}

bool CheckList::getItemApproval(unsigned int listIndex)
{
    if (isItemBad.size() > listIndex) {
        return isItemBad[listIndex];
    } else {
        return false;
        // Error handling
    }
}

int CheckList::getSize()
{
    return listItems.size();
}
