#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include "adminlogindialog.h"
#include "user.h"
#include "userlogindialog.h"

namespace Ui {
class EntryDialog;
}

class EntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EntryDialog(QWidget *parent = 0);
    ~EntryDialog();
    void registerLoginDialog(AdminLoginDialog *adminLogin, UserLoginDialog *userLogin, User *myUser);

private slots:

    void on_adminPushButton_clicked();

    void on_userPushButton_clicked();

private:
    Ui::EntryDialog *ui;
    AdminLoginDialog *adminLoginDialog;
    UserLoginDialog *userLoginDialog;
    User *user;
};

#endif // ENTRYDIALOG_H
