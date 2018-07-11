#include "userlogindialog.h"
#include "ui_userlogindialog.h"
#include "database.h"
#include "debuginfo.h"

#include <iostream>

UserLoginDialog::UserLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLoginDialog)
{
    std::cout << "UserLoginDialog(QWidget *parent)" << std::endl;
    ui->setupUi(this);
}

UserLoginDialog::~UserLoginDialog()
{
    delete ui;
}

void UserLoginDialog::loginUser(User *myUser)
{
    user = myUser;
    userMain->setUser(user);
}

void UserLoginDialog::registerUserMainDialog(UserMain *myUserMain)
{
    userMain = myUserMain;
}

void UserLoginDialog::on_userLoginPushButton_clicked()
{
    QString username;

    username = ui->userLoginLineEdit->text();
    if (checkCredentials(username, "none")) {
        if (user != nullptr) {
            (*user).setIsLoggedIn(true);
        }
        if (userMain != nullptr) {
            userMain->setModal(true);
            userMain->setWindowFlags(Qt::Window);
#ifdef WINDOWS_IMPLEMENTATION
            userMain->show();
#else
            userMain->showFullScreen();
#endif
            userMain->exec();
        }
        this->close();
    } else {
        // Login failed, do something?
        if (user != nullptr) {
            (*user).setIsLoggedIn(false);
        }
    }
    ui->userLoginLineEdit->setText("");
}

bool UserLoginDialog::checkCredentials(QString username, QString password)
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

void UserLoginDialog::on_pushButton_0_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('0');
    ui->userLoginLineEdit->setText(temporary);
}

void UserLoginDialog::on_pushButton_1_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('1');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_2_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('2');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_3_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('3');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_4_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('4');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_5_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('5');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_6_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('6');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_7_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('7');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_8_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('8');
    ui->userLoginLineEdit->setText(temporary);
}


void UserLoginDialog::on_pushButton_9_clicked()
{
    QString temporary;

    temporary = ui->userLoginLineEdit->text();
    temporary.append('9');
    ui->userLoginLineEdit->setText(temporary);
}

void UserLoginDialog::on_pushButton_a_clicked()
{
    QString temporary;
    int length;

    temporary = ui->userLoginLineEdit->text();
    length = temporary.size();
    if (length > 0) {
        temporary.remove(length - 1, 1);
        ui->userLoginLineEdit->setText(temporary);
    }
}

void UserLoginDialog::on_pushButton_b_clicked()
{
    ui->userLoginLineEdit->setText("");
}

void UserLoginDialog::on_userExitPushButton_clicked()
{
    this->close();
}
