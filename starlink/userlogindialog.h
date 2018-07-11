#ifndef USERLOGINDIALOG_H
#define USERLOGINDIALOG_H

#include <QDialog>
#include "database.h"
#include "user.h"
#include "usermain.h"

namespace Ui {
class UserLoginDialog;
}

class UserLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserLoginDialog(QWidget *parent = 0);
    ~UserLoginDialog();
    void loginUser(User *myUser);
    void registerUserMainDialog(UserMain *myUserMain);

private slots:
    void on_userLoginPushButton_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_a_clicked();

    void on_pushButton_b_clicked();

    void on_userExitPushButton_clicked();

private:
    bool checkCredentials(QString username, QString password);
    Ui::UserLoginDialog *ui;
    Database database;
    User *user;
    UserMain *userMain;
};

#endif // USERLOGINDIALOG_H
