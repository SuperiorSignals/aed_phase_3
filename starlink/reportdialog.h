#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>

#include "database.h"
#include "user.h"

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = 0);
    ~ReportDialog();

    User *getUser() const;
    void setUser(User *myUser);
    void refreshLabels();

private slots:
    void on_reportExitPushButton_clicked();

private:
    Ui::ReportDialog *ui;
    Database database;
    User *user;
};

#endif // REPORTDIALOG_H
