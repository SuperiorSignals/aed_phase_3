#include "entrydialog.h"
#include "ui_entrydialog.h"
#include "debuginfo.h"

#include <iostream>

EntryDialog::EntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryDialog)
{
    std::cout << "EntryDialog(QWidgent *parent)" << std::endl;
    ui->setupUi(this);
}

EntryDialog::~EntryDialog()
{
    delete ui;
}

void EntryDialog::registerLoginDialog(AdminLoginDialog *adminLogin, UserLoginDialog *userLogin, User *myUser)
{
    adminLoginDialog = adminLogin;
    userLoginDialog = userLogin;
    user = myUser;
}

void EntryDialog::on_adminPushButton_clicked()
{
    adminLoginDialog->setModal(true);
    adminLoginDialog->loginUser(user);
    adminLoginDialog->setWindowFlags(Qt::Window);
#ifdef WINDOWS_IMPLEMENTATION
    adminLoginDialog->show();
#else
    adminLoginDialog->showFullScreen();
#endif
    adminLoginDialog->exec();
}

void EntryDialog::on_userPushButton_clicked()
{
    userLoginDialog->setModal(true);
    userLoginDialog->loginUser(user);
    userLoginDialog->setWindowFlags(Qt::Window);
#ifdef WINDOWS_IMPLEMENTATION
    userLoginDialog->show();
#else
    userLoginDialog->showFullScreen();
#endif
    userLoginDialog->exec();
}
