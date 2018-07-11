#include "usermain.h"
#include "ui_usermain.h"
#include "checkdialog.h"
#include "debuginfo.h"
#include "reportdialog.h"

#include <iostream>

UserMain::UserMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserMain)
{
    std::cout << "UserMain(QWidget *parent)" << std::endl;
    ui->setupUi(this);
    isCheckDialogOpen = false;
    isReportDialogOpen = false;
}

UserMain::~UserMain()
{
    delete ui;
}

void UserMain::registerDialogs(CheckDialog *myCheckDialog, ReportDialog *myReportDialog)
{
    checkDialog = myCheckDialog;
    reportDialog = myReportDialog;
}


void UserMain::on_userMainOshaPushButton_clicked()
{
    if (checkDialog != nullptr) {
        //checkDialog->populateTabs();
        checkDialog->populateTabsCombined();
        checkDialog->setModal(true);
        checkDialog->setWindowFlags(Qt::Window);
#ifdef WINDOWS_IMPLEMENTATION
        checkDialog->show();
#else
        checkDialog->showFullScreen();
#endif
        checkDialog->exec();
    }
}

void UserMain::on_userMainExitPushButton_clicked()
{
    this->close();
}

void UserMain::on_userMainReportPushButton_clicked()
{
    if (reportDialog != nullptr) {
        reportDialog->setUser(user);
        reportDialog->refreshLabels();
        reportDialog->setModal(true);
        reportDialog->setWindowFlags(Qt::Window);
#ifdef WINDOWS_IMPLEMENTATION
        reportDialog->show();
#else
        reportDialog->showFullScreen();
#endif
        reportDialog->exec();
    }
}

User *UserMain::getUser() const
{
    return user;
}

void UserMain::setUser(User *myUser)
{
    user = myUser;
}
