#ifndef CHECKDIALOG_H
#define CHECKDIALOG_H

#include <QDialog>
#include "checkform.h"
#include "checklist.h"
#include "user.h"
#include <vector>

namespace Ui {
class CheckDialog;
}

class CheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckDialog(QWidget *parent = 0);
    ~CheckDialog();
    void populateTabs(CheckList offCheckList, CheckList onCheckList);
    void populateTabs();
    void populateTabsCombined();
    void registerCheckList(CheckList *offCheckList, CheckList *onCheckList);
    void registerUser(User *myUser);

private slots:
    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_exitButton_clicked();

    void on_submitButton_clicked();

private:
    Ui::CheckDialog *ui;
    CheckList *motorOffCheckList;
    CheckList *motorOnCheckList;
    std::vector<CheckForm *> checkForms;
    User *user;
    void confirmCheckedItems();
};

#endif // CHECKDIALOG_H
