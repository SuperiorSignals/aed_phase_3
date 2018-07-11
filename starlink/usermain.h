#ifndef USERMAIN_H
#define USERMAIN_H

#include <QDialog>
#include "checkdialog.h"
#include "reportdialog.h"

namespace Ui {
class UserMain;
}

class UserMain : public QDialog
{
    Q_OBJECT

public:
    explicit UserMain(QWidget *parent = 0);
    ~UserMain();
    void registerDialogs(CheckDialog *myCheckDialog, ReportDialog *myReportDialog);

    User *getUser() const;
    void setUser(User *myUser);

private slots:
    void on_userMainOshaPushButton_clicked();

    void on_userMainExitPushButton_clicked();

    void on_userMainReportPushButton_clicked();

private:
    bool isCheckDialogOpen;
    bool isReportDialogOpen;
    Ui::UserMain *ui;
    CheckDialog *checkDialog;
    ReportDialog *reportDialog;
    User *user;
};

#endif // USERMAIN_H
