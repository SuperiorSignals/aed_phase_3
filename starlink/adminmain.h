#ifndef ADMINMAIN_H
#define ADMINMAIN_H

#include <QDialog>

#include "reportdialog.h"

namespace Ui {
class AdminMain;
}

class AdminMain : public QDialog
{
    Q_OBJECT

public:
    explicit AdminMain(QWidget *parent = 0);
    ~AdminMain();
    void registerDialogs(ReportDialog *myReportDialog);

    User *getUser() const;
    void setUser(User *myUser);

private slots:
    void on_addUserPushButton_clicked();

    void on_infoPushButton_clicked();

    void on_exitPushButton_clicked();

    void on_removeUserPushButton_clicked();

private:
    Ui::AdminMain *ui;
    ReportDialog *reportDialog;
    User *user;
};

#endif // ADMINMAIN_H
