#ifndef ADMINLOGINDIALOG_H
#define ADMINLOGINDIALOG_H

#include <QDialog>
#include "user.h"
#include "adminmain.h"
#include "database.h"

namespace Ui {
class AdminLoginDialog;
}

class AdminLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminLoginDialog(QWidget *parent = 0);
    ~AdminLoginDialog();
    void loginUser(User *myUser);
    void registerAdminMainDialog(AdminMain *myAdminMain);

private slots:
    void on_adminLogSubmitPushButton_clicked();
    void on_adminLogExitPushButton_clicked();

private:
    Ui::AdminLoginDialog *ui;
    bool checkCredentials(QString username, QString password);
    User *user;
    AdminMain *adminMain;
    Database database;
};

#endif // ADMINLOGINDIALOG_H
