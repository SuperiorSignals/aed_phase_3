#include "adminmain.h"
#include "ui_adminmain.h"

#include "debuginfo.h"

#include <iostream>

AdminMain::AdminMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminMain)
{
    ui->setupUi(this);
    std::cout << "AdminMain(QWidget *parent)" << std::endl;
}

AdminMain::~AdminMain()
{
    delete ui;
}

void AdminMain::registerDialogs(ReportDialog *myReportDialog)
{
    reportDialog = myReportDialog;
}

void AdminMain::on_addUserPushButton_clicked()
{

}

void AdminMain::on_infoPushButton_clicked()
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

void AdminMain::on_exitPushButton_clicked()
{
    this->close();
}

void AdminMain::on_removeUserPushButton_clicked()
{

}

User *AdminMain::getUser() const
{
    return user;
}

void AdminMain::setUser(User *myUser)
{
    user = myUser;
}
