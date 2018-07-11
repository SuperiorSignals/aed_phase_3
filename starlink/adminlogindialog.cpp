#include "adminlogindialog.h"
#include "ui_adminlogindialog.h"
#include "debuginfo.h"

#include <iostream>

AdminLoginDialog::AdminLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminLoginDialog)
{
    std::cout << "AdminLoginDialog(QWidget *parent)" << std::endl;
    ui->setupUi(this);
}

AdminLoginDialog::~AdminLoginDialog()
{
    delete ui;
}

bool AdminLoginDialog::checkCredentials(QString username, QString password)
{
    database.readUserData();
    //database.printDatabase();
#ifdef WINDOWS_IMPLEMENTATION
    if (database.verifyCredentials(username.toLocal8Bit().constData(), password.toLocal8Bit().constData())) {
        database.populateUserInfo(username.toLocal8Bit().constData(), user);
        return true;
    }
#else
    if (database.verifyCredentials(username.toUtf8().constData(), password.toUtf8().constData())) {
        database.populateUserInfo(username.toUtf8().constData(), user);
        return true;
    }
#endif
    return false;
}

void AdminLoginDialog::loginUser(User *myUser)
{
    user = myUser;
    adminMain->setUser(user);
}

void AdminLoginDialog::on_adminLogSubmitPushButton_clicked()
{
    QString username;
    QString password;

    username = ui->adminLoginUserLineEdit->text();
    password = ui->adminLoginPassLineEdit->text();

    if (checkCredentials(username, password)) {
        ui->adminLoginWarningLabel->setText("Login successful!");
        if (user != nullptr) {
            (*user).setIsLoggedIn(true);
        }
        if (adminMain != nullptr) {
            adminMain->setModal(true);
            adminMain->setWindowFlags(Qt::Window);
#ifdef WINDOWS_IMPLEMENTATION
            adminMain->show();
#else
            adminMain->showFullScreen();
#endif
            adminMain->exec();
        }
        this->close();
    } else {
        ui->adminLoginWarningLabel->setText("Login failure! Please try again.");
        if (user != nullptr) {
            (*user).setIsLoggedIn(false);
        }
    }
    ui->adminLoginUserLineEdit->setText("");
    ui->adminLoginPassLineEdit->setText("");
}

void AdminLoginDialog::on_adminLogExitPushButton_clicked()
{
    this->close();
}

void AdminLoginDialog::registerAdminMainDialog(AdminMain *myAdminMain)
{
    adminMain = myAdminMain;
}
