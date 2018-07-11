#include "checkdialog.h"
#include "ui_checkdialog.h"
#include "checkform.h"
#include "database.h"

#include <iostream>

#define CHECKBOXES_PER_PAGE 4

CheckDialog::CheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckDialog)
{
    std::cout << "CheckDialog(QWidget *parent)" << std::endl;
    motorOffCheckList = nullptr;
    motorOnCheckList = nullptr;
    user = nullptr;
    ui->setupUi(this);
}

CheckDialog::~CheckDialog()
{
    delete ui;
}

void CheckDialog::populateTabs(CheckList offCheckList, CheckList onCheckList)
{
    int offCount, onCount;
    int pageNumber, extraPage;
    int i;
    CheckForm *myCheckForm;

    offCount = offCheckList.getSize();
    onCount = onCheckList.getSize();

    pageNumber = offCount / 4;
    extraPage = offCount % 4;

    while (ui->tabWidget->count() > 0) {//If there are pre-existing tabs, delete them
        ui->tabWidget->removeTab(ui->tabWidget->count() - 1);
    }

    for (i = 0; i < pageNumber; i++) {
        ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(i));
        checkForms.push_back(myCheckForm);
        myCheckForm->completeText(offCheckList.getListItem(i * 4 + 0), 1);
        myCheckForm->completeText(offCheckList.getListItem(i * 4 + 1), 2);
        myCheckForm->completeText(offCheckList.getListItem(i * 4 + 2), 3);
        myCheckForm->completeText(offCheckList.getListItem(i * 4 + 3), 4);
    }

    if (extraPage > 0) {

    }
}

void CheckDialog::populateTabs()
{
    int offCount, onCount;
    int pageNumber, extraPage;
    int i;
    CheckForm *myCheckForm;

    offCount = motorOffCheckList->getSize();
    onCount = motorOnCheckList->getSize();

    pageNumber = offCount / CHECKBOXES_PER_PAGE;
    extraPage = offCount % CHECKBOXES_PER_PAGE;

    while (ui->tabWidget->count() > 0) {//If there are pre-existing tabs, delete them
        ui->tabWidget->removeTab(ui->tabWidget->count() - 1);
    }

    for (i = 0; i < pageNumber; i++) {
        ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(i));
        checkForms.push_back(myCheckForm);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 0), 1);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 1), 2);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 2), 3);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 3), 4);
    }

    if (extraPage > 0) {
        ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(pageNumber));
        checkForms.push_back(myCheckForm);
        for (i = 0; i < extraPage; i++) {
            myCheckForm->completeText(motorOffCheckList->getListItem(pageNumber * CHECKBOXES_PER_PAGE + i), i + 1);
        }
        myCheckForm->removeCheckBox(CHECKBOXES_PER_PAGE - extraPage);
    }
}

void CheckDialog::populateTabsCombined()
{
    int offCount, onCount;
    //int totalCount;
    //int pageNumber, extraPage;
    int offPageNumber, onPageNumber;
    //int totalPageNumber;
    int offExtraPage, onExtraPage;
    int i;
    CheckForm *myCheckForm;

    offCount = motorOffCheckList->getSize();
    onCount = motorOnCheckList->getSize();
    //totalCount = offCount + onCount;

    offPageNumber = offCount / CHECKBOXES_PER_PAGE;
    offExtraPage = offCount % CHECKBOXES_PER_PAGE;
    onPageNumber = onCount / CHECKBOXES_PER_PAGE;
    onExtraPage = offCount % CHECKBOXES_PER_PAGE;
    //totalPageNumber = offPageNumber + onPageNumber;

    while (ui->tabWidget->count() > 0) {//If there are pre-existing tabs, delete them
        ui->tabWidget->removeTab(ui->tabWidget->count() - 1);
    }

    for (i = 0; i < offPageNumber; i++) {
        ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(i));
        checkForms.push_back(myCheckForm);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 0), 1);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 1), 2);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 2), 3);
        myCheckForm->completeText(motorOffCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 3), 4);
    }

    if (offExtraPage > 0) {
        ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(offPageNumber));
        checkForms.push_back(myCheckForm);
        for (i = 0; i < offExtraPage; i++) {
            myCheckForm->completeText(motorOffCheckList->getListItem(offPageNumber * CHECKBOXES_PER_PAGE + i), i + 1);
        }
        myCheckForm->removeCheckBox(CHECKBOXES_PER_PAGE - offExtraPage);
    }

    for (i = 0; i < onPageNumber; i++) {
        if (offExtraPage > 0) {
            ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(i + offPageNumber + 1));
        } else {
            ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(i + offPageNumber));
        }
        checkForms.push_back(myCheckForm);
        myCheckForm->completeText(motorOnCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 0), 1);
        myCheckForm->completeText(motorOnCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 1), 2);
        myCheckForm->completeText(motorOnCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 2), 3);
        myCheckForm->completeText(motorOnCheckList->getListItem(i * CHECKBOXES_PER_PAGE + 3), 4);
    }

    if (onExtraPage > 0) {
        if (offExtraPage > 0) {
            ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(offPageNumber + onPageNumber + 1));
        } else {
            ui->tabWidget->addTab(myCheckForm = new CheckForm(), QString("Page %0").arg(offPageNumber + onPageNumber));
        }

        checkForms.push_back(myCheckForm);
        for (i = 0; i < onExtraPage; i++) {
            myCheckForm->completeText(motorOnCheckList->getListItem(onPageNumber * CHECKBOXES_PER_PAGE + i), i + 1);
        }
        myCheckForm->removeCheckBox(CHECKBOXES_PER_PAGE - onExtraPage);
    }
}

void CheckDialog::registerCheckList(CheckList *offCheckList, CheckList *onCheckList)
{
    motorOffCheckList = offCheckList;
    motorOnCheckList = onCheckList;
}

void CheckDialog::on_previousButton_clicked()
{
    int currentPosition;

    currentPosition = ui->tabWidget->currentIndex();
    if (currentPosition > 0) {
        ui->tabWidget->setCurrentIndex(currentPosition - 1);
    } else {
        //At beginning, do nothing
    }
}

void CheckDialog::on_nextButton_clicked()
{
    int currentPosition;

    currentPosition = ui->tabWidget->currentIndex();
    if (currentPosition < (ui->tabWidget->count() - 1)) {
        ui->tabWidget->setCurrentIndex(currentPosition + 1);
    } else {
        //At end, do nothing
    }
}

void CheckDialog::on_exitButton_clicked()
{
    this->close(); //Dialog has been closed, but not deleted
}

void CheckDialog::on_submitButton_clicked()
{
    int i, length;
    std::vector<std::string> output;
    Database database;

    if (user != nullptr) {
        output.push_back(user->getUserName());
    }
    confirmCheckedItems();
    length = motorOffCheckList->getSize();
    for (i = 0; i < length; i++) {
        if (motorOffCheckList->getItemChecked(i)) {
            output.push_back("true");
        } else {
            output.push_back("false");
        }
    }
    database.logData(output);
    this->close();
}

void CheckDialog::confirmCheckedItems()
{
    int i, j, length;
    int listSize;

    listSize = motorOffCheckList->getSize();
    length = checkForms.size();
    for (i = 0; i < length; i++) {
        j = i * 4 + 0;
        if ((checkForms[i])->getIsChecked(1)) {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, true);
            }
        } else {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, false);
            }
        }
        j = i * 4 + 1;
        if ((checkForms[i])->getIsChecked(2)) {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, true);
            }
        } else {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, false);
            }
        }
        j = i * 4 + 2;
        if ((checkForms[i])->getIsChecked(3)) {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, true);
            }
        } else {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, false);
            }
        }
        j = i * 4 + 3;
        if ((checkForms[i])->getIsChecked(4)) {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, true);
            }
        } else {
            if (j < listSize) {
                motorOffCheckList->setItemChecked(j, false);
            }
        }
    }
}

void CheckDialog::registerUser(User *myUser)
{
    user = myUser;
}
